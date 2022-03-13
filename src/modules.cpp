// Copyright (C) 2020-2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/modules.h"

#include "gonk/gonk.h"
#include "gonk/gonkmodule.h"
#include "gonk/plugin.h"

#include <QLibrary>

#include <filesystem>

namespace gonk
{

std::vector<std::string> split_module_name(std::string s)
{
  std::vector<std::string> result;
  size_t pos = 0;
  while ((pos = s.find('.')) != std::string::npos) {
    result.push_back(s.substr(0, pos));
    s.erase(0, pos + 1);
  }
  result.push_back(s);
  return result;
}


struct ModuleTree
{
  std::string name;
  ModuleInfo module_info;
  std::map<std::string, std::shared_ptr<ModuleTree>> children;

  ModuleTree() = default;
  ModuleTree(std::string n) : name(std::move(n)) {}

  ModuleTree& operator[](const std::string& key)
  {
    std::shared_ptr<ModuleTree>& child = children[key];
    if (child == nullptr)
      child = std::make_shared<ModuleTree>(key);
    return *child;
  }

  ModuleTree& operator=(const ModuleInfo& info)
  {
    module_info = info;
    return *(this);
  }
};

class ModuleImporter
{
  script::Engine* m_engine;
  std::vector<std::string>& m_import_paths;
  std::vector<ModuleInfo> m_modules;
public:

  ModuleImporter(script::Engine* e, std::vector<std::string>& import_paths)
    : m_engine(e), m_import_paths(import_paths)
  {

  }

  void load()
  {
    for (const auto& p : m_import_paths)
      load(p);

    createModules();
  }

protected:

  void load(std::string dirpath)
  {
    std::filesystem::directory_iterator iterator{ dirpath };

    for(const std::filesystem::directory_entry& entry : iterator)
    {
      if (!entry.is_directory())
        continue;

      std::filesystem::path gonkmodule = entry.path() / "gonkmodule";

      if (!std::filesystem::exists(gonkmodule))
        continue;

      GonkModuleFile gonkmodulefile = GonkModuleFile::read(gonkmodule);

      ModuleInfo module_info;
      module_info.path = gonkmodule.parent_path().string();
      module_info.fullname = gonkmodulefile.name;
      module_info.sourcefile = gonkmodulefile.sourcefile.value_or(std::string());
      module_info.entry_point = gonkmodulefile.entry_point.value_or(std::string());
      module_info.dependencies = gonkmodulefile.dependencies.value_or(std::vector<std::string>());

      m_modules.push_back(module_info);
    }
  }

  void createModule(ModuleTree& tree, std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end, const ModuleInfo& info)
  {
    if (std::distance(begin, end) == 1)
      tree[*begin] = info;
    else
      createModule(tree[*begin], begin + 1, end, info);
  }

  void createModule(ModuleTree& tree, script::Module parent_module)
  {
    if (tree.children.empty())
    {
      std::string name = split_module_name(tree.module_info.fullname).back();

      if (parent_module.isNull())
      {
        m_engine->newModule<GonkModuleInterface>(m_engine, std::move(name), tree.module_info);
      }
      else
      {
       
        parent_module.newSubModule<GonkModuleInterface>(m_engine, std::move(name), tree.module_info);
      }
    }
    else
    {
      script::Module m;

      if (parent_module.isNull())
        m = m_engine->newModule(tree.name);
      else
        m = parent_module.newSubModule(tree.name);

      for (const auto& child : tree.children)
        createModule(*(child.second), m);
    }
  }

  void createModules()
  {
    ModuleTree tree;

    for (const auto& m : m_modules)
    {
      std::vector<std::string> names = split_module_name(m.fullname);
      createModule(tree, names.cbegin(), names.cend(), m);
    }

    if (!tree.children.empty())
    {
      for (auto m : tree.children)
      {
        createModule(*(m.second), script::Module());
      }
    }
  }
};


GonkModuleInterface::GonkModuleInterface(script::Engine* e, std::string name, ModuleInfo minfo)
  : script::ModuleInterface(e, std::move(name)),
    info(minfo)
{
  if(minfo.sourcefile.empty())
    script = e->newScript(script::SourceFile::fromString(""));
  else
    script = e->newScript(script::SourceFile(info.path + "/" + info.sourcefile));
}


bool GonkModuleInterface::is_loaded() const
{
  return loaded;
}

void GonkModuleInterface::load()
{
  loadDependencies();
  loadPlugin();

  plugin->load(script::Module(shared_from_this()));

  loadChildren();
  loadScript();

  loaded = true;
}

void GonkModuleInterface::unload()
{
  plugin->unload(script::Module(shared_from_this()));
  loaded = false;
}

script::Script GonkModuleInterface::get_script() const
{
  return script;
}

script::Namespace GonkModuleInterface::get_global_namespace() const
{
  return get_script().rootNamespace();
}

const std::vector<script::Module>& GonkModuleInterface::child_modules() const
{
  return modules;
}

void GonkModuleInterface::add_child(script::Module m)
{
  modules.push_back(m);
}

void GonkModuleInterface::loadScript()
{
  if (script.isCompiled())
    return;

  if (!script.source().isLoaded())
  {
    script::SourceFile file = script.source();
    file.load();
  }

  if (script.source().content().empty())
    return;

  compile(script);
}

void GonkModuleInterface::loadChildren()
{
  for (script::Module m : modules)
    m.load();
}

void GonkModuleInterface::loadDependencies()
{
  ModuleManager& manager = Gonk::Instance().moduleManager();

  for (std::string dep : info.dependencies)
    manager.loadModule(dep);
}

void GonkModuleInterface::loadPlugin()
{
  ModuleManager& manager = Gonk::Instance().moduleManager();

  // @TODO: why should the fullname constrain the dll name ?

  std::string lib_name = info.fullname;
  for (char& c : lib_name)
  {
    if (c == '.')
      c = '-';
  }

  QLibrary library{ QString::fromStdString(info.path + "/" + lib_name) };

  if (!library.load())
  {
    throw script::ModuleLoadingError{ "Could not load module library" };
  }

  void (*funpointer)() = library.resolve(info.entry_point.c_str());

  if (!funpointer)
  {
    throw script::ModuleLoadingError{ "Could not find entry point in module" };
  }

  gonk::Plugin* (*plugin_getter)() = reinterpret_cast<gonk::Plugin * (*)()>(funpointer);

  gonk::Plugin* p = plugin_getter();

  if (!p)
  {
    throw script::ModuleLoadingError{ "Module library returned a null plugin" };
  }

  plugin.reset(p);
}


ModuleManager::ModuleManager(script::Engine* e)
  : m_script_engine(e)
{

}

ModuleManager::~ModuleManager()
{

}

void ModuleManager::addImportPath(std::string dir)
{
  if (!std::filesystem::is_directory(dir))
    return;

  m_import_paths.push_back(dir);
}

const std::vector<std::string>& ModuleManager::importPaths() const
{
  return m_import_paths;
}

script::Module ModuleManager::getModule(const std::string& name) const
{
  script::Module m;

  auto it = std::find(name.begin(), name.end(), '.');

  if (it == name.end())
  {
    m = m_script_engine->getModule(name);
  }
  else
  {
    m = m_script_engine->getModule(std::string(name.begin(), it));

    while (it != name.end() && !m.isNull())
    {
      ++it;
      auto other_it = std::find(it, name.end(), '.');
      m = m.getSubModule(std::string(it, other_it));
      it = other_it;
    }
  }

  return m;
}

void ModuleManager::fetchModules()
{
  ModuleImporter importer{ m_script_engine, m_import_paths };
  importer.load();
}

static void load_module(script::Engine& engine, std::vector<std::string>::const_iterator begin, std::vector<std::string>::const_iterator end, script::Module parent)
{
  if (begin == end)
  {
    if (!parent.isNull())
      parent.load();
  }
  else
  {
    if (parent.isNull())
      load_module(engine, begin + 1, end, engine.getModule(*begin));
    else
      load_module(engine, begin + 1, end, parent.getSubModule(*begin));
  }
}

void ModuleManager::loadModule(const std::string& name)
{
  auto names = split_module_name(name);
  load_module(*m_script_engine, names.cbegin(), names.cend(), script::Module());
}

} // namespace gonk
