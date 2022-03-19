// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/gonk.h"

#include "gonk/builtins.h"
#include "gonk/modules.h"
#include "gonk/pretty-print.h"
#include "gonk/script-runner.h"
#include "gonk/version.h"

#include <script/context.h>
#include <script/function.h>
#include <script/module.h>
#include <script/namespace.h>

#include <filesystem>
#include <fstream>
#include <iostream>

#ifdef _WIN32
#include <windows.h>    //GetModuleFileNameW
#else
#include <limits.h>
#include <unistd.h>     //readlink
#endif

Gonk* Gonk::m_instance = nullptr;

// from https://stackoverflow.com/questions/1528298/get-path-of-executable
static std::filesystem::path getexepath()
{
#ifdef _WIN32
  wchar_t path[MAX_PATH] = { 0 };
  GetModuleFileNameW(NULL, path, MAX_PATH);
  return path;
#else
  char result[PATH_MAX];
  ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
  return std::string(result, (count > 0) ? count : 0);
#endif
}

static std::string executable_dir()
{
  std::string path = getexepath().string();
  for (char& c : path)
  {
    if (c == '\\')
      c = '/';
  }

  size_t offset = path.find_last_of('/');
  path.erase(path.begin() + offset + 1, path.end());
  return path;
}

Gonk::Gonk(int & argc, char **argv)
  : m_cli(argc, argv)
{
  m_instance = this;

  setupEngine();

  m_module_manager = std::make_unique<gonk::ModuleManager>(&m_engine);
  m_printer = std::make_unique<gonk::PrettyPrinter>(m_engine);
}

Gonk::~Gonk()
{
  m_engine.tearDown();
}

int Gonk::argc() const
{
  return m_cli.argc;
}

char** Gonk::argv() const
{
  return m_cli.argv;
}

std::string Gonk::argv(size_t index) const
{
  return std::string(argv()[index]);
}

int Gonk::exec()
{
  if (cli().empty())
  {
    std::cout << "Gonk!" << std::endl;
    return 0;
  }

  m_module_manager->addImportPath(executable_dir() + "/modules");
  m_module_manager->fetchModules();

  if (cli().version)
  {
    std::cout << gonk::versionstr() << std::endl;
    return 0;
  }
  else if (cli().help)
  {
    return displayHelp();
  }
  else if (cli().interactive)
  {
    return interactiveSession();
  }
  else if (cli().list_modules)
  {
    listModules();
    return 0;
  }
  else
  {
    return runScript();
  }
}

Gonk& Gonk::Instance()
{
  return *m_instance;
}

const gonk::CLI& Gonk::cli() const
{
  return m_cli;
}

gonk::ModuleManager& Gonk::moduleManager() const
{
  return *m_module_manager;
}

void Gonk::setupEngine()
{
  m_engine.setup();

  script::Namespace ns = m_engine.rootNamespace();
  gonk::register_builtins(ns);
}

int Gonk::displayHelp()
{
  m_cli.displayHelp();
  return 0;
}

int Gonk::interactiveSession()
{
  for (;;)
  {
    std::cout << ">>> " << std::flush;

    std::string command;
    std::getline(std::cin, command);

    if (command.empty())
      continue;

    if (command.at(0) == ':')
    {
      if (command == ":q")
      {
        break;
      }
    }
    else if (command.find("import ") == 0)
    {
      std::string module_name{ command.begin() + 7, command.end() };
      while (module_name.back() == ' ' || module_name.back() == ';')
        module_name.pop_back();
      importModule(module_name);
    }
    else
    {
      eval(command);
    }
  }

  return 0;
}

void list_modules(script::Module m, std::string prefix)
{
  std::cout << prefix << m.name() << "\n";

  for (const auto& smod : m.submodules())
  {
    list_modules(smod, prefix + m.name() + ".");
  }
}

void Gonk::listModules()
{
  for (const auto& m : scriptEngine()->modules())
  {
    list_modules(m, "");
  }
}

void Gonk::eval(std::string cmd)
{
  try
  {
    script::Value v = scriptEngine()->eval(cmd);
    display(v);
  }
  catch (const std::runtime_error & ex)
  {
    std::cout << ex.what() << std::endl;
  }
  catch (const script::EvaluationError & ex)
  {
    std::cout << ex.message << std::endl;
  }
}

int Gonk::runScript()
{
  gonk::ScriptRunner runner{ *this };
  return runner.run();
}

void Gonk::importModule(const std::string& name)
{
  script::Module m = moduleManager().getModule(name);

  if (m.isNull())
  {
    std::cerr << "No such module '" << name << "'" << std::endl;
    return;
  }

  if (!m.isLoaded())
    m.load();

  scriptEngine()->currentContext().use(m);
}

void Gonk::display(const script::Value & val)
{
  m_printer->print(val);
}
