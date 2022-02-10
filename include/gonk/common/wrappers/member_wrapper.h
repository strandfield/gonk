// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_WRAPPERS_MEMBER_FUN_WRAPPER_H
#define GONK_WRAPPERS_MEMBER_FUN_WRAPPER_H

#include "gonk/common/values.h"

#include <script/interpreter/executioncontext.h>
#include <script/function-impl.h>

namespace gonk
{

namespace wrapper
{

/****************************************************************
Non-void member functions
****************************************************************/


template<typename MemberType, MemberType f>
struct member_wrapper_t;

template<typename R, typename ClassType, R(ClassType::*F)()const>
struct member_wrapper_t<R(ClassType::*)()const, F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(), c->engine());
  }
};

template<typename R, typename ClassType, R(ClassType::*F)()>
struct member_wrapper_t<R(ClassType::*)(), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, R(ClassType::*F)(A1)const>
struct member_wrapper_t<R(ClassType::*)(A1)const, F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, R(ClassType::*F)(A1)>
struct member_wrapper_t<R(ClassType::*)(A1), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, R(ClassType::*F)(A1, A2)const>
struct member_wrapper_t<R(ClassType::*)(A1, A2)const, F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, R(ClassType::*F)(A1, A2)>
struct member_wrapper_t<R(ClassType::*)(A1, A2), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, R(ClassType::*F)(A1, A2, A3)const>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3)const, F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, R(ClassType::*F)(A1, A2, A3)>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, R(ClassType::*F)(A1, A2, A3, A4)const>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4)const, F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, R(ClassType::*F)(A1, A2, A3, A4)>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, typename A5, R(ClassType::*F)(A1, A2, A3, A4, A5)const>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4, A5)const, F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)), value_cast<A5>(c->arg(5))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, typename A5, R(ClassType::*F)(A1, A2, A3, A4, A5)>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4, A5), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)), value_cast<A5>(c->arg(5))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, R(ClassType::*F)(A1, A2, A3, A4, A5, A6)const>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4, A5, A6)const, F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)), value_cast<A5>(c->arg(5)), value_cast<A6>(c->arg(6))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, R(ClassType::*F)(A1, A2, A3, A4, A5, A6)>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4, A5, A6), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)), value_cast<A5>(c->arg(5)), value_cast<A6>(c->arg(6))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, R(ClassType::*F)(A1, A2, A3, A4, A5, A6, A7)const>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4, A5, A6, A7)const, F> {
  static script::Value wrap(script::FunctionCall* c) {
    const ClassType& ref = value_cast<const ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)), value_cast<A5>(c->arg(5)), value_cast<A6>(c->arg(6)), value_cast<A7>(c->arg(7))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, R(ClassType::*F)(A1, A2, A3, A4, A5, A6, A7)>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4, A5, A6, A7), F> {
  static script::Value wrap(script::FunctionCall* c) {
    ClassType& ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)), value_cast<A5>(c->arg(5)), value_cast<A6>(c->arg(6)), value_cast<A7>(c->arg(7))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, R(ClassType::* F)(A1, A2, A3, A4, A5, A6, A7, A8)const>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4, A5, A6, A7, A8)const, F> {
  static script::Value wrap(script::FunctionCall* c) {
    const ClassType& ref = value_cast<const ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)), value_cast<A5>(c->arg(5)), value_cast<A6>(c->arg(6)), value_cast<A7>(c->arg(7)), value_cast<A8>(c->arg(8))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, R(ClassType::* F)(A1, A2, A3, A4, A5, A6, A7, A8)>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4, A5, A6, A7, A8), F> {
  static script::Value wrap(script::FunctionCall* c) {
    ClassType& ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)), value_cast<A5>(c->arg(5)), value_cast<A6>(c->arg(6)), value_cast<A7>(c->arg(7)), value_cast<A8>(c->arg(8))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, R(ClassType::* F)(A1, A2, A3, A4, A5, A6, A7, A8, A9)const>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9)const, F> {
  static script::Value wrap(script::FunctionCall* c) {
    const ClassType& ref = value_cast<const ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)), value_cast<A5>(c->arg(5)), value_cast<A6>(c->arg(6)), value_cast<A7>(c->arg(7)), value_cast<A8>(c->arg(8)), value_cast<A9>(c->arg(9))), c->engine());
  }
};

template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9, R(ClassType::* F)(A1, A2, A3, A4, A5, A6, A7, A8, A9)>
struct member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4, A5, A6, A7, A8, A9), F> {
  static script::Value wrap(script::FunctionCall* c) {
    ClassType& ref = value_cast<ClassType&>(c->arg(0));
    return make_value<R>(((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)), value_cast<A5>(c->arg(5)), value_cast<A6>(c->arg(6)), value_cast<A7>(c->arg(7)), value_cast<A8>(c->arg(8)), value_cast<A9>(c->arg(9))), c->engine());
  }
};

/****************************************************************
void member functions
****************************************************************/

template<typename MemberType, MemberType f>
struct void_member_wrapper_t;

template<typename ClassType, void(ClassType::*F)()>
struct void_member_wrapper_t<void(ClassType::*)(), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    ((ref).*(F))();
    return script::Value::Void;
  }
};

template<typename ClassType, typename A1, void(ClassType::*F)(A1)>
struct void_member_wrapper_t<void(ClassType::*)(A1), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    ((ref).*(F))(value_cast<A1>(c->arg(1)));
    return script::Value::Void;
  }
};

template<typename ClassType, typename A1, typename A2, void(ClassType::*F)(A1, A2)>
struct void_member_wrapper_t<void(ClassType::*)(A1, A2), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    ((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)));
    return script::Value::Void;
  }
};

template<typename ClassType, typename A1, typename A2, typename A3, void(ClassType::*F)(A1, A2, A3)>
struct void_member_wrapper_t<void(ClassType::*)(A1, A2, A3), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    ((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)));
    return script::Value::Void;
  }
};

template<typename ClassType, typename A1, typename A2, typename A3, typename A4, void(ClassType::*F)(A1, A2, A3, A4)>
struct void_member_wrapper_t<void(ClassType::*)(A1, A2, A3, A4), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    ((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)));
    return script::Value::Void;
  }
};

template<typename ClassType, typename A1, typename A2, typename A3, typename A4, typename A5, void(ClassType::*F)(A1, A2, A3, A4, A5)>
struct void_member_wrapper_t<void(ClassType::*)(A1, A2, A3, A4, A5), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    ((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)), value_cast<A5>(c->arg(5)));
    return script::Value::Void;
  }
};

template<typename ClassType, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, void(ClassType::*F)(A1, A2, A3, A4, A5, A6)>
struct void_member_wrapper_t<void(ClassType::*)(A1, A2, A3, A4, A5, A6), F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    ((ref).*(F))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4)), value_cast<A5>(c->arg(5)), value_cast<A6>(c->arg(6)));
    return script::Value::Void;
  }
};

/****************************************************************
const void member functions
****************************************************************/

template<typename MemberType, MemberType f>
struct const_void_member_wrapper_t;

template<typename ClassType, void(ClassType::*F)()const>
struct const_void_member_wrapper_t<void(ClassType::*)()const, F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    ((ref).*(F))();
    return script::Value::Void;
  }
};

template<typename ClassType, typename A1, void(ClassType::*F)(A1)const>
struct const_void_member_wrapper_t<void(ClassType::*)(A1)const, F> {
  static script::Value wrap(script::FunctionCall *c) {
    ClassType & ref = value_cast<ClassType&>(c->arg(0));
    ((ref).*(F))(value_cast<A1>(c->arg(1)));
    return script::Value::Void;
  }
};

/****************************************************************
generic member function wrapper
****************************************************************/

template<typename R, typename T, typename... Args>
class MethodWrapper : public script::FunctionImpl
{
public:
  R(T::*method)(Args...);
  std::string m_name;
  script::DynamicPrototype proto;

public:
  explicit MethodWrapper(script::Class& cla, std::string name, R(T::* mem)(Args...))
    : FunctionImpl(cla.engine()),
      method(mem),
      m_name(std::move(name))
  {
    script::Engine* e = cla.engine();
    enclosing_symbol = script::Symbol(cla).impl();
    proto.setReturnType(make_type<T>(e));
    proto.set({ make_type<T&>(e).withFlag(script::Type::ThisFlag), make_type<Args>(e)... });
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
    T& ref = value_cast<T&>(c->arg(0));
    return make_value<R>(((ref).*(method))(value_cast<typename std::tuple_element<Is, Tuple>::type>(c->arg(Is + 1))...), c->engine());
  }

  template<std::size_t... Is>
  script::Value do_invoke(script::FunctionCall* c, std::index_sequence<Is...>, std::true_type)
  {
    using Tuple = std::tuple<Args...>;
    T& ref = value_cast<T&>(c->arg(0));
    ((ref).*(method))(value_cast<typename std::tuple_element<Is, Tuple>::type>(c->arg(Is + 1))...);
    return script::Value::Void;
  }

  script::Value invoke(script::FunctionCall* c) override
  {
    return do_invoke(c, std::index_sequence_for<Args...>{}, std::integral_constant<bool, std::is_void<R>::value>());
  }
};

template<typename R, typename T, typename... Args>
class ConstMethodWrapper : public script::FunctionImpl
{
public:
  R(T::* method)(Args...)const;
  std::string m_name;
  script::DynamicPrototype proto;

public:
  explicit ConstMethodWrapper(script::Class& cla, std::string name, R(T::* mem)(Args...)const)
    : FunctionImpl(cla.engine()),
      method(mem),
      m_name(std::move(name))
  {
    script::Engine* e = cla.engine();
    enclosing_symbol = script::Symbol(cla).impl();
    proto.setReturnType(make_type<T>(e));
    proto.set({ make_type<const T&>(e).withFlag(script::Type::ThisFlag), make_type<Args>(e)... });
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
    const T& ref = value_cast<T&>(c->arg(0));
    return make_value<R>(((ref).*(method))(value_cast<typename std::tuple_element<Is, Tuple>::type>(c->arg(Is + 1))...), c->engine());
  }

  template<std::size_t... Is>
  script::Value do_invoke(script::FunctionCall* c, std::index_sequence<Is...>, std::true_type)
  {
    using Tuple = std::tuple<Args...>;
    const T& ref = value_cast<T&>(c->arg(0));
    ((ref).*(method))(value_cast<typename std::tuple_element<Is, Tuple>::type>(c->arg(Is + 1))...);
    return script::Value::Void;
  }

  script::Value invoke(script::FunctionCall* c) override
  {
    return do_invoke(c, std::index_sequence_for<Args...>{}, std::integral_constant<bool, std::is_void<R>::value>());
  }
};

} // namespace wrapper

} // namespace gonk

#endif // GONK_WRAPPERS_MEMBER_FUN_WRAPPER_H
