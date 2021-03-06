// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_CONSTRUCTOR_H
#define GONK_BINDING_CONSTRUCTOR_H

#include "gonk/common/values.h"

#include "gonk/common/binding/constructor_binder.h"

#include <script/class.h>
#include <script/functionbuilder.h>

namespace gonk
{

namespace bind
{

template<typename T>
script::FunctionBuilder default_constructor(script::Class& c)
{
  return script::FunctionBuilder::Constructor(c).setCallback(constructor_binder<T>::default_ctor);
}

template<typename T>
script::FunctionBuilder copy_constructor(script::Class& c)
{
  script::Engine* e = c.engine();
  return script::FunctionBuilder::Constructor(c).setCallback(constructor_binder<T>::copy_ctor)
    .params(script::Type::cref(make_type<T>(e)));
}

template<typename T, typename... Args>
script::FunctionBuilder constructor(script::Class& c)
{
  script::Engine* e = c.engine();
  return script::FunctionBuilder::Constructor(c).setCallback(constructor_binder<T>::template generic_ctor<Args...>)
    .params(make_type<Args>(e)...);
}

template<typename T, typename... Args>
script::FunctionBuilder custom_constructor(script::Class& c, script::NativeFunctionSignature func)
{
  script::Engine* e = c.engine();
  return script::FunctionBuilder::Constructor(c).setCallback(func)
    .params(make_type<Args>(e)...);
}

} // namespace bind

} // namespace gonk

#endif // GONK_BINDING_CONSTRUCTOR_H
