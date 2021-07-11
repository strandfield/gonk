// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/common/semvalue.h"

#include <script/class.h>
#include <script/engine.h>
#include <script/initialization.h>
#include <script/namelookup.h>
#include <script/namespace.h>
#include <script/overloadresolution.h>

namespace gonk
{

static std::map<int, std::shared_ptr<TypeInfo>> & get_typeinfo_map()
{
  static std::map<int, std::shared_ptr<TypeInfo>> map = {};
  return map;
}

static bool check_op_eq(const script::Type & t, const script::Function & op)
{
  if (op.returnType().baseType() != script::Type::Boolean)
    return false;

  if (op.parameter(0) != script::Type::cref(t) || op.parameter(1) != script::Type::cref(t))
    return false;

  return true;
}

static bool check_op_assign(const script::Type & t, const script::Function & op)
{
  if (op.returnType() != script::Type::ref(t))
    return false;

  if (op.parameter(0) != script::Type::ref(t) || op.parameter(1) != script::Type::cref(t))
    return false;

  return true;
}

static bool check_op_less(const script::Type & t, const script::Function & op)
{
  if (op.returnType() != script::Type::Boolean)
    return false;

  if (op.parameter(0) != script::Type::cref(t) || op.parameter(1) != script::Type::cref(t))
    return false;

  return true;
}

static void get_hash(TypeInfo & info)
{
  script::Engine *e = info.engine;

  script::Scope scp{ script::Scope::enclosingNamespace(info.type, e) };
  std::vector<script::Function> funcs = script::NameLookup::resolve("hash", scp).functions();
  auto resol = script::resolve_overloads(funcs, std::vector<script::Type>{script::Type::cref(info.type)});
  if (!resol)
    return;

  if (resol.function.returnType() != script::Type::Int)
    return;
  else if (resol.function.parameter(0) != script::Type::cref(info.type))
    return;

  info.hash = resol.function;
}

script::Function TypeInfo::get_eq(script::Engine* e, const script::Type& t)
{
  auto creftype = script::Type::cref(t);

  std::vector<script::Function> ops = script::NameLookup::resolve(script::EqualOperator, creftype, creftype, script::Scope{ e->rootNamespace() });
  auto resol = script::resolve_overloads(ops, std::vector<script::Type>{ creftype, creftype });
  if (!resol)
    throw std::runtime_error{ "TypeInfo::get(): type must be equality-comparable" };

  if (!check_op_eq(t.baseType(), resol.function))
    throw std::runtime_error{ "TypeInfo::get(): invalid operator==" };

  return resol.function;
}

script::Function TypeInfo::get_assign(script::Engine* e, const script::Type& t)
{
  auto creftype = script::Type::cref(t);
  auto reftype = script::Type::ref(t);

  std::vector<script::Function> ops = script::NameLookup::resolve(script::AssignmentOperator, reftype, creftype, script::Scope{ e->rootNamespace() });
  auto resol = script::resolve_overloads(ops, std::vector<script::Type>{ reftype, creftype });
  if (!resol)
    throw std::runtime_error{ "TypeInfo::get(): type must be assignable" };

  if (!check_op_assign(t.baseType(), resol.function))
    throw std::runtime_error{ "TypeInfo::get(): invalid operator=" };

  return resol.function;
}

script::Function TypeInfo::get_less(script::Engine* e, const script::Type& t)
{
  auto creftype = script::Type::cref(t);

  std::vector<script::Function> ops = script::NameLookup::resolve(script::LessOperator, creftype, creftype, script::Scope{ e->rootNamespace() });
  auto resol = script::resolve_overloads(ops, std::vector<script::Type>{ creftype, creftype });
  if (!resol)
    throw std::runtime_error{ "TypeInfo::get(): type must have operator<" };

  if (!check_op_less(t.baseType(), resol.function))
    throw std::runtime_error{ "TypeInfo::get(): invalid operator<" };

  return resol.function;
}

std::shared_ptr<TypeInfo> TypeInfo::get(script::Engine *e, const script::Type & t)
{
  auto & typeinfomap = get_typeinfo_map();

  auto it = typeinfomap.find(t.baseType().data());
  if (it != typeinfomap.end())
    return it->second;

  std::shared_ptr<TypeInfo> ret = std::make_shared<TypeInfo>();
  ret->type = t.baseType();
  ret->engine = e;

  if (t.isObjectType() || t.isFundamentalType())
  {
    ret->eq = get_eq(e, t);
    ret->assign = get_assign(e, t);
    ret->less = get_less(e, t);

    get_hash(*ret);
  }
  else
  {
    throw std::runtime_error{ "Not implemented" };
  }

  typeinfomap[t.baseType().data()] = ret;
  return ret;
}

SemValue TypeInfo::defaultConstruct() const
{
  const std::vector<script::Value> args;
  return SemValue(this->engine->construct(this->type, args));
}

SemValue::SemValue()
  : typeinfo_(nullptr)
{

}

SemValue::SemValue(const SemValue& other)
  : typeinfo_(other.typeinfo_)
{
  if (other.isValid())
    value_ = engine()->copy(other.value_);
}

SemValue::SemValue(SemValue&& other)
  : typeinfo_(other.typeinfo_)
  , value_(other.value_)
{
  other.typeinfo_ = nullptr;
  other.value_ = script::Value{};
}

SemValue::SemValue(const std::shared_ptr<TypeInfo> & ti, const script::Value & val)
  : typeinfo_(ti)
{
  value_ = engine()->copy(val);
}

SemValue::SemValue(const std::shared_ptr<TypeInfo> & ti, script::Value && val)
  : typeinfo_(ti)
  , value_(val)
{

}

SemValue::SemValue(const script::Value & val)
{
  typeinfo_ = TypeInfo::get(val.engine(), val.type());
  value_ = engine()->copy(val);
}

SemValue::SemValue(script::Value && val)
  : value_(val)
{
  typeinfo_ = TypeInfo::get(val.engine(), val.type());
}

SemValue::~SemValue()
{
  if (isValid())
    engine()->destroy(value_);
  value_ = script::Value{};
}

script::Value SemValue::release()
{
  typeinfo_ = nullptr;
  script::Value ret = value_;
  value_ = script::Value{};
  return ret;
}

void SemValue::assign(const script::Value & v)
{
  assert(isValid());
  assert(typeinfo_->type == v.type());

  typeinfo_->assign.invoke({ value_, v });
}

int SemValue::hash() const
{
  script::Value result = typeinfo_->hash.invoke({ value_ });
  int ret = result.toInt();
  engine()->destroy(result);
  return ret;
}

SemValue& SemValue::operator=(const SemValue& other)
{
  if (value_ == other.value_)
    return *(this);

  if (isValid())
    engine()->destroy(value_);

  typeinfo_ = other.typeinfo_;
  if(isValid())
    value_ = engine()->copy(other.value_);

  return *(this);
}

SemValue& SemValue::operator=(SemValue&& other)
{
  typeinfo_ = other.typeinfo_;
  value_ = other.value_;

  other.typeinfo_ = nullptr;
  other.value_ = script::Value{};

  return *this;
}

bool SemValue::operator==(const SemValue& other) const
{
  if (other.isNull() && isNull())
    return true;

  if (other.isNull() != isNull())
    return false;

  auto ret = typeinfo_->eq.invoke({ value_, other.value_ });
  bool result = ret.toBool();
  engine()->destroy(ret);
  return result;
}

bool SemValue::operator!=(const SemValue& other) const
{
  return !(*this == other);
}

bool SemValue::operator<(const SemValue& other) const
{
  assert(!typeinfo_->less.isNull());

  script::Engine *e = typeinfo_->engine;
  script::Value ret = typeinfo_->less.invoke({ value_, other.value_ });
  bool result = ret.toBool();
  e->destroy(ret);
  return result;
}

ObserverValue::ObserverValue(const std::shared_ptr<TypeInfo> & ti, const script::Value & val)
  : SemValue()
{
  typeinfo_ = ti;
  value_ = val;
}

ObserverValue::~ObserverValue()
{
  release();
}

} // namespace gonk
