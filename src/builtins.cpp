// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/builtins.h"

#include <script/class.h>
#include <script/classbuilder.h>
#include <script/context.h>
#include <script/functionbuilder.h>
#include <script/module.h>
#include <script/namespace.h>
#include <script/script.h>
#include <script/typesystem.h>

#include <script/interpreter/executioncontext.h>

#include <iostream>

namespace gonk
{

extern void register_pointer_template(script::Namespace ns); // defined in pointer-template.cpp

namespace callbacks
{

script::Value print_int(script::FunctionCall* c)
{
  std::cout << c->arg(0).toInt() << std::endl;
  return script::Value::Void;
}

script::Value print_bool(script::FunctionCall* c)
{
  std::cout << c->arg(0).toBool() << std::endl;
  return script::Value::Void;
}

script::Value print_double(script::FunctionCall* c)
{
  std::cout << c->arg(0).toDouble() << std::endl;
  return script::Value::Void;
}

script::Value print_string(script::FunctionCall* c)
{
  std::cout << c->arg(0).toString() << std::endl;
  return script::Value::Void;
}

script::Value gnk_assert(script::FunctionCall* c)
{
  if(!c->arg(0).toBool())
    throw script::RuntimeError{ "Assertion failure!" };
  return script::Value::Void;
}

script::Value raise(script::FunctionCall* c)
{
  throw script::RuntimeError{ script::get<std::string>(c->arg(0)) };
}

} // namespace callbacks

void register_builtins(script::Namespace& ns)
{
  ns.newFunction("print", callbacks::print_int)
    .params(script::Type::Int).create();

  ns.newFunction("print", callbacks::print_bool)
    .params(script::Type::Boolean).create();

  ns.newFunction("print", callbacks::print_double)
    .params(script::Type::Double).create();

  ns.newFunction("print", callbacks::print_string)
    .params(script::Type::cref(script::Type::String)).create();

  ns.newFunction("assert", callbacks::gnk_assert)
    .params(script::Type::Boolean).create();

  ns.newFunction("raise", callbacks::raise)
    .params(script::Type::cref(script::Type::String)).create();

  gonk::register_pointer_template(ns);
}

} // namespace gonk
