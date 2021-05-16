// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_WRAPPERS_FUNCTION_WRAPPER_H
#define GONK_WRAPPERS_FUNCTION_WRAPPER_H

#include "gonk/common/values.h"

#include <script/interpreter/executioncontext.h>
#include <script/function-impl.h>

#include <tuple>
#include <type_traits>
#include <utility>

namespace gonk
{

namespace wrapper
{

/****************************************************************
Non-void functions
****************************************************************/

template<typename FuncType, FuncType f>
struct function_wrapper_t
{
  static script::Value wrap(script::FunctionCall *c) = delete;
};


template<typename R, R(*f)()>
struct function_wrapper_t<R(*)(), f> {
  static script::Value wrap(script::FunctionCall *c) {
    return make_value<R>(f(), c->engine());
  }
};

template<typename R, typename Arg, R(*f)(Arg)>
struct function_wrapper_t<R(*)(Arg), f> {
  static script::Value wrap(script::FunctionCall *c) {
    return make_value<R>(f(value_cast<Arg>(c->arg(0))), c->engine());
  }
};

template<typename R, typename A1, typename A2, R(*f)(A1, A2)>
struct function_wrapper_t<R(*)(A1, A2), f> {
  static script::Value wrap(script::FunctionCall *c) {
    return make_value<R>(f(value_cast<A1>(c->arg(0)), value_cast<A2>(c->arg(1))), c->engine());
  }
};

template<typename R, typename A1, typename A2, typename A3, R(*f)(A1, A2, A3)>
struct function_wrapper_t<R(*)(A1, A2, A3), f> {
  static script::Value wrap(script::FunctionCall *c) {
    return make_value<R>(f(value_cast<A1>(c->arg(0)), value_cast<A2>(c->arg(1)), value_cast<A3>(c->arg(2))), c->engine());
  }
};

template<typename R, typename A1, typename A2, typename A3, typename A4, R(*f)(A1, A2, A3, A4)>
struct function_wrapper_t<R(*)(A1, A2, A3, A4), f> {
  static script::Value wrap(script::FunctionCall *c) {
    return make_value<R>(f(value_cast<A1>(c->arg(0)), value_cast<A2>(c->arg(1)), value_cast<A3>(c->arg(2)), value_cast<A4>(c->arg(3))), c->engine());
  }
};

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5, R(*f)(A1, A2, A3, A4, A5)>
struct function_wrapper_t<R(*)(A1, A2, A3, A4, A5), f> {
  static script::Value wrap(script::FunctionCall *c) {
    return make_value<R>(f(value_cast<A1>(c->arg(0)), value_cast<A2>(c->arg(1)), value_cast<A3>(c->arg(2)), value_cast<A4>(c->arg(3)), value_cast<A5>(c->arg(4))), c->engine());
  }
};

template<typename R, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, R(*f)(A1, A2, A3, A4, A5, A6)>
struct function_wrapper_t<R(*)(A1, A2, A3, A4, A5, A6), f> {
  static script::Value wrap(script::FunctionCall *c) {
    return make_value<R>(f(value_cast<A1>(c->arg(0)), value_cast<A2>(c->arg(1)), value_cast<A3>(c->arg(2)), value_cast<A4>(c->arg(3)), value_cast<A5>(c->arg(4)), value_cast<A6>(c->arg(5))), c->engine());
  }
};


/****************************************************************
Function returning void
****************************************************************/

template<typename FuncType, FuncType f>
struct void_function_wrapper_t;

template<void(*f)()>
struct void_function_wrapper_t<void(*)(), f> {
  static script::Value wrap(script::FunctionCall *c) {
    f();
    return script::Value::Void;
  }
};

template<typename Arg, void(*f)(Arg)>
struct void_function_wrapper_t<void(*)(Arg), f> {
  static script::Value wrap(script::FunctionCall *c) {
    f(value_cast<Arg>(c->arg(0)));
    return script::Value::Void;
  }
};

template<typename A1, typename A2, void(*f)(A1, A2)>
struct void_function_wrapper_t<void(*)(A1, A2), f> {
  static script::Value wrap(script::FunctionCall *c) {
    f(value_cast<A1>(c->arg(0)), value_cast<A2>(c->arg(1)));
    return script::Value::Void;
  }
};

template<typename A1, typename A2, typename A3, void(*f)(A1, A2, A3)>
struct void_function_wrapper_t<void(*)(A1, A2, A3), f> {
  static script::Value wrap(script::FunctionCall *c) {
    f(value_cast<A1>(c->arg(0)), value_cast<A2>(c->arg(1)), value_cast<A3>(c->arg(2)));
    return script::Value::Void;
  }
};

template<typename A1, typename A2, typename A3, typename A4, void(*f)(A1, A2, A3, A4)>
struct void_function_wrapper_t<void(*)(A1, A2, A3, A4), f> {
  static script::Value wrap(script::FunctionCall *c) {
    f(value_cast<A1>(c->arg(0)), value_cast<A2>(c->arg(1)), value_cast<A3>(c->arg(2)), value_cast<A4>(c->arg(3)));
    return script::Value::Void;
  }
};

template<typename A1, typename A2, typename A3, typename A4, typename A5, void(*f)(A1, A2, A3, A4, A5)>
struct void_function_wrapper_t<void(*)(A1, A2, A3, A4, A5), f> {
  static script::Value wrap(script::FunctionCall *c) {
    f(value_cast<A1>(c->arg(0)), value_cast<A2>(c->arg(1)), value_cast<A3>(c->arg(2)), value_cast<A4>(c->arg(3)), value_cast<A5>(c->arg(4)));
    return script::Value::Void;
  }
};

template<typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, void(*f)(A1, A2, A3, A4, A5, A6)>
struct void_function_wrapper_t<void(*)(A1, A2, A3, A4, A5, A6), f> {
  static script::Value wrap(script::FunctionCall *c) {
    f(value_cast<A1>(c->arg(0)), value_cast<A2>(c->arg(1)), value_cast<A3>(c->arg(2)), value_cast<A4>(c->arg(3)), value_cast<A5>(c->arg(4)), value_cast<A6>(c->arg(5)));
    return script::Value::Void;
  }
};

template<typename T, typename... Args>
class FunctionWrapper : public script::FunctionImpl
{
public:
  T(*function)(Args...);
  std::string m_name;
  script::DynamicPrototype proto;

public:
  explicit FunctionWrapper(script::Symbol sym, std::string name, T(*fun)(Args...))
    : FunctionImpl(sym.engine()),
      function(fun),
      m_name(std::move(name))
  {
    enclosing_symbol = sym.impl();
    proto.setReturnType(make_type<T>());
    proto.set({ make_type<Args>()... });
  }

  const std::string& name() const override
  {
    return m_name;
  }

  bool is_native() const override
  {
    return true;
  }

  void set_body(std::shared_ptr<script::program::Statement>) override
  {

  }

  const script::Prototype& prototype() const override
  {
    return proto;
  }

  template<std::size_t... Is>
  script::Value do_invoke(script::FunctionCall* c, std::index_sequence<Is...>, std::false_type)
  {
    using Tuple = std::tuple<Args...>;
    return make_value<T>(function(value_cast<typename std::tuple_element<Is, Tuple>::type>(c->arg(Is))...), c->engine());
  }

  template<std::size_t... Is>
  script::Value do_invoke(script::FunctionCall* c, std::index_sequence<Is...>, std::true_type)
  {
    using Tuple = std::tuple<Args...>;
    function(value_cast<typename std::tuple_element<Is, Tuple>::type>(c->arg(Is))...);
    return script::Value::Void;
  }

  script::Value invoke(script::FunctionCall* c) override
  {
    return do_invoke(c, std::index_sequence_for<Args...>{}, std::integral_constant<bool, std::is_void<T>::value>());
  }
};

} // namespace wrapper

} // namespace gonk

#endif // GONK_WRAPPERS_FUNCTION_WRAPPER_H
