// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/gonk.h"

#include "gonk/builtins.h"
#include "gonk/modules.h"
#include "gonk/pretty-print.h"

#include <script/class.h>
#include <script/context.h>
#include <script/enumerator.h>
#include <script/function.h>
#include <script/module.h>
#include <script/namespace.h>
#include <script/script.h>
#include <script/typesystem.h>

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
  : m_argc(argc),
    m_argv(argv)
{
  m_instance = this;

  setupEngine();

  m_module_manager = std::make_unique<gonk::ModuleManager>(&m_engine);
  m_printer = std::make_unique<gonk::PrettyPrinter>(m_engine);
}

Gonk::~Gonk()
{

}

int Gonk::argc() const
{
  return m_argc;
}

char** Gonk::argv() const
{
  return m_argv;
}

std::string Gonk::argv(size_t index) const
{
  return std::string(argv()[index]);
}

int Gonk::exec()
{
  if (argc() <= 1)
  {
    std::cout << "Gonk!" << std::endl;
    return 0;
  }

  m_module_manager->addImportPath(executable_dir() + "/modules");
  m_module_manager->fetchModules();

  if (argv(1) == "--interactive")
  {
    return interactiveSession();
  }
  else if (argv(1) == "--list-modules")
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
  std::string path = argv(1);

  {
    std::ifstream file{ path };
    if (!file.is_open())
      path += ".gnk";
  }

  script::SourceFile src{ path };

  try
  {
    src.load();
  }
  catch (std::runtime_error & err)
  {
    std::cerr << err.what() << std::endl;
    return 1;
  }

  script::Script s = scriptEngine()->newScript(src);

  if (!s.compile())
  {
    for (const auto& e : s.messages())
    {
      std::cerr << e.to_string() << std::endl;
    }

    return -1;
  }

  try
  {
    s.run();
  }
  catch (script::RuntimeError& err)
  {
    std::cerr << err.message << std::endl;
    return 1;
  }
  catch (std::exception& ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }

  return invokeMain(s);
}

int Gonk::invokeMain(const script::Script& s)
{
  script::Function func = [&]() {

    for (const auto& f : s.functions())
    {
      if (f.name() != "main")
        continue;

      if (f.returnType() != script::Type::Int && f.returnType() != script::Type::Void)
      {
        std::cerr << "main() function does not match any known signature" << std::endl;
        return script::Function();
      }

      if (f.prototype().size() != 0)
      {
        std::cerr << "main() function does not match any known signature" << std::endl;
        return script::Function();
      }

      return f;
    }

    return script::Function();
  }();

  if (func.isNull())
    return 0;

  try
  {
    script::Value ret = func.invoke({});

    if (ret.isInt())
      return ret.toInt();
  }
  catch (script::RuntimeError& err)
  {
    std::cerr << err.message << std::endl;
    return 1;
  }
  catch (std::exception& ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }

  return 0;
}

void Gonk::importModule(const std::string& name)
{
  script::Module m;

  auto it = std::find(name.begin(), name.end(), '.');

  if (it == name.end())
  {
    m = scriptEngine()->getModule(name);
  }
  else
  {
    m = scriptEngine()->getModule(std::string(name.begin(), it));

    while (it != name.end() && !m.isNull())
    {
      ++it;
      auto other_it = std::find(it, name.end(), '.');
      m = m.getSubModule(std::string(it, other_it));
      it = other_it;
    }
  }

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
