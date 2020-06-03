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

namespace script
{

template<typename T>
script::Value make_value(T&& val, script::Engine* e)
{
  if constexpr (!std::is_const<T>::value && !std::is_reference<T>::value && !std::is_pointer<T>::value)
  {
    if constexpr (std::is_enum<T>::value)
      return make_enum(e, make_type<T>(), val);
    else
      return e->construct<T>(std::forward<T>(val));
  }
  else if constexpr (std::is_lvalue_reference<T>::value && !std::is_const<T>::value)
  {
    if constexpr (std::is_enum<typename std::remove_reference<T>::type>::value)
      static_assert(gonk::dependent_false<T>::value, "references to enum not supported by make_value()");
    else
      return e->expose(std::forward<T>(val));
  }
  else if constexpr (std::is_rvalue_reference<T>::value)
  {
    if constexpr (std::is_enum<typename std::remove_reference<T>::type>::value)
      static_assert(gonk::dependent_false<T>::value, "references to enum not supported by make_value()");
    else
      return e->construct<std::remove_reference<T>>(std::forward<T>(val)); // not sure about this one
  }
  else if constexpr (std::is_pointer<T>::value && !std::is_const<typename std::remove_pointer<T>::type>::value)
  {
    if constexpr (std::is_enum<typename std::remove_pointer<T>::type>::value)
      static_assert(gonk::dependent_false<T>::value, "pointer to enum not supported by make_value()");
    else
      return e->construct<gonk::Pointer<std::remove_pointer_t<T>>>(std::forward<T>(val));
  }
  else
  {
    static_assert(gonk::dependent_false<T>::value, "type not supported by make_value()");
  }
}

} // namespace script

namespace script
{

template<typename T>
T value_cast(const script::Value& val)
{
  if constexpr (!std::is_const<T>::value && !std::is_reference<T>::value && !std::is_pointer<T>::value)
  {
    if constexpr (std::is_enum<T>::value)
      return static_cast<T>(val.toEnumerator().value());
    else
      return script::get<T>(val);
  }
  else if constexpr (std::is_lvalue_reference<T>::value)
  {
    if constexpr (std::is_enum<typename std::remove_reference<T>::type>::value)
      static_assert(gonk::dependent_false<T>::value, "references to enum not supported by value_cast()");
    else
      return script::get<typename std::remove_reference<typename std::remove_const<T>::type>::type>(val);
  }
  else if constexpr (std::is_rvalue_reference<T>::value)
  {
    if constexpr (std::is_enum<typename std::remove_reference<T>::type>::value)
      static_assert(gonk::dependent_false<T>::value, "references to enum not supported by value_cast()");
    else
      return std::move(std::get<typename std::remove_reference<T>::type>(val));
  }
  else if constexpr (std::is_pointer<T>::value && !std::is_const<typename std::remove_pointer<T>::type>::value)
  {
    if constexpr (std::is_enum<typename std::remove_pointer<T>::type>::value)
      static_assert(gonk::dependent_false<T>::value, "pointer to enum not supported by value_cast()");
    else
      return script::get<gonk::Pointer<typename std::remove_pointer<T>::type>>(val).ptr;
  }
  else
  {
    static_assert(gonk::dependent_false<T>::value, "type not supported by value_cast()");
  }
}

} // namespace script

#endif // GONK_COMMONS_VALUES_H
