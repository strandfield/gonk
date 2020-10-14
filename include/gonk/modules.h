// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_MODULES_H
#define GONK_MODULES_H

#include "gonk/gonk-defs.h"

#include <script/module.h>

#include <unordered_map>

namespace gonk
{

class Plugin;

struct ModuleInfo
{
  std::string name;
  std::string path;
  std::string entry_point;
  std::vector<std::string> dependencies;
  std::shared_ptr<Plugin> plugin;
};

class GONK_API ModuleManager
{
public:
  explicit ModuleManager(script::Engine* e);
  ~ModuleManager();

  void addImportPath(std::string dir);
  const std::vector<std::string>& importPaths() const;

  ModuleInfo getModuleInfo(const script::Module& m) const;

  script::Module getModule(const std::string& name) const;

  void fetchModules();

  void loadModule(const std::string& name);

  void attachPlugin(script::Module m, std::shared_ptr<Plugin> plugin);

private:
  script::Engine* m_script_engine;
  std::vector<std::string> m_import_paths;
  std::unordered_map<script::NamespaceImpl*, ModuleInfo> m_module_infos;
};

} // namespace gonk

#endif // GONK_MODULES_H
