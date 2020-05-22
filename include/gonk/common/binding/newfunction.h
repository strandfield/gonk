// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_NEW_FUNCTION_H
#define GONK_BINDING_NEW_FUNCTION_H

#include "gonk/common/wrappers/newfunction_wrapper.h"

#include <script/namespace.h>
#include <script/function.h>
#include <script/functionbuilder.h>

namespace script
{

namespace bind
{

template<typename T>
script::Function new_function(script::Namespace & ns, std::string && name)
{
  return ns.newFunction(std::move(name), wrapper::qclass_newfunction_wrapper_t<T>::wrap)
    .returns(make_type<T&>())
    .get();
}


template<typename T, typename A1, typename... Rest>
script::Function new_function(script::Namespace & ns, std::string && name)
{
  return ns.newFunction(std::move(name), wrapper::qclass_newfunction_wrapper_t<T, A1, Rest...>::wrap)
    .returns(make_type<T&>())
    .params(make_type<A1>(), make_type<Rest>()...)
    .get();
}

} // namespace bind

} // namespace script

#endif // GONK_BINDING_NEW_FUNCTION_H
