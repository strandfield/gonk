// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_FN_AS_MEMFN_H
#define GONK_BINDING_FN_AS_MEMFN_H

#include "gonk/common/values.h"

#include "gonk/common/wrappers/function_wrapper.h"

#include <script/class.h>
#include <script/functionbuilder.h>

namespace gonk
{

namespace bind
{

/* Non-void non-static non-const member functions */

template<typename T, typename R, R(*F)(T &)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(T&), F>::wrap)
    .returns(make_type<R>());
}

template<typename T, typename R, typename A1, R(*F)(T &, A1)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(T&, A1), F>::wrap)
    .returns(make_type<R>())
    .params(make_type<A1>());
}

template<typename T, typename R, typename A1, typename A2, R(*F)(T &, A1, A2)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(T&, A1, A2), F>::wrap)
    .returns(make_type<R>())
    .params(make_type<A1>(), make_type<A2>());
}

template<typename T, typename R, typename A1, typename A2, typename A3, R(*F)(T &, A1, A2, A3)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(T&, A1, A2, A3), F>::wrap)
    .returns(make_type<R>())
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>());
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, R(*F)(T &, A1, A2, A3, A4)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(T&, A1, A2, A3, A4), F>::wrap)
    .returns(make_type<R>())
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>(), make_type<A4>());
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, R(*F)(T &, A1, A2, A3, A4, A5)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(T&, A1, A2, A3, A4, A5), F>::wrap)
    .returns(make_type<R>())
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>(), make_type<A4>(), make_type<A5>());
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, R(*F)(T &, A1, A2, A3, A4, A5, A6)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(T&, A1, A2, A3, A4, A5, A6), F>::wrap)
    .returns(make_type<R>())
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>(), make_type<A4>(), make_type<A5>(), make_type<A6>());
}


/* Non-void non-static const member functions */

template<typename T, typename R, R(*F)(const T &)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(const T&), F>::wrap)
    .setConst()
    .returns(make_type<R>());
}

template<typename T, typename R, typename A1, R(*F)(const T &, A1)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(const T&, A1), F>::wrap)
    .setConst()
    .returns(make_type<R>())
    .params(make_type<A1>());
}

template<typename T, typename R, typename A1, typename A2, R(*F)(const T &, A1, A2)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(const T&, A1, A2), F>::wrap)
    .setConst()
    .returns(make_type<R>())
    .params(make_type<A1>(), make_type<A2>());
}

template<typename T, typename R, typename A1, typename A2, typename A3, R(*F)(const T &, A1, A2, A3)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(const T&, A1, A2, A3), F>::wrap)
    .setConst()
    .returns(make_type<R>())
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>());
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, R(*F)(const T &, A1, A2, A3, A4)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(const T&, A1, A2, A3, A4), F>::wrap)
    .setConst()
    .returns(make_type<R>())
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>(), make_type<A4>());
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, R(*F)(const T &, A1, A2, A3, A4, A5)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(const T&, A1, A2, A3, A4, A5), F>::wrap)
    .setConst()
    .returns(make_type<R>())
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>(), make_type<A4>(), make_type<A5>());
}

template<typename T, typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, R(*F)(const T &, A1, A2, A3, A4, A5, A6)>
script::FunctionBuilder fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::function_wrapper_t<R(*)(const T&, A1, A2, A3, A4, A5, A6), F>::wrap)
    .setConst()
    .returns(make_type<R>())
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>(), make_type<A4>(), make_type<A5>(), make_type<A6>());
}


/* void non-static non-const member functions */

template<typename T, void(*F)(T &)>
script::FunctionBuilder void_fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::void_function_wrapper_t<void(*)(T&), F>::wrap);
}

template<typename T, typename A1, void(*F)(T &, A1)>
script::FunctionBuilder void_fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::void_function_wrapper_t<void(*)(T&, A1), F>::wrap)
    .params(make_type<A1>());
}

template<typename T, typename A1, typename A2, void(*F)(T &, A1, A2)>
script::FunctionBuilder void_fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::void_function_wrapper_t<void(*)(T&, A1, A2), F>::wrap)
    .params(make_type<A1>(), make_type<A2>());
}

template<typename T, typename A1, typename A2, typename A3, void(*F)(T &, A1, A2, A3)>
script::FunctionBuilder void_fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::void_function_wrapper_t<void(*)(T&, A1, A2, A3), F>::wrap)
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>());
}

template<typename T, typename A1, typename A2, typename A3, typename A4, void(*F)(T &, A1, A2, A3, A4)>
script::FunctionBuilder void_fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::void_function_wrapper_t<void(*)(T&, A1, A2, A3, A4), F>::wrap)
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>(), make_type<A4>());
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, void(*F)(T &, A1, A2, A3, A4, A5)>
script::FunctionBuilder void_fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::void_function_wrapper_t<void(*)(T&, A1, A2, A3, A4, A5), F>::wrap)
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>(), make_type<A4>(), make_type<A5>());
}

template<typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, void(*F)(T &, A1, A2, A3, A4, A5, A6)>
script::FunctionBuilder void_fn_as_memfn(script::Class& cla, std::string && name)
{
  return cla.newMethod(std::move(name), wrapper::void_function_wrapper_t<void(*)(T&, A1, A2, A3, A4, A5, A6), F>::wrap)
    .params(make_type<A1>(), make_type<A2>(), make_type<A3>(), make_type<A4>(), make_type<A5>(), make_type<A6>());
}

} // namespace bind

} // namespace gonk

#endif // GONK_BINDING_FN_AS_MEMFN_H
