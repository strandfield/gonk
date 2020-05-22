// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_GETTER_H
#define GONK_BINDING_GETTER_H

#include "gonk/common/values.h"

#include "gonk/common/wrappers/reference_member_wrapper.h"

#include <script/class.h>
#include <script/functionbuilder.h>

namespace script
{

namespace bind
{

template<typename T, typename R, R(T::*F)()>
script::FunctionBuilder non_const_getter(Class & cla, std::string && name)
{
  static_assert(std::is_reference<R>::value, "Return type must be a reference");

  return cla.newMethod(std::move(name), wrapper::ref_member_wrapper_t<decltype(F), F>::wrap)
    .returns(make_type<Proxy<typename std::remove_reference<R>::type>>());
}

template<typename T, typename R, typename A1, R(T::*F)(A1)>
script::FunctionBuilder non_const_getter(Class & cla, std::string && name)
{
  static_assert(std::is_reference<R>::value, "Return type must be a reference");

  return cla.newMethod(std::move(name), wrapper::ref_member_wrapper_t<decltype(F), F>::wrap)
    .returns(make_type<Proxy<typename std::remove_reference<R>::type>>())
    .params(make_type<A1>());
}

template<typename T, typename R, typename A1, typename A2, R(T::*F)(A1, A2)>
script::FunctionBuilder non_const_getter(Class & cla, std::string && name)
{
  static_assert(std::is_reference<R>::value, "Return type must be a reference");

  return cla.newMethod(std::move(name), wrapper::ref_member_wrapper_t<decltype(F), F>::wrap)
    .returns(make_type<Proxy<typename std::remove_reference<R>::type>>())
    .params(make_type<A1>(), make_type<A2>());
}

/* Const-getters returning a non-const reference */

template<typename T, typename R, R(T::*F)() const>
script::FunctionBuilder getter(Class & cla, std::string && name)
{
  static_assert(std::is_reference<R>::value, "Return type must be a reference");

  return cla.newMethod(std::move(name), wrapper::ref_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<Proxy<typename std::remove_reference<R>::type>>());
}

template<typename T, typename R, typename A1, R(T::*F)(A1) const>
script::FunctionBuilder getter(Class & cla, std::string && name)
{
  static_assert(std::is_reference<R>::value, "Return type must be a reference");

  return cla.newMethod(std::move(name), wrapper::ref_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<Proxy<typename std::remove_reference<R>::type>>())
    .params(make_type<A1>());
}

template<typename T, typename R, typename A1, typename A2, R(T::*F)(A1, A2) const>
script::FunctionBuilder getter(Class & cla, std::string && name)
{
  static_assert(std::is_reference<R>::value, "Return type must be a reference");

  return cla.newMethod(std::move(name), wrapper::ref_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<Proxy<typename std::remove_reference<R>::type>>())
    .params(make_type<A1>(), make_type<A2>());
}

} // namespace bind

} // namespace script

#endif // GONK_BINDING_GETTER_H
