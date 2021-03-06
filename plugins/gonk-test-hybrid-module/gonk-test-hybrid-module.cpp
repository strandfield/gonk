// Copyright (C) 2021-2022 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk-test-hybrid-module.h"

#include "gonk/common/binding/function.h"
#include "gonk/functioncreator.h"

#include <script/engine.h>
#include <script/namespace.h>
#include <script/script.h>
#include <script/typesystem.h>

#include <memory>

static int give_me_a_five()
{
  return 5;
}

class Integer
{
public:
  int v;

  Integer(int v_)
    : v(v_)
  {

  }
};

extern "C"
{

GONK_TESTHYBRIDMODULE_API int custom_add(const int& a, const int& b)
{
    return a + b;
}

GONK_TESTHYBRIDMODULE_API int Integer_value(const Integer& self)
{
  return self.v;
}

} // extern "C"

class GonkTestHybridModulePlugin : public gonk::Plugin
{
private:
  std::unique_ptr<gonk::FunctionCreator> m_creator;

public:

  void load(script::Module m) override
  {
    m.engine()->registerType<Integer>("Integer");

    m_creator = std::make_unique<gonk::FunctionCreator>(m.engine(), *library);
    m.asScript().attach(*m_creator);

    m_creator->addCreator<int, const int&, const int&>();

    m_creator->addCtorCreator<Integer, int>();
    m_creator->addCreator<int, const Integer&>();

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
