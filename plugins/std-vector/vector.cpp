// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "vector.h"

#include "gonk/common/semvalue.h"

#include <script/classtemplateinstancebuilder.h>
#include <script/namespace.h>
#include <script/symbol.h>
#include <script/templatebuilder.h>

namespace gonk
{

namespace std_vector
{

namespace callbacks
{

// std::vector();
static script::Value default_ctor(script::FunctionCall* c)
{
  c->thisObject().init<std::vector<SemValue>>();
  return c->thisObject();
}

// std::vector(const std::vector<T>& other);
static script::Value copy_ctor(script::FunctionCall* c)
{
  const std::vector<SemValue>& other = script::get<std::vector<SemValue>>(c->arg(1));
  c->thisObject().init<std::vector<SemValue>>(other);
  return c->thisObject();
}

// ~std::vector()
static script::Value dtor(script::FunctionCall* c)
{
  c->thisObject().destroy<std::vector<SemValue>>();
  return script::Value::Void;
}

// std::vector(int count);
static script::Value ctor_int(script::FunctionCall* c)
{
  const int count = script::get<int>(c->arg(1));
  const std::vector<script::Value> args;
  SemValue value{ c->engine()->construct(TypeInfo::get(c->callee().memberOf())->element_type, args) };
  c->thisObject().init<std::vector<SemValue>>(static_cast<size_t>(count), value);
  return c->thisObject();
}

// std::vector(int size, const T& value);
static script::Value ctor_int_T(script::FunctionCall* c)
{
  const int size = script::get<int>(c->arg(1));
  ObserverValue value = semarg(c, 2);
  c->thisObject().init<std::vector<SemValue>>(static_cast<size_t>(size), value);
  return c->thisObject();
}

// std::vector<T>& operator=(const std::vector<T>& other);
static script::Value op_assign(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  const std::vector<SemValue>& other = script::get<std::vector<SemValue>>(c->arg(1));
  self = other;
  return c->arg(0);
}

// void assign(int count, const T& value);
static script::Value assign_int_T(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  const int count = script::get<int>(c->arg(1));
  ObserverValue value = semarg(c, 2);
  self.assign(count, value);
  return script::Value::Void;
}

// T at(int index) const;
static script::Value at_int(script::FunctionCall* c)
{
  const std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  const int index = script::get<int>(c->arg(1));
  SemValue value{ self.at(static_cast<size_t>(index)) }; // creates a copy
  return value.release(); // releases the copy
}

// T& operator[](int index);
static script::Value op_subscript(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  const int index = script::get<int>(c->arg(1));
  return self[static_cast<size_t>(index)].get();
}

// T front() const;
static script::Value front_const(script::FunctionCall* c)
{
  const std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  SemValue value{ self.front() }; // creates a copy
  return value.release(); // releases the copy
}

// T& front();
static script::Value front(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  return self.front().get();
}

// T back() const;
static script::Value back_const(script::FunctionCall* c)
{
  const std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  SemValue value{ self.back() }; // creates a copy
  return value.release(); // releases the copy
}

// T& back();
static script::Value back(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  return self.back().get();
}

// bool empty() const;
static script::Value empty(script::FunctionCall* c)
{
  const std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  return c->engine()->newBool(self.empty());
}

// int size() const;
static script::Value size(script::FunctionCall* c)
{
  const std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  return c->engine()->newInt(static_cast<int>(self.size()));
}

// void reserve(int capacity);
static script::Value reserve(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  int capacity = script::get<int>(c->arg(1));
  self.reserve(static_cast<size_t>(capacity));
  return script::Value::Void;
}

// int capacity() const;
static script::Value capacity(script::FunctionCall* c)
{
  const std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  return c->engine()->newInt(static_cast<int>(self.capacity()));
}

// void shrink_to_fit();
static script::Value shrink_to_fit(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  self.shrink_to_fit();
  return script::Value::Void;
}

// void clear();
static script::Value clear(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  self.clear();
  return script::Value::Void;
}

// void insert(int index, const T& value);
static script::Value insert_int_T(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  const int index = script::get<int>(c->arg(1));
  ObserverValue value = semarg(c, 2);
  self.insert(self.begin() + static_cast<size_t>(index), value);
  return script::Value::Void;
}

// void erase(int index);
static script::Value erase_int(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  const int index = script::get<int>(c->arg(1));
  self.erase(self.begin() + static_cast<size_t>(index));
  return script::Value::Void;
}

// void push_back(const T& value);
static script::Value push_back(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  ObserverValue value = semarg(c, 1);
  self.push_back(value);
  return script::Value::Void;
}

// void pop_back();
static script::Value pop_back(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  self.pop_back();
  return script::Value::Void;
}

// void resize(int index);
static script::Value resize_int(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  const int size = script::get<int>(c->arg(1));
  const std::vector<script::Value> args;
  SemValue value{ c->engine()->construct(TypeInfo::get(c->callee().memberOf())->element_type, args) };
  self.resize(static_cast<size_t>(size), value);
  return script::Value::Void;
}

// void resize(int index, const T& value);
static script::Value resize_int_T(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  const int size = script::get<int>(c->arg(1));
  ObserverValue value = semarg(c, 2);
  self.resize(static_cast<size_t>(size), value);
  return script::Value::Void;
}

// bool operator==(const std::vector<T>& other) const;
static script::Value eq(script::FunctionCall* c)
{
  const std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  const std::vector<SemValue>& other = script::get<std::vector<SemValue>>(c->arg(1));
  return c->engine()->newBool(self == other);
}

// bool operator!=(const std::vector<T>& other) const;
static script::Value neq(script::FunctionCall* c)
{
  const std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  const std::vector<SemValue>& other = script::get<std::vector<SemValue>>(c->arg(1));
  return c->engine()->newBool(self != other);
}

} // namespace callbacks

void fill_instance(script::Class& c, script::Type t)
{
  // std::vector();
  c.newConstructor(callbacks::default_ctor).create();
  // std::vector(const std::vector<T>& other);
  c.newConstructor(callbacks::copy_ctor)
    .params(script::Type::cref(c.id())).create();
  // ~std::vector();
  c.newDestructor(callbacks::dtor).create();

  // std::vector(int count);
  c.newConstructor(callbacks::ctor_int)
    .params(script::Type::Int)
    .create();
  // std::vector(int size, const T& value);
  c.newConstructor(callbacks::ctor_int_T)
    .params(script::Type::Int, script::Type::cref(t))
    .create();

  // std::vector<T> operator=(const std::vector<T>& other);
  c.newOperator(script::AssignmentOperator, callbacks::op_assign)
    .returns(script::Type::ref(c.id()))
    .params(script::Type::cref(c.id()))
    .create();
  // void assign(int count, const T& value);
  c.newMethod("assign", callbacks::assign_int_T)
    .params(script::Type::Int, script::Type::cref(t))
    .create();

  // T at(int index) const;
  c.newMethod("at", callbacks::at_int)
    .returns(t)
    .params(script::Type::Int)
    .setConst()
    .create();
  // T& operator[](int index);
  c.newOperator(script::SubscriptOperator, callbacks::op_subscript)
    .returns(script::Type::ref(t))
    .params(script::Type::Int)
    .create();
  // T front() const;
  c.newMethod("front", callbacks::front_const)
    .returns(t)
    .setConst()
    .create();
  // T& front();
  c.newMethod("front", callbacks::front)
    .returns(script::Type::ref(t))
    .create();
  // T back() const;
  c.newMethod("back", callbacks::back_const)
    .returns(t)
    .setConst()
    .create();
  // T& back();
  c.newMethod("back", callbacks::back)
    .returns(script::Type::ref(t))
    .create();

  // bool empty() const;
  c.newMethod("empty", callbacks::empty)
    .returns(script::Type::Boolean)
    .setConst()
    .create();
  // int size() const;
  c.newMethod("size", callbacks::size)
    .returns(script::Type::Int)
    .setConst()
    .create();
  // void reserve(int capacity);
  c.newMethod("reserve", callbacks::reserve)
    .params(script::Type::Int)
    .create();
  // int capacity() const;
  c.newMethod("capacity", callbacks::capacity)
    .returns(script::Type::Int)
    .setConst()
    .create();
  // void shrink_to_fit();
  c.newMethod("shrink_to_fit", callbacks::shrink_to_fit)
    .create();

  // void clear();
  c.newMethod("clear", callbacks::clear)
    .create();
  // void insert(int index, const T& value);
  c.newMethod("insert", callbacks::insert_int_T)
    .params(script::Type::Int, script::Type::cref(t))
    .create();
  // void erase(int index);
  c.newMethod("erase", callbacks::erase_int)
    .params(script::Type::Int)
    .create();
  // void push_back(const T& value);
  c.newMethod("push_back", callbacks::push_back)
    .params(script::Type::cref(t))
    .create();
  // void pop_back();
  c.newMethod("pop_back", callbacks::pop_back)
    .create();
  // void resize(int size);
  c.newMethod("resize", callbacks::resize_int)
    .params(script::Type::Int)
    .create();
  // void resize(int size, const T& value);
  c.newMethod("resize", callbacks::resize_int_T)
    .params(script::Type::Int, script::Type::cref(t))
    .create();

  // bool operator==(const std::vector<T>& other) const;
  c.newOperator(script::EqualOperator, callbacks::eq)
    .params(script::Type::cref(c.id()))
    .setConst()
    .create();

  // bool operator!=(const std::vector<T>& other) const;
  c.newOperator(script::InequalOperator, callbacks::neq)
    .params(script::Type::cref(c.id()))
    .setConst()
    .create();
}

} // namespace std_vector

script::Class VectorTemplate::instantiate(script::ClassTemplateInstanceBuilder& builder)
{
  builder.setFinal();
  const script::Type element_type = builder.arguments().front().type;

  builder.setData(gonk::TypeInfo::get(builder.getTemplate().engine(), element_type));

  script::Class vector = builder.get();

  gonk::std_vector::fill_instance(vector, element_type);

  return vector;
}

} // namespace gonk

void register_vector_file(script::Namespace ns)
{
  using namespace script;

  script::ClassTemplate vector_template = script::Symbol{ ns }.newClassTemplate("vector")
    .params(script::TemplateParameter(script::TemplateParameter::TypeParameter{}, "T"))
    .setScope(script::Scope(ns))
    .withBackend<gonk::VectorTemplate>()
    .get();
    
  gonk::std_vector::register_specialization<int>(vector_template, script::make_type<std::vector<int>>());
}
