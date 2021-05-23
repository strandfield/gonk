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

namespace gonk
{

namespace std_inttypes
{

int class_type_id_offset_value = 0;
int enum_type_id_offset_value = 0;

int class_type_id_offset()
{
  return class_type_id_offset_value;
}

int enum_type_id_offset()
{
  return enum_type_id_offset_value;
}

} // namespace std_inttypes

} // namespace gonk


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
  script::Class c = ns.newClass(name).get();

  c.newConstructor(callbacks::ctor_default<T>).create();

  c.newConstructor(callbacks::ctor_int<T>)
    .setExplicit()
    .params(gonk::make_type<const int&>()).create();

  gonk::bind::constructor<T, const T&>(c).create();
  gonk::bind::destructor<T>(c).create();

  c.newOperator(script::AssignmentOperator, callbacks::assign<T>)
    .returns(gonk::make_type<T&>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::AdditionOperator, callbacks::add<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::SubstractionOperator, callbacks::sub<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::MultiplicationOperator, callbacks::mul<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::DivisionOperator, callbacks::div<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::RemainderOperator, callbacks::mod<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::AdditionAssignmentOperator, callbacks::add_assign<T>)
    .returns(gonk::make_type<T&>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::SubstractionAssignmentOperator , callbacks::sub_assign<T>)
    .returns(gonk::make_type<T&>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::MultiplicationAssignmentOperator, callbacks::mul_assign<T>)
    .returns(gonk::make_type<T&>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::DivisionAssignmentOperator, callbacks::div_assign<T>)
    .returns(gonk::make_type<T&>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::RemainderAssignmentOperator, callbacks::mod_assign<T>)
    .returns(gonk::make_type<T&>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::LeftShiftOperator, callbacks::lshift<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::RightShiftOperator, callbacks::rshift<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::LeftShiftAssignmentOperator, callbacks::lshift_assign<T>)
    .returns(gonk::make_type<T&>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::RightShiftAssignmentOperator, callbacks::rshift_assign<T>)
    .returns(gonk::make_type<T&>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::EqualOperator, callbacks::equal<T>)
    .setConst()
    .returns(gonk::make_type<bool>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::InequalOperator, callbacks::inequal<T>)
    .setConst()
    .returns(gonk::make_type<bool>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::LessOperator, callbacks::less<T>)
    .setConst()
    .returns(gonk::make_type<bool>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::GreaterOperator, callbacks::greater<T>)
    .setConst()
    .returns(gonk::make_type<bool>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::LessEqualOperator, callbacks::leq<T>)
    .setConst()
    .returns(gonk::make_type<bool>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::GreaterEqualOperator, callbacks::geq<T>)
    .setConst()
    .returns(gonk::make_type<bool>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::UnaryPlusOperator, callbacks::unaryplus<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .create();

  c.newOperator(script::UnaryMinusOperator, callbacks::unaryminus<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .create();

  c.newOperator(script::BitwiseOrOperator, callbacks::bitwise_or<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::BitwiseAndOperator, callbacks::bitwise_and<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::BitwiseXorOperator, callbacks::bitxor<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::BitwiseAndAssignmentOperator, callbacks::bitand_assign<T>)
    .returns(gonk::make_type<T&>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::BitwiseOrAssignmentOperator, callbacks::bitor_assign<T>)
    .returns(gonk::make_type<T&>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::BitwiseXorAssignmentOperator, callbacks::bitxor_assign<T>)
    .returns(gonk::make_type<T&>())
    .params(gonk::make_type<const T&>())
    .create();

  c.newOperator(script::BitwiseNot, callbacks::bitnot<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .create();

  c.newOperator(script::PostDecrementOperator, callbacks::postdecr<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .create();

  c.newOperator(script::PostIncrementOperator, callbacks::postincr<T>)
    .setConst()
    .returns(gonk::make_type<T>())
    .create();

  c.newOperator(script::PreDecrementOperator, callbacks::predecr<T>)
    .returns(gonk::make_type<T&>())
    .create();

  c.newOperator(script::PreIncrementOperator, callbacks::preincr<T>)
    .returns(gonk::make_type<T&>())
    .create();

  c.newConversion(script::Type::Int, callbacks::operator_int<T>)
    .setConst()
    .create();

  script::FunctionBuilder(c, "__gonk_repr__").setCallback(callbacks::to_string<T>)
    .setConst()
    .returns(script::Type::String)
    .create();
}

static void register_inttypes(script::Namespace& ns)
{
#if defined(GONK_INT32_IS_INT)
  script::Symbol{ ns }.newTypedef(script::Type::Int, "int32_t").create();
  register_int_type<int64_t>(ns, "int64_t");
#else
  register_int_type<int32_t>(ns, "int32_t");
  script::Symbol{ ns }.newTypedef(script::Type::Int, "int64_t").create();
#endif
  register_int_type<uint32_t>(ns, "uint32_t");
  register_int_type<uint64_t>(ns, "uint64_t");
}

class StdInttypesPlugin : public gonk::Plugin
{
public:

  void load(script::Module m) override
  {
    script::Engine* e = m.engine();

    gonk::std_inttypes::class_type_id_offset_value = static_cast<int>(e->typeSystem()->reserve(script::Type::ObjectFlag, 1));

    script::Namespace ns = m.root().getNamespace("std");

    register_inttypes(ns);

    ns = m.root();

#if defined(GONK_INT32_IS_INT)
    script::Symbol{ ns }.newTypedef(script::Type::Int, "int32_t").create();
    script::Symbol{ ns }.newTypedef(gonk::make_type<int64_t>(), "int64_t").create();
#else
    script::Symbol{ ns }.newTypedef(gonk::make_type<int32_t>(), "int32_t").create();
    script::Symbol{ ns }.newTypedef(script::Type::Int, "int64_t").create();
#endif

    script::Symbol{ ns }.newTypedef(gonk::make_type<uint32_t>(), "uint32_t").create();
    script::Symbol{ ns }.newTypedef(gonk::make_type<uint64_t>(), "uint64_t").create();
  }

  void unload(script::Module m) override
  {

  }
};

gonk::Plugin* gonk_std_inttypes_module()
{
  return new StdInttypesPlugin();
}
