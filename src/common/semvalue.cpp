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
  auto resol = script::OverloadResolution::New(e);
  if (!resol.process(funcs, { script::Type::cref(info.type) }))
    return;

  if (resol.selectedOverload().returnType() != script::Type::Int)
    return;
  else if (resol.selectedOverload().parameter(0) != script::Type::cref(info.type))
    return;

  info.hash = resol.selectedOverload();
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
    auto creftype = script::Type::cref(t);
    auto reftype = script::Type::ref(t);

    {
      std::vector<script::Function> ops = script::NameLookup::resolve(script::EqualOperator, creftype, creftype, script::Scope{ e->rootNamespace() });
      auto resol = script::OverloadResolution::New(e);
      if (!resol.process(ops, { creftype, creftype }))
        throw std::runtime_error{ "TypeInfo::get(): type must be equality-comparable" };

      ret->eq = resol.selectedOverload();
      if (!check_op_eq(t.baseType(), ret->eq))
        throw std::runtime_error{ "TypeInfo::get(): invalid operator==" };
    }

    {
      std::vector<script::Function> ops = script::NameLookup::resolve(script::AssignmentOperator, reftype, creftype, script::Scope{ e->rootNamespace() });
      auto resol = script::OverloadResolution::New(e);
      if (!resol.process(ops, { reftype, creftype }))
        throw std::runtime_error{ "TypeInfo::get(): type must be assignable" };

      ret->assign = resol.selectedOverload();
      if (!check_op_assign(t.baseType(), ret->assign))
        throw std::runtime_error{ "TypeInfo::get(): invalid operator=" };
    }

    {
      std::vector<script::Function> ops = script::NameLookup::resolve(script::LessOperator, creftype, creftype, script::Scope{ e->rootNamespace() });
      auto resol = script::OverloadResolution::New(e);
      if (!resol.process(ops, { creftype, creftype }))
        throw std::runtime_error{ "TypeInfo::get(): type must have operator<" };

      ret->less = resol.selectedOverload();
      if (!check_op_less(t.baseType(), ret->less))
        throw std::runtime_error{ "TypeInfo::get(): invalid operator<" };
    }

    get_hash(*ret);
  }
  else
  {
    throw std::runtime_error{ "Not implemented" };
  }

  typeinfomap[t.baseType().data()] = ret;
  return ret;
}

std::shared_ptr<TypeInfo> TypeInfo::get(const script::Class & cla)
{
  return std::static_pointer_cast<TypeInfo>(cla.data());
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
