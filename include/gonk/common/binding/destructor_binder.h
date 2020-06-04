// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_DESTRUCTOR_BINDER_H
#define GONK_BINDING_DESTRUCTOR_BINDER_H

#include "gonk/common/values.h"

#include <script/interpreter/executioncontext.h>

namespace gonk
{

namespace bind
{

template<typename T>
struct destructor_binder
{
  static script::Value destructor(script::FunctionCall *c) 
  {
    c->thisObject().destroy<T>();
    return script::Value::Void;
  }
};

} // namespace bind

} // namespace gonk

#endif // GONK_BINDING_DESTRUCTOR_BINDER_H
