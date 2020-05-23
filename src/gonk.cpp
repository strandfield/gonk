// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/gonk.h"

#include "gonk/modules.h"

#include <script/class.h>
#include <script/classbuilder.h>
#include <script/context.h>
#include <script/functionbuilder.h>
#include <script/module.h>
#include <script/namespace.h>
#include <script/script.h>
#include <script/typesystem.h>

#include <script/interpreter/executioncontext.h>

#include <QCoreApplication>
#include <QDebug>
#include <QtGlobal>

#include <fstream>
#include <iostream>

Gonk* Gonk::m_instance = nullptr;

namespace callbacks
{

script::Value print_int(script::FunctionCall *c)
{
  std::cout << c->arg(0).toInt() << std::endl;
  return script::Value::Void;
}

script::Value print_bool(script::FunctionCall *c)
{
  std::cout << c->arg(0).toBool() << std::endl;
  return script::Value::Void;
}

script::Value print_double(script::FunctionCall *c)
{
  std::cout << c->arg(0).toDouble() << std::endl;
  return script::Value::Void;
}

script::Value print_string(script::FunctionCall *c)
{
  std::cout << c->arg(0).toString() << std::endl;
  return script::Value::Void;
}

} // namespace callbacks

Gonk::Gonk(int & argc, char **argv)
  : m_argc(argc),
    m_argv(argv)
{
  m_instance = this;

  m_engine.setup();

  m_engine.rootNamespace().newFunction("print", callbacks::print_int)
    .params(script::Type::Int).create();

  m_engine.rootNamespace().newFunction("print", callbacks::print_bool)
    .params(script::Type::Boolean).create();

  m_engine.rootNamespace().newFunction("print", callbacks::print_double)
    .params(script::Type::Double).create();

  m_engine.rootNamespace().newFunction("print", callbacks::print_string)
    .params(script::Type::cref(script::Type::String)).create();

  m_module_manager.reset(new gonk::ModuleManager(&m_engine));
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

  m_module_manager->addImportPath("./modules");
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
    scriptEngine()->manage(v);
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
    std::cerr << err.what() << std::endl;
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

  script::Value ret = func.invoke({});

  if (ret.isInt())
    return ret.toInt();

  return 0;
}

void Gonk::display(const script::Value & val)
{
  switch (val.type().baseType().data())
  {
  case script::Type::Void:
    return;
  case script::Type::Boolean:
    std::cout << (val.toBool() ? "true" : "false") << std::endl;
    return;
  case script::Type::Char:
    std::cout << val.toChar() << std::endl;
    return;
  case script::Type::Int:
    std::cout << val.toInt() << std::endl;
    return;
  case script::Type::Float:
    std::cout << val.toFloat() << std::endl;
    return;
  case script::Type::Double:
    std::cout << val.toDouble() << std::endl;
    return;
  case script::Type::String:
    std::cout << val.toString() << std::endl;
    return;
  default:
    break;
  }

  /// TODO: better print
  std::cout << m_engine.typeSystem()->typeName(val.type()) << "@" << (void*)(val.impl()) << std::endl;
}
