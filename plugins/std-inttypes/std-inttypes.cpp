// Copyright (C) 2020-2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "std-inttypes.h"

#include "gonk/plugin.h"

#include "gonk/common/binding/class.h"

#include <script/engine.h>
#include <script/namespace.h>
#include <script/typesystem.h>
#include <script/classbuilder.h>
#include <script/symbol.h>
#include <script/typedefs.h>
#include <script/interpreter/executioncontext.h>

namespace callbacks
{

template<typename T>
script::Value ctor_default(script::FunctionCall* c)
{
  return c->engine()->construct<T>(T(0));
}

template<typename T>
script::Value ctor_int(script::FunctionCall* c)
{
  return c->engine()->construct<T>(static_cast<T>(script::get<int>(c->arg(1))));
}

template<typename T>
script::Value assign(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) = script::get<T>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value add_assign(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) += script::get<T>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value sub_assign(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) -= script::get<T>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value mul_assign(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) *= script::get<T>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value div_assign(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) /= script::get<T>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value mod_assign(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) %= script::get<T>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value lshift_assign(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) <<= script::get<T>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value rshift_assign(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) >>= script::get<T>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value add(script::FunctionCall* c)
{
  return c->engine()->construct<T>(script::get<T>(c->arg(0)) + script::get<T>(c->arg(1)));
}

template<typename T>
script::Value sub(script::FunctionCall* c)
{
  return c->engine()->construct<T>(script::get<T>(c->arg(0)) - script::get<T>(c->arg(1)));
}

template<typename T>
script::Value mul(script::FunctionCall* c)
{
  return c->engine()->construct<T>(script::get<T>(c->arg(0)) * script::get<T>(c->arg(1)));
}

template<typename T>
script::Value div(script::FunctionCall* c)
{
  return c->engine()->construct<T>(script::get<T>(c->arg(0)) / script::get<T>(c->arg(1)));
}

template<typename T>
script::Value mod(script::FunctionCall* c)
{
  return c->engine()->construct<T>(script::get<T>(c->arg(0)) % script::get<T>(c->arg(1)));
}

template<typename T>
script::Value equal(script::FunctionCall* c)
{
  return c->engine()->newBool(script::get<T>(c->arg(0)) == script::get<T>(c->arg(1)));
}

template<typename T>
script::Value inequal(script::FunctionCall* c)
{
  return c->engine()->newBool(script::get<T>(c->arg(0)) != script::get<T>(c->arg(1)));
}

template<typename T>
script::Value greater(script::FunctionCall* c)
{
  return c->engine()->newBool(script::get<T>(c->arg(0)) > script::get<T>(c->arg(1)));
}

template<typename T>
script::Value less(script::FunctionCall* c)
{
  return c->engine()->newBool(script::get<T>(c->arg(0)) < script::get<T>(c->arg(1)));
}

template<typename T>
script::Value geq(script::FunctionCall* c)
{
  return c->engine()->newBool(script::get<T>(c->arg(0)) >= script::get<T>(c->arg(1)));
}

template<typename T>
script::Value leq(script::FunctionCall* c)
{
  return c->engine()->newBool(script::get<T>(c->arg(0)) <= script::get<T>(c->arg(1)));
}

template<typename T>
script::Value lshift(script::FunctionCall* c)
{
  return c->engine()->construct<T>(script::get<T>(c->arg(0)) << script::get<T>(c->arg(1)));
}

template<typename T>
script::Value rshift(script::FunctionCall* c)
{
  return c->engine()->construct<T>(script::get<T>(c->arg(0)) >> script::get<T>(c->arg(1)));
}

template<typename T>
script::Value preincr(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) += T(1);
  return c->arg(0);
}

template<typename T>
script::Value predecr(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) -= T(1);
  return c->arg(0);
}

template<typename T>
script::Value postincr(script::FunctionCall* c)
{
  script::Value ret = c->engine()->copy(c->arg(0));
  script::get<T>(c->arg(0)) += T(1);
  return ret;
}

template<typename T>
script::Value postdecr(script::FunctionCall* c)
{
  script::Value ret = c->engine()->copy(c->arg(0));
  script::get<T>(c->arg(0)) -= T(1);
  return ret;
}

template<typename T>
script::Value unaryplus(script::FunctionCall* c)
{
  return c->engine()->copy(c->arg(0));
}

template<typename T>
script::Value unaryminus(script::FunctionCall* c)
{
  return c->engine()->construct<T>(-(script::get<T>(c->arg(0))));
}

template<typename T>
script::Value bitwise_and(script::FunctionCall* c)
{
  return c->engine()->construct<T>(script::get<T>(c->arg(0)) & script::get<T>(c->arg(1)));
}

template<typename T>
script::Value bitwise_or(script::FunctionCall* c)
{
  return c->engine()->construct<T>(script::get<T>(c->arg(0)) | script::get<T>(c->arg(1)));
}

template<typename T>
script::Value bitxor(script::FunctionCall* c)
{
  return c->engine()->construct<T>(script::get<T>(c->arg(0)) ^ script::get<T>(c->arg(1)));
}

template<typename T>
script::Value bitnot(script::FunctionCall* c)
{
  return c->engine()->construct<T>(~(script::get<T>(c->arg(0))));
}

template<typename T>
script::Value bitand_assign(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) &= script::get<T>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value bitor_assign(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) |= script::get<T>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value bitxor_assign(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) ^= script::get<T>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value operator_int(script::FunctionCall* c)
{
  return c->engine()->newInt(static_cast<int>(script::get<T>(c->arg(0))));
}

template<typename T>
script::Value to_string(script::FunctionCall* c)
{
  return c->engine()->newString(std::to_string(script::get<T>(c->arg(0))));
}

} // namespace callbacks

template<typename T>
static void register_int_type(script::Namespace& ns, std::string name)
{
  script::Engine* e = ns.engine();
  script::Type t = e->registerType<T>(typeid(T).name(), script::Type::ObjectFlag);
  script::Class c = ns.newClass(name).setId(t.data()).get();

  script::FunctionBuilder::Constructor(c).setCallback(callbacks::ctor_default<T>).create();

  script::FunctionBuilder::Constructor(c).setCallback(callbacks::ctor_int<T>)
    .setExplicit()
    .params(gonk::make_type<const int&>(e)).create();

  gonk::bind::constructor<T, const T&>(c).create();
  gonk::bind::destructor<T>(c).create();

  script::FunctionBuilder::Op(c, script::AssignmentOperator).setCallback(callbacks::assign<T>)
    .returns(gonk::make_type<T&>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::AdditionOperator).setCallback(callbacks::add<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::SubstractionOperator).setCallback(callbacks::sub<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::MultiplicationOperator).setCallback(callbacks::mul<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::DivisionOperator).setCallback(callbacks::div<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::RemainderOperator).setCallback(callbacks::mod<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::AdditionAssignmentOperator).setCallback(callbacks::add_assign<T>)
    .returns(gonk::make_type<T&>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::SubstractionAssignmentOperator).setCallback(callbacks::sub_assign<T>)
    .returns(gonk::make_type<T&>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::MultiplicationAssignmentOperator).setCallback(callbacks::mul_assign<T>)
    .returns(gonk::make_type<T&>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::DivisionAssignmentOperator).setCallback(callbacks::div_assign<T>)
    .returns(gonk::make_type<T&>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::RemainderAssignmentOperator).setCallback(callbacks::mod_assign<T>)
    .returns(gonk::make_type<T&>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::LeftShiftOperator).setCallback(callbacks::lshift<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::RightShiftOperator).setCallback(callbacks::rshift<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::LeftShiftAssignmentOperator).setCallback(callbacks::lshift_assign<T>)
    .returns(gonk::make_type<T&>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::RightShiftAssignmentOperator).setCallback(callbacks::rshift_assign<T>)
    .returns(gonk::make_type<T&>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::EqualOperator).setCallback(callbacks::equal<T>)
    .setConst()
    .returns(gonk::make_type<bool>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::InequalOperator).setCallback(callbacks::inequal<T>)
    .setConst()
    .returns(gonk::make_type<bool>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::LessOperator).setCallback(callbacks::less<T>)
    .setConst()
    .returns(gonk::make_type<bool>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::GreaterOperator).setCallback(callbacks::greater<T>)
    .setConst()
    .returns(gonk::make_type<bool>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::LessEqualOperator).setCallback(callbacks::leq<T>)
    .setConst()
    .returns(gonk::make_type<bool>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::GreaterEqualOperator).setCallback(callbacks::geq<T>)
    .setConst()
    .returns(gonk::make_type<bool>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::UnaryPlusOperator).setCallback(callbacks::unaryplus<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .create();

  script::FunctionBuilder::Op(c, script::UnaryMinusOperator).setCallback(callbacks::unaryminus<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .create();

  script::FunctionBuilder::Op(c, script::BitwiseOrOperator).setCallback(callbacks::bitwise_or<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::BitwiseAndOperator).setCallback(callbacks::bitwise_and<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::BitwiseXorOperator).setCallback(callbacks::bitxor<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::BitwiseAndAssignmentOperator).setCallback(callbacks::bitand_assign<T>)
    .returns(gonk::make_type<T&>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::BitwiseOrAssignmentOperator).setCallback(callbacks::bitor_assign<T>)
    .returns(gonk::make_type<T&>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::BitwiseXorAssignmentOperator).setCallback(callbacks::bitxor_assign<T>)
    .returns(gonk::make_type<T&>(e))
    .params(gonk::make_type<const T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::BitwiseNot).setCallback(callbacks::bitnot<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .create();

  script::FunctionBuilder::Op(c, script::PostDecrementOperator).setCallback(callbacks::postdecr<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .create();

  script::FunctionBuilder::Op(c, script::PostIncrementOperator).setCallback(callbacks::postincr<T>)
    .setConst()
    .returns(gonk::make_type<T>(e))
    .create();

  script::FunctionBuilder::Op(c, script::PreDecrementOperator).setCallback(callbacks::predecr<T>)
    .returns(gonk::make_type<T&>(e))
    .create();

  script::FunctionBuilder::Op(c, script::PreIncrementOperator).setCallback(callbacks::preincr<T>)
    .returns(gonk::make_type<T&>(e))
    .create();

  script::FunctionBuilder::Cast(c).setCallback(callbacks::operator_int<T>)
    .setReturnType(script::Type::Int)
    .setConst()
    .create();

  script::FunctionBuilder::Fun(c, "__gonk_repr__").setCallback(callbacks::to_string<T>)
    .setConst()
    .returns(script::Type::String)
    .create();
}

static void register_inttypes(script::Namespace& ns)
{
  if constexpr (std::is_same<std::int32_t, int>::value)
  {
    script::Symbol{ ns }.newTypedef(script::Type::Int, "int32_t").create();
    register_int_type<int64_t>(ns, "int64_t");
  }
  else
  {
    register_int_type<int32_t>(ns, "int32_t");
    script::Symbol{ ns }.newTypedef(script::Type::Int, "int64_t").create();
  }

  register_int_type<uint32_t>(ns, "uint32_t");
  register_int_type<uint64_t>(ns, "uint64_t");
}

class StdInttypesPlugin : public gonk::Plugin
{
public:

  void load(script::Module m) override
  {
    script::Engine* e = m.engine();

    script::Namespace ns = m.root().getNamespace("std");

    register_inttypes(ns);

    ns = m.root();

    if constexpr (std::is_same<std::int32_t, int>::value)
    {
      script::Symbol{ ns }.newTypedef(script::Type::Int, "int32_t").create();
      script::Symbol{ ns }.newTypedef(gonk::make_type<int64_t>(e), "int64_t").create();
    }
    else
    {
      script::Symbol{ ns }.newTypedef(gonk::make_type<int32_t>(e), "int32_t").create();
      script::Symbol{ ns }.newTypedef(script::Type::Int, "int64_t").create();
    }

    script::Symbol{ ns }.newTypedef(gonk::make_type<uint32_t>(e), "uint32_t").create();
    script::Symbol{ ns }.newTypedef(gonk::make_type<uint64_t>(e), "uint64_t").create();
  }

  void unload(script::Module m) override
  {

  }
};

gonk::Plugin* gonk_std_inttypes_module()
{
  return new StdInttypesPlugin();
}
