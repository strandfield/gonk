// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/pretty-print.h"

#include <script/class.h>
#include <script/engine.h>
#include <script/enumerator.h>
#include <script/namelookup.h>
#include <script/namespace.h>
#include <script/typesystem.h>

#include <iostream>
#include <sstream>

namespace gonk
{

PrettyPrinter::PrettyPrinter(script::Engine& e)
  : m_engine(e)
{

}

std::string PrettyPrinter::repr(const script::Value& val)
{
  switch (val.type().baseType().data())
  {
  case script::Type::Void:
    return "";
  case script::Type::Boolean:
    return (val.toBool() ? "true" : "false");
  case script::Type::Char:
    return std::string(1, val.toChar());
  case script::Type::Int:
    return std::to_string(val.toInt());
  case script::Type::Float:
    return std::to_string(val.toFloat());
  case script::Type::Double:
    return std::to_string(val.toDouble());
  case script::Type::String:
    return val.toString();
  default:
    break;
  }

  if (val.type().isEnumType())
  {
    script::Enumerator enm = val.toEnumerator();
    return enm.enumeration().name() + "::" + enm.name();
  }

  if (val.type().isObjectType())
  {
    script::Function repr_func = reprFunction(val.type());

    if (!repr_func.isNull())
    {
      return repr_func.invoke({ val }).toString();
    }
  }

  std::stringstream ss;
  ss << m_engine.typeSystem()->typeName(val.type());
  ss << "@";
  ss << (void*)(val.impl());
  return ss.str();
}

void PrettyPrinter::print(const script::Value& val)
{
  std::cout << repr(val) << std::endl;
}

script::Function PrettyPrinter::reprFunction(const script::Type& t)
{
  int key = t.baseType().data();
  auto it = m_repr_functions.find(key);

  if (it != m_repr_functions.end())
    return it->second;

  script::Function repr_func = findReprFunction(t);
  m_repr_functions[key] = repr_func;
  return repr_func;
}

static bool validate_repr_function(const script::Function& f, const script::Type& t)
{
  if (f.returnType() != script::Type::String)
    return false;

  if (f.prototype().size() != 1)
    return false;

  if (f.parameter(0) != script::Type::cref(t))
    return false;

  return true;
}

script::Function PrettyPrinter::findReprFunction(const script::Type& t) const
{
  script::Class c = m_engine.typeSystem()->getClass(t);
  assert(!c.isNull());

  script::NameLookup lookup = script::NameLookup::resolve("__gonk_repr__", script::Scope(c));

  for (auto f : lookup.functions())
  {
    if (validate_repr_function(f, t))
      return f;
  }

  lookup = script::NameLookup::resolve("__gonk_repr__", script::Scope(c.enclosingNamespace()));

  for (auto f : lookup.functions())
  {
    if (validate_repr_function(f, t))
      return f;
  }

  return {};
}

} // namespace gonk
