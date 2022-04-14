// Copyright (C) 2020-2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_OPERATORS_H
#define GONK_BINDING_OPERATORS_H

#include "gonk/common/values.h"

#include "gonk/common/wrappers/operator_wrapper.h"

#include <script/class.h>
#include <script/namespace.h>
#include <script/operator.h>
#include <script/functionbuilder.h>

namespace gonk
{

namespace bind
{

/* Non-member operators */

template<typename ReturnType, typename LHS, typename RHS>
script::Function op_add(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::AdditionOperator).setCallback(wrapper::add_wrapper<LHS, RHS>)
    .returns(make_type<ReturnType>(e))
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename ReturnType, typename LHS, typename RHS>
script::Function op_sub(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::SubstractionOperator).setCallback(wrapper::sub_wrapper<LHS, RHS>)
    .returns(make_type<ReturnType>(e))
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename ReturnType, typename LHS, typename RHS>
script::Function op_mul(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::MultiplicationOperator).setCallback(wrapper::mul_wrapper<LHS, RHS>)
    .returns(make_type<ReturnType>(e))
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename ReturnType, typename LHS, typename RHS>
script::Function op_div(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::DivisionOperator).setCallback(wrapper::div_wrapper<LHS, RHS>)
    .returns(make_type<ReturnType>(e))
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename LHS, typename RHS>
script::Function op_eq(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::EqualOperator).setCallback(wrapper::eq_wrapper<LHS, RHS>)
    .returns(script::Type::Boolean)
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename LHS, typename RHS>
script::Function op_neq(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::InequalOperator).setCallback(wrapper::neq_wrapper<LHS, RHS>)
    .returns(script::Type::Boolean)
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename LHS, typename RHS>
script::Function op_less(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::LessOperator).setCallback(wrapper::less_wrapper<LHS, RHS>)
    .returns(script::Type::Boolean)
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename LHS, typename RHS>
script::Function op_leq(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::LessEqualOperator).setCallback(wrapper::leq_wrapper<LHS, RHS>)
    .returns(script::Type::Boolean)
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename LHS, typename RHS>
script::Function op_greater(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::GreaterOperator).setCallback(wrapper::greater_wrapper<LHS, RHS>)
    .returns(script::Type::Boolean)
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename LHS, typename RHS>
script::Function op_geq(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::GreaterEqualOperator).setCallback(wrapper::geq_wrapper<LHS, RHS>)
    .returns(script::Type::Boolean)
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename ReturnType, typename LHS, typename RHS>
script::Function op_lshift(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::LeftShiftOperator).setCallback(wrapper::left_shift_wrapper<LHS, RHS>)
    .returns(make_type<ReturnType>(e))
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename ReturnType, typename LHS, typename RHS>
script::Function op_rshift(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::RightShiftOperator).setCallback(wrapper::right_shift_wrapper<LHS, RHS>)
    .returns(make_type<ReturnType>(e))
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename LHS, typename RHS>
script::Function op_put_to(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::LeftShiftOperator).setCallback(wrapper::put_to_wrapper<LHS, RHS>)
    .returns(make_type<LHS>(e))
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename LHS, typename RHS>
script::Function op_read_from(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::RightShiftOperator).setCallback(wrapper::read_from_wrapper<LHS, RHS>)
    .returns(make_type<LHS>(e))
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename ReturnType, typename LHS, typename RHS>
script::Function op_bitor(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::BitwiseOrOperator).setCallback(wrapper::or_wrapper<LHS, RHS>)
    .returns(make_type<ReturnType>(e))
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename ReturnType, typename LHS, typename RHS>
script::Function op_bitand(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::BitwiseAndOperator).setCallback(wrapper::and_wrapper<LHS, RHS>)
    .returns(make_type<ReturnType>(e))
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename LHS, typename RHS>
script::Function op_or_assign(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::BitwiseOrAssignmentOperator).setCallback(wrapper::or_assign_wrapper<LHS, RHS>)
    .returns(make_type<LHS&>(e))
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename LHS, typename RHS>
script::Function op_and_assign(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::BitwiseAndAssignmentOperator).setCallback(wrapper::and_assign_wrapper<LHS, RHS>)
    .returns(make_type<LHS&>(e))
    .params(make_type<LHS>(e), make_type<RHS>(e))
    .get();
}

template<typename ReturnType, typename Arg>
script::Function op_unary_plus(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::UnaryPlusOperator).setCallback(wrapper::unary_plus_wrapper<Arg>)
    .returns(make_type<ReturnType>(e))
    .params(make_type<Arg>(e))
    .get();
}

template<typename ReturnType, typename Arg>
script::Function op_unary_minus(script::Namespace& ns)
{
  script::Engine* e = ns.engine();
  return script::FunctionBuilder::Op(ns, script::UnaryMinusOperator).setCallback(wrapper::unary_minus_wrapper<Arg>)
    .returns(make_type<ReturnType>(e))
    .params(make_type<Arg>(e))
    .get();
}


/* Member operators */

template<typename T, typename RHS>
script::Function memop_assign(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::AssignmentOperator).setCallback(wrapper::assign_wrapper<T&, RHS>)
    .returns(make_type<T&>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename R, typename RHS>
script::Function memop_subscript(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::SubscriptOperator).setCallback(wrapper::subscript_wrapper<R, T&, RHS>)
    .returns(make_type<R>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename R, typename RHS>
script::Function memop_const_subscript(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::SubscriptOperator).setCallback(wrapper::subscript_wrapper<R, const T&, RHS>)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_add_assign(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::AdditionAssignmentOperator).setCallback(wrapper::add_assign_wrapper<T&, RHS>)
    .returns(make_type<T&>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_sub_assign(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::AdditionAssignmentOperator).setCallback(wrapper::sub_assign_wrapper<T&, RHS>)
    .returns(make_type<T&>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_mul_assign(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::MultiplicationAssignmentOperator).setCallback(wrapper::mul_assign_wrapper<T&, RHS>)
    .returns(make_type<T&>(e))
    .params(make_type<RHS>(e))
    .get();
}


template<typename T, typename RHS>
script::Function memop_div_assign(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::DivisionAssignmentOperator).setCallback(wrapper::div_assign_wrapper<T&, RHS>)
    .returns(make_type<T&>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_xor_assign(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::BitwiseXorAssignmentOperator).setCallback(wrapper::xor_assign_wrapper<T&, RHS>)
    .returns(make_type<T&>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename R, typename RHS>
script::Function memop_add(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::AdditionOperator).setCallback(wrapper::add_wrapper<const T &, RHS>)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename R, typename RHS>
script::Function memop_sub(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::SubstractionOperator).setCallback(wrapper::sub_wrapper<const T &, RHS>)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename R, typename RHS>
script::Function memop_mul(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::MultiplicationOperator).setCallback(wrapper::mul_wrapper<const T &, RHS>)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename R, typename RHS>
script::Function memop_div(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::DivisionOperator).setCallback(wrapper::div_wrapper<const T &, RHS>)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_eq(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::EqualOperator).setCallback(wrapper::eq_wrapper<const T &, RHS>)
    .setConst()
    .returns(script::Type::Boolean)
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_neq(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::InequalOperator).setCallback(wrapper::neq_wrapper<const T &, RHS>)
    .setConst()
    .returns(script::Type::Boolean)
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_less(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::LessOperator).setCallback(wrapper::less_wrapper<const T&, RHS>)
    .setConst()
    .returns(script::Type::Boolean)
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_leq(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::LessEqualOperator).setCallback(wrapper::leq_wrapper<const T &, RHS>)
    .setConst()
    .returns(script::Type::Boolean)
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_greater(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::GreaterOperator).setCallback(wrapper::greater_wrapper<const T &, RHS>)
    .setConst()
    .returns(script::Type::Boolean)
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_geq(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::GreaterEqualOperator).setCallback(wrapper::geq_wrapper<const T &, RHS>)
    .setConst()
    .returns(script::Type::Boolean)
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename R, typename RHS>
script::Function memop_lshift(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::LeftShiftOperator).setCallback(wrapper::left_shift_wrapper<const T &, RHS>)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename R, typename RHS>
script::Function memop_rshift(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::RightShiftOperator).setCallback(wrapper::right_shift_wrapper<const T &, RHS>)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_put_to(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::LeftShiftOperator).setCallback(wrapper::put_to_wrapper<T&, RHS>)
    .returns(make_type<T&>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_read_from(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::RightShiftOperator).setCallback(wrapper::read_from_wrapper<T&, RHS>)
    .returns(make_type<T&>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename R, typename RHS>
script::Function memop_bitor(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::BitwiseOrOperator).setCallback(wrapper::or_wrapper<const T&, RHS>)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename R, typename RHS>
script::Function memop_bitand(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::BitwiseAndOperator).setCallback(wrapper::and_wrapper<const T&, RHS>)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename R, typename RHS>
script::Function memop_bitxor(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::BitwiseXorOperator).setCallback(wrapper::xor_wrapper<const T&, RHS>)
    .setConst()
    .returns(make_type<R>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_or_assign(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::BitwiseOrAssignmentOperator).setCallback(wrapper::or_assign_wrapper<T&, RHS>)
    .returns(make_type<T&>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename RHS>
script::Function memop_and_assign(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::BitwiseAndAssignmentOperator).setCallback(wrapper::and_assign_wrapper<T&, RHS>)
    .returns(make_type<T&>(e))
    .params(make_type<RHS>(e))
    .get();
}

template<typename T, typename R>
script::Function memop_unary_plus(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::UnaryPlusOperator).setCallback(wrapper::unary_plus_wrapper<const T&>)
    .returns(make_type<R>(e))
    .get();
}

template<typename T, typename R>
script::Function memop_unary_minus(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::UnaryMinusOperator).setCallback(wrapper::unary_minus_wrapper<const T &>)
    .returns(make_type<R>(e))
    .get();
}

template<typename T>
script::Function memop_preincr(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::PreIncrementOperator).setCallback(wrapper::preincr_wrapper<T>)
    .returns(make_type<T&>(e))
    .get();
}

template<typename T>
script::Function memop_postincr(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::PostIncrementOperator).setCallback(wrapper::postincr_wrapper<T>)
    .returns(make_type<T>(e))
    .get();
}

template<typename T>
script::Function memop_predecr(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::PreDecrementOperator).setCallback(wrapper::predecr_wrapper<T>)
    .returns(make_type<T&>(e))
    .get();
}

template<typename T>
script::Function memop_postdecr(script::Class& cla)
{
  script::Engine* e = cla.engine();
  return script::FunctionBuilder::Op(cla, script::PostDecrementOperator).setCallback(wrapper::postdecr_wrapper<T>)
    .returns(make_type<T>(e))
    .get();
}

} // namespace bind

} // namespace gonk

#endif // GONK_BINDING_OPERATORS_H
