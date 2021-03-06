// Copyright (C) 2020-2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_MEMBERFUNCTION_H
#define GONK_BINDING_MEMBERFUNCTION_H

#include "gonk/common/values.h"

#include "gonk/common/wrappers/function_wrapper.h"
#include "gonk/common/wrappers/member_wrapper.h"

#include <script/class.h>
#include <script/functionbuilder.h>

namespace gonk
{

namespace bind
{

/* Non-void non-static non-const member functions */

template<typename T, typename R, R(T::*F)()>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .returns(make_type<R>(e));
}

template<typename T, typename R, typename A1, R(T::*F)(A1)>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .returns(make_type<R>(e))
    .params(make_type<A1>(e));
}

template<typename T, typename R, typename A1, typename A2, R(T::*F)(A1, A2)>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, R(T::*F)(A1, A2, A3)>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, R(T::*F)(A1, A2, A3, A4)>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, R(T::*F)(A1, A2, A3, A4, A5)>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, R(T::*F)(A1, A2, A3, A4, A5, A6)>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e), make_type<A6>(e));
}

/* Non-void non-static const member functions */

template<typename T, typename R, R(T::*F)()const>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<R>(e));
}

template<typename T, typename R, typename A1, R(T::*F)(A1)const>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e));
}

template<typename T, typename R, typename A1, typename A2, R(T::*F)(A1, A2)const>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, R(T::*F)(A1, A2, A3)const>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, R(T::*F)(A1, A2, A3, A4)const>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, R(T::*F)(A1, A2, A3, A4, A5)const>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, R(T:: * F)(A1, A2, A3, A4, A5, A6)const>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e), make_type<A6>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, R(T:: * F)(A1, A2, A3, A4, A5, A6, A7)const>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e), make_type<A6>(e), make_type<A7>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, R(T::* F)(A1, A2, A3, A4, A5, A6, A7, A8)const>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e), make_type<A6>(e), make_type<A7>(e), make_type<A8>(e));
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, R(T::* F)(A1, A2, A3, A4, A5, A6, A7, A8, A9)const>
script::FunctionBuilder member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e), make_type<A6>(e), make_type<A7>(e), make_type<A8>(e), make_type<A9>(e));
}


/* void functions */

template<typename T, void(T::*F)()>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::void_member_wrapper_t<decltype(F), F>::wrap);
}

template<typename T, typename A1, void(T::*F)(A1)>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::void_member_wrapper_t<decltype(F), F>::wrap)
    .params(make_type<A1>(e));
}

template<typename T, typename A1, typename A2, void(T::*F)(A1, A2)>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::void_member_wrapper_t<decltype(F), F>::wrap)
    .params(make_type<A1>(e), make_type<A2>(e));
}

template<typename T, typename A1, typename A2, typename A3, void(T::*F)(A1, A2, A3)>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::void_member_wrapper_t<decltype(F), F>::wrap)
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e));
}

template<typename T, typename A1, typename A2, typename A3, typename A4, void(T::*F)(A1, A2, A3, A4)>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::void_member_wrapper_t<decltype(F), F>::wrap)
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e));
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, void(T::*F)(A1, A2, A3, A4, A5)>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::void_member_wrapper_t<decltype(F), F>::wrap)
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e));
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, void(T::*F)(A1, A2, A3, A4, A5, A6)>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::void_member_wrapper_t<decltype(F), F>::wrap)
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e), make_type<A6>(e));
}

/* void const functions */

template<typename T, void(T::*F)()const>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst();
}

template<typename T, typename A1, void(T::*F)(A1)const>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .params(make_type<A1>(e));
}

template<typename T, typename A1, typename A2, void(T::*F)(A1, A2)const>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .params(make_type<A1>(e), make_type<A2>(e));
}

template<typename T, typename A1, typename A2, typename A3, void(T::*F)(A1, A2, A3)const>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e));
}

template<typename T, typename A1, typename A2, typename A3, typename A4, void(T::*F)(A1, A2, A3, A4)const>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e));
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, void(T::*F)(A1, A2, A3, A4, A5)const>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e));
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, void(T::*F)(A1, A2, A3, A4, A5, A6)const>
script::FunctionBuilder void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e), make_type<A6>(e));
}

template<typename T, void(T::*F)()const>
script::FunctionBuilder const_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst();
}

template<typename T, typename A1, void(T::*F)(A1)const>
script::FunctionBuilder const_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .params(make_type<A1>(e));
}

template<typename T, typename A1, typename A2, void(T::*F)(A1, A2)const>
script::FunctionBuilder const_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .params(make_type<A1>(e), make_type<A2>(e));
}

template<typename T, typename A1, typename A2, typename A3, void(T::*F)(A1, A2, A3)const>
script::FunctionBuilder const_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e));
}

template<typename T, typename A1, typename A2, typename A3, typename A4, void(T::*F)(A1, A2, A3, A4)const>
script::FunctionBuilder const_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e));
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, void(T::*F)(A1, A2, A3, A4, A5)const>
script::FunctionBuilder const_void_member_function(script::Class& cla, std::string name)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Fun(cla, std::move(name)).setCallback(wrapper::const_void_member_wrapper_t<decltype(F), F>::wrap)
    .setConst()
    .params(make_type<A1>(e), make_type<A2>(e), make_type<A3>(e), make_type<A4>(e), make_type<A5>(e));
}

/* generic member function */

template<typename R, typename T, typename...Args>
script::Function method(script::Class& cla, std::string name, R(T::*memfun)(Args...))
{
  auto impl = std::make_shared<gonk::wrapper::MethodWrapper<R, T, Args...>>(cla, std::move(name), memfun);
  script::Function ret{ impl };
  cla.addMethod(ret);
  return ret;
}

template<typename R, typename T, typename...Args>
script::Function method(script::Class& cla, std::string name, R(T::*memfun)(Args...)const)
{
  auto impl = std::make_shared<gonk::wrapper::ConstMethodWrapper<R, T, Args...>>(cla, std::move(name), memfun);
  script::Function ret{ impl };
  cla.addMethod(ret);
  return ret;
}

template<typename R, typename T, typename...Args>
script::Function method(script::Class& cla, std::string name, R(*fn)(T&, Args...))
{
  auto impl = std::make_shared<gonk::wrapper::FunctionWrapper<R, T&, Args...>>(script::Symbol(cla), std::move(name), fn);
  impl->setMember(false);
  script::Function ret{ impl };
  cla.addMethod(ret);
  return ret;
}

template<typename R, typename T, typename...Args>
script::Function method(script::Class& cla, std::string name, R(*fn)(const T&, Args...))
{
  auto impl = std::make_shared<gonk::wrapper::FunctionWrapper<R, const T&, Args...>>(script::Symbol(cla), std::move(name), fn);
  impl->setMember(false);
  script::Function ret{ impl };
  cla.addMethod(ret);
  return ret;
}

} // namespace bind

} // namespace gonk

#endif // GONK_BINDING_MEMBERFUNCTION_H
