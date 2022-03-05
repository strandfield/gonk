// Copyright (C) 2020-2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_FUNCTION_H
#define GONK_BINDING_FUNCTION_H

#include "gonk/common/values.h"

#include "gonk/common/wrappers/function_wrapper.h"

#include <script/class.h>
#include <script/namespace.h>
#include <script/functionbuilder.h>

namespace gonk
{

namespace bind
{

/* Non-void functions */

template<typename R, R(*F)()>
script::FunctionBuilder free_function(script::Namespace & ns, std::string && name)
{
  auto* e = ns.engine();
  return script::FunctionBuilder(ns, std::move(name)).setCallback(wrapper::function_wrapper_t<R(*)(), F>::wrap)
    .returns(make_type<R>(e));
}

template<typename R, typename A1, R(*F)(A1)>
script::FunctionBuilder free_function(script::Namespace & ns, std::string && name)
{
  auto* e = ns.engine();
  return script::FunctionBuilder(ns, std::move(name)).setCallback(wrapper::function_wrapper_t<R(*)(A1), F>::wrap)
    .returns(make_type<R>(e))
    .params(make_type<A1>(e));
}

template<typename R, typename A1, typename A2, R(*F)(A1, A2)>
script::FunctionBuilder free_function(script::Namespace & ns, std::string && name)
{
  auto* e = ns.engine();
  return script::FunctionBuilder(ns, std::move(name)).setCallback(wrapper::function_wrapper_t<R(*)(A1, A2), F>::wrap)
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e));
}

template<typename R, typename A1, typename A2, typename A3, R(*F)(A1, A2, A3)>
script::FunctionBuilder free_function(script::Namespace & ns, std::string && name)
{
  auto* e = ns.engine();
  return script::FunctionBuilder(ns, std::move(name)).setCallback(wrapper::function_wrapper_t<R(*)(A1, A2, A3), F>::wrap)
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e));
}

template<typename R, typename A1, typename A2, typename A3, typename A4, R(*F)(A1, A2, A3, A4)>
script::FunctionBuilder free_function(script::Namespace & ns, std::string && name)
{
  auto* e = ns.engine();
  return script::FunctionBuilder(ns, std::move(name)).setCallback(wrapper::function_wrapper_t<R(*)(A1, A2, A3, A4), F>::wrap)
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e));
}

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5, R(*F)(A1, A2, A3, A4, A5)>
script::FunctionBuilder free_function(script::Namespace & ns, std::string && name)
{
  auto* e = ns.engine();
  return script::FunctionBuilder(ns, std::move(name)).setCallback(wrapper::function_wrapper_t<R(*)(A1, A2, A3, A4, A5), F>::wrap)
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e));
}


/* void functions */

template<void(*F)()>
script::FunctionBuilder void_function(script::Namespace & ns, std::string && name)
{
  return script::FunctionBuilder(ns, std::move(name)).setCallback(wrapper::void_function_wrapper_t<decltype(F), F>::wrap);
}

template<typename A1, void(*F)(A1)>
script::FunctionBuilder void_function(script::Namespace & ns, std::string && name)
{
  auto* e = ns.engine();
  return script::FunctionBuilder(ns, std::move(name)).setCallback(wrapper::void_function_wrapper_t<decltype(F), F>::wrap)
    .params(make_type<A1>(e));
}

template<typename A1, typename A2, void(*F)(A1, A2)>
script::FunctionBuilder void_function(script::Namespace & ns, std::string && name)
{
  auto* e = ns.engine();
  return script::FunctionBuilder(ns, std::move(name)).setCallback(wrapper::void_function_wrapper_t<decltype(F), F>::wrap)
    .params(make_type<A1>(e), make_type<A2>(e));
}

template<typename A1, typename A2, typename A3, void(*F)(A1, A2, A3)>
script::FunctionBuilder void_function(script::Namespace & ns, std::string && name)
{
  auto* e = ns.engine();
  return script::FunctionBuilder(ns, std::move(name)).setCallback(wrapper::void_function_wrapper_t<decltype(F), F>::wrap)
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e));
}

template<typename A1, typename A2, typename A3, typename A4, void(*F)(A1, A2, A3, A4)>
script::FunctionBuilder void_function(script::Namespace & ns, std::string && name)
{
  auto* e = ns.engine();
  return script::FunctionBuilder(ns, std::move(name)).setCallback(wrapper::void_function_wrapper_t<decltype(F), F>::wrap)
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e));
}

template<typename A1, typename A2, typename A3, typename A4, typename A5, void(*F)(A1, A2, A3, A4, A5)>
script::FunctionBuilder void_function(script::Namespace & ns, std::string && name)
{
  auto* e = ns.engine();
  return script::FunctionBuilder(ns, std::move(name)).setCallback(wrapper::void_function_wrapper_t<decltype(F), F>::wrap)
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e));
}

/* generic function */

template<typename T, typename...Args>
script::Function function(script::Namespace& ns, std::string name, T(*fun)(Args...))
{
  auto impl = std::make_shared<gonk::wrapper::FunctionWrapper<T, Args...>>(script::Symbol{ ns }, std::move(name), fun);
  script::Function ret{ impl };
  ns.addFunction(ret);
  return ret;
}

template<typename T, typename...Args>
script::Function function(script::Class& cla, std::string name, T(*fun)(Args...))
{
  auto impl = std::make_shared<gonk::wrapper::FunctionWrapper<T, Args...>>(script::Symbol{ cla }, std::move(name), fun);
  impl->setMember(true);
  script::Function ret{ impl };
  cla.addFunction(ret);
  return ret;
}

} // namespace bind

} // namespace gonk

#endif // GONK_BINDING_FUNCTION_H
