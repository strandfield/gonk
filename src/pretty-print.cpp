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

namespace gonk
{

PrettyPrinter::PrettyPrinter(script::Engine& e)
  : m_engine(e)
{

}

void PrettyPrinter::print(const script::Value& val)
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

  if (val.type().isEnumType())
  {
    script::Enumerator enm = val.toEnumerator();
    std::cout << enm.enumeration().name() << "::" << enm.name() << std::endl;
    return;
  }

  if (val.type().isObjectType())
  {
    script::Function repr_func = reprFunction(val.type());

    if (!repr_func.isNull())
    {
      std::string repr = repr_func.invoke({ val }).toString();
      std::cout << repr << std::endl;
      return;
    }
  }

  std::cout << m_engine.typeSystem()->typeName(val.type()) << "@" << (void*)(val.impl()) << std::endl;
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
