// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "map.h"

#include <script/classtemplateinstancebuilder.h>
#include <script/namelookup.h>
#include <script/namespace.h>
#include <script/overloadresolution.h>
#include <script/symbol.h>
#include <script/templatebuilder.h>

namespace gonk
{

namespace std_map
{

namespace callbacks
{

// std::map();
static script::Value default_ctor(script::FunctionCall* c)
{
  c->thisObject() = script::Value(new script::CppValue<Map>(c->engine(), c->callee().parameter(0).baseType(), Map()));
  return c->thisObject();
}

// std::map(const std::map<T>& other);
static script::Value copy_ctor(script::FunctionCall* c)
{
  const Map& other = script::get<Map>(c->arg(1));
  c->thisObject() = script::Value(new script::CppValue<Map>(c->engine(), c->callee().parameter(0).baseType(), other));
  return c->thisObject();
}

// ~std::map()
static script::Value dtor(script::FunctionCall* c)
{
  c->thisObject().destroy<Map>();
  return script::Value::Void;
}

// std::map<Key, T>& operator=(const std::map<Key, T>& other);
static script::Value op_assign(script::FunctionCall* c)
{
  Map& self = script::get<Map>(c->arg(0));
  const Map& other = script::get<Map>(c->arg(1));
  self = other;
  return c->arg(0);
}

// T& operator[](const Key& k);
static script::Value op_subscript(script::FunctionCall* c)
{
  Map& self = script::get<Map>(c->arg(0));
  map::FakeKey key{ c, c->arg(1) };
  map::Element& elem = self[key];
  
  if (!elem.type)
    elem = map::Element(MapTemplate::info(c).element_type);

  return elem.value;
}

// const T& at(const Key& key) const;
static script::Value at(script::FunctionCall* c)
{
  Map& self = script::get<Map>(c->arg(0));
  map::FakeKey key{ c, c->arg(1) };
  const map::Element& elem = self.at(key);
  return elem.value;
}

// bool empty() const;
static script::Value empty(script::FunctionCall* c)
{
  const Map& self = script::get<Map>(c->arg(0));
  return c->engine()->newBool(self.empty());
}

// int size() const;
static script::Value size(script::FunctionCall* c)
{
  const Map& self = script::get<Map>(c->arg(0));
  return c->engine()->newInt(static_cast<int>(self.size()));
}

// void clear();
static script::Value clear(script::FunctionCall* c)
{
  Map& self = script::get<Map>(c->arg(0));
  self.clear();
  return script::Value::Void;
}

// void erase(const Key& key);
static script::Value erase_int(script::FunctionCall* c)
{
  Map& self = script::get<Map>(c->arg(0));
  map::FakeKey key{ c, c->arg(1) };
  self.erase(key);
  return script::Value::Void;
}

// int count(const Key& key) const;
static script::Value count(script::FunctionCall* c)
{
  Map& self = script::get<Map>(c->arg(0));
  map::FakeKey key{ c, c->arg(1) };
  return c->engine()->newInt(static_cast<int>(self.count(key)));
}

// bool operator==(const std::map<Key, T>& other) const;
static script::Value eq(script::FunctionCall* c)
{
  const Map& self = script::get<Map>(c->arg(0));
  const Map& other = script::get<Map>(c->arg(1));
  return c->engine()->newBool(self == other);
}

// bool operator!=(const std::map<Key, T>& other) const;
static script::Value neq(script::FunctionCall* c)
{
  const Map& self = script::get<Map>(c->arg(0));
  const Map& other = script::get<Map>(c->arg(1));
  return c->engine()->newBool(self != other);
}

} // namespace callbacks

void fill_instance(script::Class& c, script::Type key_type, script::Type element_type)
{
  // std::map();
  script::ConstructorBuilder(c).setCallback(callbacks::default_ctor).create();
  // std::map(const std::map<Key, T>& other);
  script::ConstructorBuilder(c).setCallback(callbacks::copy_ctor)
    .params(script::Type::cref(c.id())).create();
  // ~std::map();
  script::DestructorBuilder(c).setCallback(callbacks::dtor).create();

  // std::map<Key, T>& operator=(const std::map<Key, T>& other);
  script::OperatorBuilder(script::Symbol(c), script::AssignmentOperator).setCallback(callbacks::op_assign)
    .returns(script::Type::ref(c.id()))
    .params(script::Type::cref(c.id()))
    .create();

  // T& operator[](const Key& key);
  script::OperatorBuilder(script::Symbol(c), script::SubscriptOperator).setCallback(callbacks::op_subscript)
    .returns(script::Type::ref(element_type))
    .params(script::Type::cref(key_type))
    .create();
  // const T& at(const Key& key) const;
  script::FunctionBuilder(c, "at").setCallback(callbacks::at)
    .returns(script::Type::cref(element_type))
    .params(script::Type::cref(key_type))
    .setConst()
    .create();

  // bool empty() const;
  script::FunctionBuilder(c, "empty").setCallback(callbacks::empty)
    .returns(script::Type::Boolean)
    .setConst()
    .create();
  // int size() const;
  script::FunctionBuilder(c, "size").setCallback(callbacks::size)
    .returns(script::Type::Int)
    .setConst()
    .create();

  // void clear();
  script::FunctionBuilder(c, "clear").setCallback(callbacks::clear)
    .create();
  // void erase(const Key& key);
  script::FunctionBuilder(c, "erase").setCallback(callbacks::erase_int)
    .params(script::Type::cref(key_type))
    .create();

  // int count(const Key& key) const;
  script::FunctionBuilder(c, "count").setCallback(callbacks::count)
    .returns(script::Type::Int)
    .params(script::Type::cref(key_type))
    .setConst()
    .create();

  // bool operator==(const std::map<Key, T>& other) const;
  script::OperatorBuilder(script::Symbol(c), script::EqualOperator).setCallback(callbacks::eq)
    .params(script::Type::cref(c.id()))
    .setConst()
    .create();

  // bool operator!=(const std::map<Key, T>& other) const;
  script::OperatorBuilder(script::Symbol(c), script::InequalOperator).setCallback(callbacks::neq)
    .params(script::Type::cref(c.id()))
    .setConst()
    .create();
}

} // namespace std_map

namespace map
{

std::map<int, std::shared_ptr<KeyType>>& KeyType::get_map()
{
  static std::map<int, std::shared_ptr<KeyType>> map = {};
  return map;
}

std::shared_ptr<KeyType> KeyType::get(script::Engine* e, const script::Type& t)
{
  auto& typeinfomap = get_map();

  auto it = typeinfomap.find(t.baseType().data());
  if (it != typeinfomap.end())
    return it->second;

  std::shared_ptr<KeyType> ret = std::make_shared<KeyType>();
  ret->type = t.baseType();
  ret->engine = e;

  if (t.isObjectType() || t.isFundamentalType())
  {
    ret->eq = TypeInfo::get_eq(e, t);
    ret->assign = TypeInfo::get_assign(e, t);
    ret->less = TypeInfo::get_less(e, t);
  }
  else
  {
    throw std::runtime_error{ "Not implemented" };
  }

  typeinfomap[t.baseType().data()] = ret;
  return ret;
}

Key::Key()
  : type(nullptr)
{

}

Key::Key(const Key& other)
  : type(other.type)
{
  if (type)
    value = engine()->copy(other.value);
}

Key::Key(Key&& other)
  : type(other.type),
    value(other.value)
{
  other.type = nullptr;
  other.value = script::Value();
}

Key::~Key()
{
  if (type)
    engine()->destroy(value);
}

Key& Key::operator=(const Key& other)
{
  if (type)
  {
    assert(type == other.type);
    type->assign.invoke({ value, other.value });
  }
  else
  {
    type = other.type;

    if (type)
      value = engine()->copy(other.value);
  }

  return *this;
}

script::Engine* Key::engine() const
{
  return type ? type->engine : nullptr;
}

FakeKey::FakeKey(script::FunctionCall* c, script::Value val)
{
  type = MapTemplate::info(c).key_type;
  value = val;
}

FakeKey::~FakeKey()
{
  type = nullptr;
  value = script::Value();
}

bool operator<(const Key& lhs, const Key& rhs)
{
  if (lhs.type.get() < rhs.type.get())
    return true;
  else if (lhs.type.get() > rhs.type.get())
    return false;

  if (lhs.type == nullptr)
    return false;

  return lhs.type->less.invoke({ lhs.value, rhs.value }).toBool();
}

bool operator==(const Key& lhs, const Key& rhs)
{
  if (lhs.type != rhs.type)
    return false;
  if (rhs.type == nullptr)
    return true;

  return lhs.type->eq.invoke({ lhs.value, rhs.value }).toBool();
}


std::map<int, std::shared_ptr<ElementType>>& ElementType::get_map()
{
  static std::map<int, std::shared_ptr<ElementType>> map = {};
  return map;
}

std::shared_ptr<ElementType> ElementType::get(script::Engine* e, const script::Type& t)
{
  auto& typeinfomap = get_map();

  auto it = typeinfomap.find(t.baseType().data());
  if (it != typeinfomap.end())
    return it->second;

  std::shared_ptr<ElementType> ret = std::make_shared<ElementType>();
  ret->type = t.baseType();
  ret->engine = e;

  if (t.isObjectType() || t.isFundamentalType())
  {
    ret->eq = TypeInfo::get_eq(e, t);
    ret->assign = TypeInfo::get_assign(e, t);
  }
  else
  {
    throw std::runtime_error{ "Not implemented" };
  }

  typeinfomap[t.baseType().data()] = ret;
  return ret;
}

Element::Element()
  : type(nullptr)
{

}

Element::Element(const Element& other)
  : type(other.type)
{
  if (type)
    value = engine()->copy(other.value);
}

Element::Element(Element&& other)
  : type(other.type),
    value(other.value)
{
  other.type = nullptr;
  other.value = script::Value();
}

Element::~Element()
{
  if (type)
    engine()->destroy(value);
}


Element::Element(std::shared_ptr<ElementType> t)
  : type(t)
{
  value = t->engine->construct(t->type, std::vector<script::Value>());
}

Element::Element(std::shared_ptr<ElementType> t, script::Value&& v)
  : type(t),
    value(v)
{

}

Element& Element::operator=(const Element& other)
{
  if (type)
  {
    assert(type == other.type);
    type->assign.invoke({ value, other.value });
  }
  else
  {
    type = other.type;

    if (type)
      value = engine()->copy(other.value);
  }

  return *this;
}

script::Engine* Element::engine() const
{
  return type ? type->engine : nullptr;
}

bool operator==(const Element& lhs, const Element& rhs)
{
  if (lhs.type != rhs.type)
    return false;
  if (rhs.type == nullptr)
    return true;

  return lhs.type->eq.invoke({ lhs.value, rhs.value }).toBool();
}

} // namespace map

MapTemplate::InstanceInfo& MapTemplate::info(script::FunctionCall* c)
{
  return static_cast<InstanceInfo&>(*c->callee().memberOf().data());
}

script::Class MapTemplate::instantiate(script::ClassTemplateInstanceBuilder& builder)
{
  builder.setFinal();
  const script::Type key_type = builder.arguments().front().type;
  const script::Type element_type = builder.arguments().back().type;

  auto instance_info = std::make_shared<InstanceInfo>();
  instance_info->key_type = map::KeyType::get(builder.getTemplate().engine(), key_type);
  instance_info->element_type = map::ElementType::get(builder.getTemplate().engine(), element_type);
  builder.setData(instance_info);

  script::Class map = builder.get();

  gonk::std_map::fill_instance(map, key_type, element_type);

  return map;
}

} // namespace gonk

void register_map_template(script::Namespace ns)
{
  using namespace script;

  script::ClassTemplate map_template = script::Symbol{ ns }.newClassTemplate("map")
    .params(script::TemplateParameter(script::TemplateParameter::TypeParameter{}, "Key"), script::TemplateParameter(script::TemplateParameter::TypeParameter{}, "T"))
    .setScope(script::Scope(ns))
    .withBackend<gonk::MapTemplate>()
    .get();
}
