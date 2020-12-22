// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/script-runner.h"

#include "gonk/gonk.h"
#include "gonk/modules.h"

#include <script/class.h>
#include <script/classtemplate.h>
#include <script/engine.h>
#include <script/module.h>
#include <script/namelookup.h>
#include <script/script.h>
#include <script/sourcefile.h>
#include <script/typesystem.h>

#include <fstream>
#include <iostream>

namespace gonk
{

ScriptRunner::ScriptRunner(Gonk& gnk)
  : m_gonk(gnk)
{

}

int ScriptRunner::run()
{
  return runScript();
}

int ScriptRunner::runScript()
{
  if (m_gonk.argv(m_argv_offset) == "--debug")
  {
    m_mode = script::CompileMode::Debug;
    ++m_argv_offset;
  }

  std::string path = m_gonk.argv(m_argv_offset++);

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
  catch (std::runtime_error& err)
  {
    std::cerr << err.what() << std::endl;
    return 1;
  }

  script::Script s = m_gonk.scriptEngine()->newScript(src);

  if (!s.compile(m_mode))
  {
    for (const auto& e : s.messages())
    {
      std::cerr << e.to_string() << std::endl;
    }

    return -1;
  }

  if (m_mode == script::CompileMode::Debug)
  {
    script::Module debugger_module = m_gonk.moduleManager().getModule("gonk.debugger");

    if (!debugger_module.isLoaded())
      debugger_module.load();
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

script::Type ScriptRunner::findVectorStringType() const
{
  script::Module std_vec_module = m_gonk.moduleManager().getModule("std.vector");

  if (std_vec_module.isNull() || !std_vec_module.isLoaded())
    return script::Type::Void;

  auto lookup = script::NameLookup::resolve("std::vector", script::Scope(std_vec_module.root()));

  if(lookup.classTemplateResult().isNull())
    return script::Type::Void;

  script::ClassTemplate vec_template = lookup.classTemplateResult().asClassTemplate();

  script::Class vec_str = vec_template.getInstance({
    script::TemplateArgument(script::Type::String)
    });

  return vec_str.id();
}

bool ScriptRunner::validateMain(const script::Function& f) const
{
  if (f.name() != "main")
    return false;

  if (f.returnType() != script::Type::Int && f.returnType() != script::Type::Void)
  {
    std::cerr << "main() function does not match any known signature" << std::endl;
    return false;
  }

  if (f.prototype().size() == 0)
    return true;

  if (f.prototype().size() != 1)
  {
    std::cerr << "main() function does not match any known signature" << std::endl;
    return false;
  }

  script::Type vec_str = findVectorStringType();

  if (f.parameter(0).baseType() == vec_str)
    return true;
  
  return false;
}

script::Function ScriptRunner::findMain(const script::Script& s) const
{
  for (const auto& f : s.functions())
  {
    if (validateMain(f))
      return f;
  }

  return script::Function();
}

script::Function ScriptRunner::findPushBackFunction(const script::Type& t) const
{
  script::Class c = m_gonk.scriptEngine()->typeSystem()->getClass(t);
  auto lookup = script::NameLookup::resolve("push_back", script::Scope(c));
  return lookup.functions().empty() ? script::Function() : lookup.functions().front();
}

script::Value ScriptRunner::invokeMain(const script::Function& f)
{
  if(f.prototype().size() == 0)
    return f.invoke({});

  script::Engine* e = f.engine();
  script::Type vec_str = findVectorStringType();
  script::Function push_back = findPushBackFunction(vec_str);

  if (push_back.isNull())
  {
    std::cerr << "failed to invoke main(std::vector<String>)" << std::endl;
    return script::Value();
  }

  script::Value args = f.engine()->construct(vec_str, std::vector<script::Value>());

  for (int i(m_argv_offset); i < m_gonk.argc(); ++i)
  {
    push_back.invoke({ args, e->newString(m_gonk.argv()[i]) });
  }

  return f.invoke({args});
}

int ScriptRunner::invokeMain(const script::Script& s)
{
  script::Function func = findMain(s);

  if (func.isNull())
    return 0;

  try
  {
    script::Value ret = invokeMain(func);

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


} // namespace gonk
