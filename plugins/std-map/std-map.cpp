// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "std-map.h"

#include <script/engine.h>
#include <script/namespace.h>
#include <script/typesystem.h>

extern void register_map_template(script::Namespace ns); // defined in map.cpp

class StdMapPlugin : public gonk::Plugin
{
public:

  void load(script::Module m) override
  {
    script::Engine* e = m.engine();

    script::Namespace ns = m.root().getNamespace("std");

    register_map_template(ns);
  }

  void unload(script::Module m) override
  {

  }
};

gonk::Plugin* gonk_std_map_module()
{
  return new StdMapPlugin();
}
