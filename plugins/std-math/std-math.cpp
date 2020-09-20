// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "std-math.h"

#include <script/engine.h>
#include <script/namespace.h>
#include <script/typesystem.h>

extern void register_math_file(script::Namespace ns); // defined in math.cpp

namespace gonk
{

namespace std_math
{

int class_type_id_offset_value = 0;
int enum_type_id_offset_value = 0;

int class_type_id_offset()
{
  return class_type_id_offset_value;
}

int enum_type_id_offset()
{
  return enum_type_id_offset_value;
}

} // namespace std_math

} // namespace gonk

class StdMathPlugin : public gonk::Plugin
{
public:

  void load(script::Module m) override
  {
    script::Engine* e = m.engine();

    gonk::std_math::class_type_id_offset_value = static_cast<int>(e->typeSystem()->reserve(script::Type::ObjectFlag, 1));

    script::Namespace ns = m.root().getNamespace("std");

    register_math_file(ns);
  }

  void unload(script::Module m) override
  {

  }
};

gonk::Plugin* gonk_std_math_module()
{
  return new StdMathPlugin();
}
