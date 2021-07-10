// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/modules.h"

#include "gonk/gonk.h"
#include "gonk/plugin.h"

#include <QFileInfo>
#include <QDir>
#include <QDirIterator>
#include <QLibrary>
#include <QSettings>

namespace gonk
{

void module_load_callback(script::Module m)
{
  ModuleManager& manager = Gonk::Instance().moduleManager();
  ModuleInfo info = manager.getModuleInfo(m);

  for (std::string dep : info.dependencies)
    manager.loadModule(dep);

  std::string lib_name = info.name;
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

  if(!funpointer)
  {
    throw script::ModuleLoadingError{ "Could not find entry point in module" };
  }

  gonk::Plugin*(*plugin_getter)() = reinterpret_cast<gonk::Plugin*(*)()>(funpointer);

  gonk::Plugin* plugin = plugin_getter();

  if (!plugin)
  {
    throw script::ModuleLoadingError{ "Module library returned a null plugin" };
  }

  manager.attachPlugin(m, std::shared_ptr<gonk::Plugin>(plugin));

  plugin->load(m);
}

void module_cleanup_callback(script::Module m)
{
  ModuleManager& manager = Gonk::Instance().moduleManager();
  ModuleInfo info = manager.getModuleInfo(m);

  if(info.plugin)
    info.plugin->unload(m);
}

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
  std::unordered_map<script::ModuleInterface*, ModuleInfo>& m_output;
  std::vector<ModuleInfo> m_modules;
public:

  ModuleImporter(script::Engine* e, std::vector<std::string>& import_paths, std::unordered_map<script::ModuleInterface*, ModuleInfo>& out)
    : m_engine(e), m_import_paths(import_paths), m_output(out)
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
    QDirIterator iterator{ QString::fromStdString(dirpath), QDir::NoDotAndDotDot | QDir::Dirs, QDirIterator ::NoIteratorFlags };

    while (iterator.hasNext())
    {
      QString path = iterator.next();
      QFileInfo gonkmodule = path + "/gonkmodule";

      if (!gonkmodule.exists())
        continue;

      QSettings settings{ gonkmodule.absoluteFilePath(), QSettings::IniFormat };

      ModuleInfo module_info;
      module_info.name = settings.value("name", QString()).toString().toStdString();
      module_info.entry_point = settings.value("entry_point", QString()).toString().toStdString();

      QStringList dependencies = settings.value("dependencies", QString()).toString().split(',', QString::SkipEmptyParts);
      for (auto d : dependencies)
        module_info.dependencies.push_back(d.toStdString());

      module_info.path = gonkmodule.absoluteFilePath().remove("/gonkmodule").toStdString();

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
      std::string name = split_module_name(tree.module_info.name).back();

      if (parent_module.isNull())
      {
        script::Module m = m_engine->newModule(name, module_load_callback, module_cleanup_callback);
        m_output[m.impl()] = tree.module_info;
      }
      else
      {
       
        script::Module m = parent_module.newSubModule(name, module_load_callback, module_cleanup_callback);
        m_output[m.impl()] = tree.module_info;
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
      std::vector<std::string> names = split_module_name(m.name);
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

ModuleManager::ModuleManager(script::Engine* e)
  : m_script_engine(e)
{

}

ModuleManager::~ModuleManager()
{

}

void ModuleManager::addImportPath(std::string dir)
{
  if (!QFileInfo(QString::fromStdString(dir)).isDir())
    return;

  m_import_paths.push_back(dir);
}

const std::vector<std::string>& ModuleManager::importPaths() const
{
  return m_import_paths;
}

ModuleInfo ModuleManager::getModuleInfo(const script::Module& m) const
{
  auto it = m_module_infos.find(m.impl());

  if (it == m_module_infos.end())
    return {};

  return it->second;
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
  ModuleImporter importer{ m_script_engine, m_import_paths, m_module_infos };
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

void ModuleManager::attachPlugin(script::Module m, std::shared_ptr<Plugin> plugin)
{
  m_module_infos[m.impl()].plugin = plugin;
}

} // namespace gonk
