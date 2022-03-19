// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "std-regex.h"

#include <script/engine.h>
#include <script/namespace.h>
#include <script/typesystem.h>

extern void register_regex_file(script::Namespace ns); // defined in regex.cpp

class StdRegexPlugin : public gonk::Plugin
{
public:

  void load(script::Module m) override
  {
    script::Namespace ns = m.root().getNamespace("std");

    register_regex_file(ns);
  }

  void unload(script::Module m) override
  {

  }
};

gonk::Plugin* gonk_std_regex_module()
{
  return new StdRegexPlugin();
}
