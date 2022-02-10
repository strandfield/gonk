// Copyright (C) 2020-2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_STATICMEMFUN_H
#define GONK_BINDING_STATICMEMFUN_H

#include "gonk/common/wrappers/function_wrapper.h"

#include <script/namespace.h>
#include <script/functionbuilder.h>

namespace gonk
{

namespace bind
{

/* Non-void static member functions */

template<typename T, typename R, R(*F)(), typename FT = decltype(F)>
script::FunctionBuilder static_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .returns(make_type<R>(e));
}

template<typename T, typename R, typename A1, R(*F)(A1), typename FT = decltype(F)>
script::FunctionBuilder static_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e));
}

template<typename T, typename R, typename A1, typename A2, R(*F)(A1, A2), typename FT = decltype(F)>
script::FunctionBuilder static_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, R(*F)(A1, A2, A3), typename FT = decltype(F)>
script::FunctionBuilder static_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, R(*F)(A1, A2, A3, A4), typename FT = decltype(F)>
script::FunctionBuilder static_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, R(*F)(A1, A2, A3, A4, A5), typename FT = decltype(F)>
script::FunctionBuilder static_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, R(*F)(A1, A2, A3, A4, A5, A6), typename FT = decltype(F)>
script::FunctionBuilder static_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e), make_type<A6>(e));
}


/* void functions */

template<typename T, void(*F)(), typename FT = decltype(F)>
script::FunctionBuilder static_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::void_function_wrapper_t<FT, F>::wrap)
    .setStatic();
}

template<typename T, typename A1, void(*F)(A1), typename FT = decltype(F)>
script::FunctionBuilder static_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::void_function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .params(make_type<A1>(e));
}

template<typename T, typename A1, typename A2, void(*F)(A1, A2), typename FT = decltype(F)>
script::FunctionBuilder static_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::void_function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .params(make_type<A1>(e), make_type<A2>(e));
}

template<typename T, typename A1, typename A2, typename A3, void(*F)(A1, A2, A3), typename FT = decltype(F)>
script::FunctionBuilder static_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::void_function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e));
}

template<typename T, typename A1, typename A2, typename A3, typename A4, void(*F)(A1, A2, A3, A4), typename FT = decltype(F)>
script::FunctionBuilder static_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::void_function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e));
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, void(*F)(A1, A2, A3, A4, A5), typename FT = decltype(F)>
script::FunctionBuilder static_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::void_function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e));
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, void(*F)(A1, A2, A3, A4, A5, A6), typename FT = decltype(F)>
script::FunctionBuilder static_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder(cla, std::move(name)).setCallback(wrapper::void_function_wrapper_t<FT, F>::wrap)
    .setStatic()
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e), make_type<A6>(e));
}

} // namespace bind

} // namespace gonk

#endif // GONK_BINDING_STATICMEMFUN_H
