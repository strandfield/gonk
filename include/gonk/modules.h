// Copyright (C) 2020-2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_MODULES_H
#define GONK_MODULES_H

#include "gonk/gonk-defs.h"

#include <script/module.h>
#include <script/module-interface.h>
#include <script/script.h>

#include <dynlib/dynlib.h>

namespace gonk
{

class Plugin;

struct ModuleInfo
{
  std::string fullname;
  std::string path;
  std::string sourcefile;
  std::string entry_point;
  std::vector<std::string> dependencies;
};

class GONK_API GonkModuleInterface : public script::ModuleInterface
{
public:
  ModuleInfo info;
  std::shared_ptr<dynlib::Library> library;
  std::shared_ptr<Plugin> plugin;
  script::Script script;
  std::vector<script::Module> modules;
  bool loaded = false;
  
public:
  GonkModuleInterface(script::Engine* e, std::string name, ModuleInfo minfo);

  bool is_loaded() const override;
  void load() override;
  void unload() override;
  script::Script get_script() const override;
  script::Namespace get_global_namespace() const override;
  const std::vector<script::Module>& child_modules() const;
  void add_child(script::Module m);

public:
  void loadScript();
  void loadChildren();

protected:
  void loadDependencies();
  void loadPlugin();
};

class GONK_API ModuleManager
{
public:
  explicit ModuleManager(script::Engine* e);
  ~ModuleManager();

  void addImportPath(std::string dir);
  const std::vector<std::string>& importPaths() const;

  script::Module getModule(const std::string& name) const;

  void fetchModules();

  void loadModule(const std::string& name);

private:
  script::Engine* m_script_engine;
  std::vector<std::string> m_import_paths;
};

} // namespace gonk

#endif // GONK_MODULES_H
