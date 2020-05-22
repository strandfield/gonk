// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_CONSTRUCTOR_H
#define GONK_BINDING_CONSTRUCTOR_H

#include "gonk/common/values.h"

#include "gonk/common/binding/constructor_binder.h"

#include <script/class.h>
#include <script/constructorbuilder.h>

namespace script
{

namespace bind
{

template<typename T>
script::ConstructorBuilder default_constructor(Class & c)
{
  return c.newConstructor(constructor_binder<T, typename details::tag_resolver<T>::tag_type>::default_ctor);
}

template<typename T>
script::ConstructorBuilder copy_constructor(Class & c)
{
  return c.newConstructor(constructor_binder<T, typename details::tag_resolver<T>::tag_type>::copy_ctor)
    .params(script::Type::cref(make_type<T>()));
}

template<typename T, typename... Args>
script::ConstructorBuilder constructor(Class & c)
{
  return c.newConstructor(constructor_binder<T, typename details::tag_resolver<T>::tag_type>::template generic_ctor<Args...>)
    .params(make_type<Args>()...);
}

} // namespace bind

} // namespace script

#endif // GONK_BINDING_CONSTRUCTOR_H
