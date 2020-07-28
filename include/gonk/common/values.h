// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_COMMONS_VALUES_H
#define GONK_COMMONS_VALUES_H

#include "gonk/common/dependent-false.h"
#include "gonk/common/enums.h"
#include "gonk/common/pointer.h"
#include "gonk/common/types.h"

#include <script/engine.h>
#include <script/value.h>

#include <string>

namespace gonk
{

template<typename T>
script::Value make_value(T&& val, script::Engine* e)
{
  if constexpr (!std::is_const<T>::value && !std::is_reference<T>::value && !std::is_pointer<T>::value)
  {
    return e->construct<T>(std::forward<T>(val));
  }
  else if constexpr (std::is_lvalue_reference<T>::value && !std::is_const<T>::value)
  {
    return e->expose(std::forward<T>(val));
  }
  else if constexpr (std::is_rvalue_reference<T>::value)
  {
    return e->construct<std::remove_reference<T>>(std::forward<T>(val)); // not sure about this one
  }
  else if constexpr (std::is_pointer<T>::value && !std::is_const<typename std::remove_pointer<T>::type>::value)
  {
    return e->construct<gonk::Pointer<std::remove_pointer_t<T>>>(std::forward<T>(val));
  }
  else if constexpr (std::is_const<T>::value && !std::is_reference<T>::value && !std::is_pointer<T>::value) // const U
  {
    return e->construct<typename std::remove_const<T>::type>(std::forward<T>(val));
  }
  else
  {
    static_assert(gonk::dependent_false<T>::value, "type not supported by make_value()");
  }
}

} // namespace gonk

namespace gonk
{

template<typename T>
T value_cast(const script::Value& val)
{
  if constexpr (!std::is_const<T>::value && !std::is_reference<T>::value && !std::is_pointer<T>::value)
  {
    return script::get<T>(val);
  }
  else if constexpr (std::is_lvalue_reference<T>::value)
  {
    return script::get<typename std::remove_reference<typename std::remove_const<T>::type>::type>(val);
  }
  else if constexpr (std::is_rvalue_reference<T>::value)
  {
    return std::move(script::get<typename std::remove_reference<T>::type>(val));
  }
  else if constexpr (std::is_pointer<T>::value && !std::is_const<typename std::remove_pointer<T>::type>::value)
  {
    return script::get<gonk::Pointer<typename std::remove_pointer<T>::type>>(val).ptr;
  }
  else
  {
    static_assert(gonk::dependent_false<T>::value, "type not supported by value_cast()");
  }
}

} // namespace gonk

#endif // GONK_COMMONS_VALUES_H
