// Copyright (C) 2020-2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_CHAINABLE_MEMFN_H
#define GONK_BINDING_CHAINABLE_MEMFN_H

#include "gonk/common/values.h"

#include "gonk/common/wrappers/chainable_member_wrapper.h"

#include <script/class.h>
#include <script/functionbuilder.h>

namespace gonk
{

namespace bind
{

/* Chainable member functions; i.e. function returning a reference to 'this' */

template<typename T, typename A1, T&(T::*F)(A1)>
script::FunctionBuilder chainable_memfn(script::Class& cla, std::string && name)
{
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::chainable_member_wrapper_t<decltype(F), F>::wrap)
    .returns(script::Type::ref(make_type<T>()))
    .params(make_type<A1>());
}

template<typename T, typename A1, typename A2, T&(T::*F)(A1, A2)>
script::FunctionBuilder chainable_memfn(script::Class& cla, std::string && name)
{
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::chainable_member_wrapper_t<decltype(F), F>::wrap)
    .returns(script::Type::ref(make_type<T>()))
    .params(make_type<A1>(), make_type<A2>());
}

template<typename T, typename A1, typename A2, typename A3, T&(T::*F)(A1, A2, A3)>
script::FunctionBuilder chainable_memfn(script::Class& cla, std::string && name)
{
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::chainable_member_wrapper_t<decltype(F), F>::wrap)
    .returns(script::Type::ref(make_type<T>()))
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>());
}

template<typename T, typename A1, typename A2, typename A3, typename A4, T&(T::*F)(A1, A2, A3, A4)>
script::FunctionBuilder chainable_memfn(script::Class& cla, std::string && name)
{
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::chainable_member_wrapper_t<decltype(F), F>::wrap)
    .returns(script::Type::ref(make_type<T>()))
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>(), make_type<A4>());
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, T&(T::*F)(A1, A2, A3, A4, A5)>
script::FunctionBuilder chainable_memfn(script::Class& cla, std::string && name)
{
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::chainable_member_wrapper_t<decltype(F), F>::wrap)
    .returns(script::Type::ref(make_type<T>()))
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>(), make_type<A4>(), make_type<A5>());
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, T&(T::*F)(A1, A2, A3, A4, A5, A6)>
script::FunctionBuilder chainable_memfn(script::Class& cla, std::string && name)
{
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::chainable_member_wrapper_t<decltype(F), F>::wrap)
    .returns(script::Type::ref(make_type<T>()))
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>(), make_type<A4>(), make_type<A5>(), make_type<A6>());
}

} // namespace bind

} // namespace gonk

#endif // GONK_BINDING_CHAINABLE_MEMFN_H
