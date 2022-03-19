// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk-test-hybrid-module.h"

#include "gonk/common/binding/function.h"

#include <script/engine.h>
#include <script/namespace.h>
#include <script/typesystem.h>

static int give_me_a_five()
{
  return 5;
}

class GonkTestHybridModulePlugin : public gonk::Plugin
{
public:

  void load(script::Module m) override
  {
    script::Namespace ns = m.root();

    gonk::bind::function(ns, "give_me_a_five", &give_me_a_five);
  }

  void unload(script::Module m) override
  {

  }
};

gonk::Plugin* gonk_test_hybrid_module()
{
  return new GonkTestHybridModulePlugin();
}
