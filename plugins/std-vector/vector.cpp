// Copyright (C) 2020-2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "vector.h"

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
  c->thisObject() = script::Value(new script::CppValue<std::vector<SemValue>>(c->engine(), c->callee().parameter(0).baseType(), std::vector<SemValue>()));
  return c->thisObject();
}

// std::vector(const std::vector<T>& other);
static script::Value copy_ctor(script::FunctionCall* c)
{
  const std::vector<SemValue>& other = script::get<std::vector<SemValue>>(c->arg(1));
  c->thisObject() = script::Value(new script::CppValue<std::vector<SemValue>>(c->engine(), c->callee().parameter(0).baseType(), other));
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
  SemValue value{ VectorTemplate::info(c).element_type->defaultConstruct() };
  c->thisObject() = script::Value(new script::CppValue<std::vector<SemValue>>(c->engine(), c->callee().parameter(0).baseType(), std::vector<SemValue>(static_cast<size_t>(count), value)));
  return c->thisObject();
}

// std::vector(int size, const T& value);
static script::Value ctor_int_T(script::FunctionCall* c)
{
  const int size = script::get<int>(c->arg(1));
  ObserverValue value{ VectorTemplate::info(c).element_type, c->arg(2) };
  c->thisObject() = script::Value(new script::CppValue<std::vector<SemValue>>(c->engine(), c->callee().parameter(0).baseType(), std::vector<SemValue>(static_cast<size_t>(size), value)));
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
  ObserverValue value{ VectorTemplate::info(c).element_type,  c->arg(2) };
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
  ObserverValue value{ VectorTemplate::info(c).element_type,  c->arg(2) };
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
  ObserverValue value{ VectorTemplate::info(c).element_type,  c->arg(1) };
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
  SemValue value{ VectorTemplate::info(c).element_type->defaultConstruct() };
  self.resize(static_cast<size_t>(size), value);
  return script::Value::Void;
}

// void resize(int index, const T& value);
static script::Value resize_int_T(script::FunctionCall* c)
{
  std::vector<SemValue>& self = script::get<std::vector<SemValue>>(c->arg(0));
  const int size = script::get<int>(c->arg(1));
  ObserverValue value{ VectorTemplate::info(c).element_type,  c->arg(2) };
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
  script::FunctionBuilder::Constructor(c).setCallback(callbacks::default_ctor).create();
  // std::vector(const std::vector<T>& other);
  script::FunctionBuilder::Constructor(c).setCallback(callbacks::copy_ctor)
    .params(script::Type::cref(c.id())).create();
  // ~std::vector();
  script::FunctionBuilder::Destructor(c).setCallback(callbacks::dtor).create();

  // std::vector(int count);
  script::FunctionBuilder::Constructor(c).setCallback(callbacks::ctor_int)
    .params(script::Type::Int)
    .create();
  // std::vector(int size, const T& value);
  script::FunctionBuilder::Constructor(c).setCallback(callbacks::ctor_int_T)
    .params(script::Type::Int, script::Type::cref(t))
    .create();

  // std::vector<T> operator=(const std::vector<T>& other);
  script::FunctionBuilder::Op(c, script::AssignmentOperator).setCallback(callbacks::op_assign)
    .returns(script::Type::ref(c.id()))
    .params(script::Type::cref(c.id()))
    .create();

  // void assign(int count, const T& value);
  script::FunctionBuilder::Fun(c, "assign").setCallback(callbacks::assign_int_T)
    .params(script::Type::Int, script::Type::cref(t))
    .create();

  // T at(int index) const;
  script::FunctionBuilder::Fun(c, "at").setCallback(callbacks::at_int)
    .returns(t)
    .params(script::Type::Int)
    .setConst()
    .create();
  // T& operator[](int index);
  script::FunctionBuilder::Op(c, script::SubscriptOperator).setCallback(callbacks::op_subscript)
    .returns(script::Type::ref(t))
    .params(script::Type::Int)
    .create();
  // T front() const;
  script::FunctionBuilder::Fun(c, "front").setCallback(callbacks::front_const)
    .returns(t)
    .setConst()
    .create();
  // T& front();
  script::FunctionBuilder::Fun(c, "front").setCallback(callbacks::front)
    .returns(script::Type::ref(t))
    .create();
  // T back() const;
  script::FunctionBuilder::Fun(c, "back").setCallback(callbacks::back_const)
    .returns(t)
    .setConst()
    .create();
  // T& back();
  script::FunctionBuilder::Fun(c, "back").setCallback(callbacks::back)
    .returns(script::Type::ref(t))
    .create();

  // bool empty() const;
  script::FunctionBuilder::Fun(c, "empty").setCallback(callbacks::empty)
    .returns(script::Type::Boolean)
    .setConst()
    .create();
  // int size() const;
  script::FunctionBuilder::Fun(c, "size").setCallback(callbacks::size)
    .returns(script::Type::Int)
    .setConst()
    .create();
  // void reserve(int capacity);
  script::FunctionBuilder::Fun(c, "reserve").setCallback(callbacks::reserve)
    .params(script::Type::Int)
    .create();
  // int capacity() const;
  script::FunctionBuilder::Fun(c, "capacity").setCallback(callbacks::capacity)
    .returns(script::Type::Int)
    .setConst()
    .create();
  // void shrink_to_fit();
  script::FunctionBuilder::Fun(c, "shrink_to_fit").setCallback(callbacks::shrink_to_fit)
    .create();

  // void clear();
  script::FunctionBuilder::Fun(c, "clear").setCallback(callbacks::clear)
    .create();
  // void insert(int index, const T& value);
  script::FunctionBuilder::Fun(c, "insert").setCallback(callbacks::insert_int_T)
    .params(script::Type::Int, script::Type::cref(t))
    .create();
  // void erase(int index);
  script::FunctionBuilder::Fun(c, "erase").setCallback(callbacks::erase_int)
    .params(script::Type::Int)
    .create();
  // void push_back(const T& value);
  script::FunctionBuilder::Fun(c, "push_back").setCallback(callbacks::push_back)
    .params(script::Type::cref(t))
    .create();
  // void pop_back();
  script::FunctionBuilder::Fun(c, "pop_back").setCallback(callbacks::pop_back)
    .create();
  // void resize(int size);
  script::FunctionBuilder::Fun(c, "resize").setCallback(callbacks::resize_int)
    .params(script::Type::Int)
    .create();
  // void resize(int size, const T& value);
  script::FunctionBuilder::Fun(c, "resize").setCallback(callbacks::resize_int_T)
    .params(script::Type::Int, script::Type::cref(t))
    .create();

  // bool operator==(const std::vector<T>& other) const;
  script::FunctionBuilder::Op(c, script::EqualOperator).setCallback(callbacks::eq)
    .params(script::Type::cref(c.id()))
    .setConst()
    .create();

  // bool operator!=(const std::vector<T>& other) const;
  script::FunctionBuilder::Op(c, script::InequalOperator).setCallback(callbacks::neq)
    .params(script::Type::cref(c.id()))
    .setConst()
    .create();
}

} // namespace std_vector

VectorTemplate::InstanceInfo& VectorTemplate::info(script::FunctionCall* c)
{
  return static_cast<InstanceInfo&>(*c->callee().memberOf().data());
}

script::Class VectorTemplate::instantiate(script::ClassTemplateInstanceBuilder& builder)
{
  builder.setFinal();
  const script::Type element_type = builder.arguments().front().type;

  auto instance_info = std::make_shared<InstanceInfo>();
  instance_info->element_type = gonk::TypeInfo::get(builder.getTemplate().engine(), element_type);
  builder.setData(instance_info);

  script::Class vector = builder.get();

  gonk::std_vector::fill_instance(vector, element_type);

  return vector;
}

} // namespace gonk

void register_vector_file(script::Namespace ns)
{
  using namespace script;

  script::Engine* e = ns.engine();

  script::ClassTemplate vector_template = script::ClassTemplateBuilder(script::Symbol(ns), "vector")
    .params(script::TemplateParameter(script::TemplateParameter::TypeParameter{}, "T"))
    .setScope(script::Scope(ns))
    .withBackend<gonk::VectorTemplate>()
    .get();
    
  gonk::std_vector::register_specialization<int>(vector_template, e->registerType<std::vector<int>>());
  gonk::std_vector::register_specialization<std::string>(vector_template, e->registerType<std::vector<std::string>>());
}
