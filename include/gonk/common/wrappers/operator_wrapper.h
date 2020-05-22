// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_WRAPPERS_OPERATOR_WRAPPER_H
#define GONK_WRAPPERS_OPERATOR_WRAPPER_H

#include "gonk/common/values.h"

#include <script/interpreter/executioncontext.h>

namespace script
{

namespace wrapper
{

template<typename LHS, typename RHS>
script::Value add_wrapper(script::FunctionCall *c)
{
  return make_value(value_cast<LHS>(c->arg(0)) + value_cast<RHS>(c->arg(1)), c->engine());
}

template<typename LHS, typename RHS>
script::Value sub_wrapper(script::FunctionCall *c)
{
  return make_value(value_cast<LHS>(c->arg(0)) - value_cast<RHS>(c->arg(1)), c->engine());
}

template<typename LHS, typename RHS>
script::Value mul_wrapper(script::FunctionCall *c)
{
  return make_value(value_cast<LHS>(c->arg(0)) * value_cast<RHS>(c->arg(1)), c->engine());
}

template<typename LHS, typename RHS>
script::Value div_wrapper(script::FunctionCall *c)
{
  return make_value(value_cast<LHS>(c->arg(0)) / value_cast<RHS>(c->arg(1)), c->engine());
}

template<typename LHS, typename RHS>
script::Value assign_wrapper(script::FunctionCall *c)
{
  value_cast<LHS>(c->arg(0)) = value_cast<RHS>(c->arg(1));
  return c->arg(0);
}

template<typename LHS, typename RHS>
script::Value add_assign_wrapper(script::FunctionCall *c)
{
  value_cast<LHS>(c->arg(0)) += value_cast<RHS>(c->arg(1));
  return c->arg(0);
}

template<typename LHS, typename RHS>
script::Value sub_assign_wrapper(script::FunctionCall *c)
{
  value_cast<LHS>(c->arg(0)) -= value_cast<RHS>(c->arg(1));
  return c->arg(0);
}

template<typename LHS, typename RHS>
script::Value mul_assign_wrapper(script::FunctionCall *c)
{
  value_cast<LHS>(c->arg(0)) *= value_cast<RHS>(c->arg(1));
  return c->arg(0);
}

template<typename LHS, typename RHS>
script::Value div_assign_wrapper(script::FunctionCall *c)
{
  value_cast<LHS>(c->arg(0)) /= value_cast<RHS>(c->arg(1));
  return c->arg(0);
}

template<typename LHS, typename RHS>
script::Value eq_wrapper(script::FunctionCall *c)
{
  return c->engine()->newBool(value_cast<LHS>(c->arg(0)) == value_cast<RHS>(c->arg(1)));
}

template<typename LHS, typename RHS>
script::Value neq_wrapper(script::FunctionCall *c)
{
  return c->engine()->newBool(value_cast<LHS>(c->arg(0)) != value_cast<RHS>(c->arg(1)));
}

template<typename LHS, typename RHS>
script::Value less_wrapper(script::FunctionCall *c)
{
  return c->engine()->newBool(value_cast<LHS>(c->arg(0)) < value_cast<RHS>(c->arg(1)));
}

template<typename LHS, typename RHS>
script::Value leq_wrapper(script::FunctionCall *c)
{
  return c->engine()->newBool(value_cast<LHS>(c->arg(0)) <= value_cast<RHS>(c->arg(1)));
}

template<typename LHS, typename RHS>
script::Value greater_wrapper(script::FunctionCall *c)
{
  return c->engine()->newBool(value_cast<LHS>(c->arg(0)) > value_cast<RHS>(c->arg(1)));
}

template<typename LHS, typename RHS>
script::Value geq_wrapper(script::FunctionCall *c)
{
  return c->engine()->newBool(value_cast<LHS>(c->arg(0)) >= value_cast<RHS>(c->arg(1)));
}

template<typename LHS, typename RHS>
script::Value and_wrapper(script::FunctionCall *c)
{
  return make_value(value_cast<LHS>(c->arg(0)) & value_cast<RHS>(c->arg(1)), c->engine());
}

template<typename LHS, typename RHS>
script::Value or_wrapper(script::FunctionCall *c)
{
  return make_value(value_cast<LHS>(c->arg(0)) | value_cast<RHS>(c->arg(1)), c->engine());
}

template<typename LHS, typename RHS>
script::Value xor_wrapper(script::FunctionCall *c)
{
  return make_value(value_cast<LHS>(c->arg(0)) ^ value_cast<RHS>(c->arg(1)), c->engine());
}

template<typename LHS, typename RHS>
script::Value and_assign_wrapper(script::FunctionCall *c)
{
  value_cast<LHS>(c->arg(0)) &= value_cast<RHS>(c->arg(1));
  return c->arg(0);
}

template<typename LHS, typename RHS>
script::Value or_assign_wrapper(script::FunctionCall *c)
{
  value_cast<LHS>(c->arg(0)) |= value_cast<RHS>(c->arg(1));
  return c->arg(0);
}

template<typename LHS, typename RHS>
script::Value xor_assign_wrapper(script::FunctionCall *c)
{
  value_cast<LHS>(c->arg(0)) ^= value_cast<RHS>(c->arg(1));
  return c->arg(0);
}

template<typename ReturnType, typename LHS>
script::Value logical_not_wrapper(script::FunctionCall *c)
{
  return make_value<ReturnType>(!value_cast<LHS>(c->arg(0)), c->engine());
}

template<typename ReturnType, typename LHS, typename RHS>
script::Value subscript_wrapper(script::FunctionCall *c)
{
  return make_value<ReturnType>(value_cast<LHS>(c->arg(0))[value_cast<RHS>(c->arg(1))], c->engine());
}

template<typename ReturnType, typename LHS, typename RHS>
script::Value subscript_proxy_wrapper(script::FunctionCall *c)
{
  return c->engine()->newPtr(make_type<Proxy<typename std::decay<ReturnType>::type>>(), &(value_cast<LHS>(c->arg(0))[value_cast<RHS>(c->arg(1))]));
}

template<typename LHS, typename RHS>
script::Value left_shift_wrapper(script::FunctionCall *c)
{
  return make_value(value_cast<LHS>(c->arg(0)) << value_cast<RHS>(c->arg(1)), c->engine());
}

template<typename LHS, typename RHS>
script::Value right_shift_wrapper(script::FunctionCall *c)
{
  return make_value(value_cast<LHS>(c->arg(0)) >> value_cast<RHS>(c->arg(1)), c->engine());
}

template<typename T>
script::Value unary_plus_wrapper(script::FunctionCall *c)
{
  return make_value(+value_cast<T>(c->arg(0)), c->engine());
}

template<typename T>
script::Value unary_minus_wrapper(script::FunctionCall *c)
{
  return make_value(-value_cast<T>(c->arg(0)), c->engine());
}

template<typename LHS, typename RHS>
script::Value put_to_wrapper(script::FunctionCall *c)
{
  value_cast<LHS>(c->arg(0)) << value_cast<RHS>(c->arg(1));
  return c->arg(0);
}

template<typename LHS, typename RHS>
script::Value read_from_wrapper(script::FunctionCall *c)
{
  value_cast<LHS>(c->arg(0)) >> value_cast<RHS>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value preincr_wrapper(script::FunctionCall *c)
{
  ++(value_cast<T&>(c->arg(0)));
  return c->arg(0);
}

template<typename T>
script::Value predecr_wrapper(script::FunctionCall *c)
{
  --(value_cast<T&>(c->arg(0)));
  return c->arg(0);
}

template<typename T>
script::Value postincr_wrapper(script::FunctionCall *c)
{
  return make_value((value_cast<T&>(c->arg(0)))++, c->engine());
}

template<typename T>
script::Value postdecr_wrapper(script::FunctionCall *c)
{
  return make_value((value_cast<T&>(c->arg(0)))--, c->engine());
}

} // namespace wrapper

} // namespace script

#endif // GONK_WRAPPERS_OPERATOR_WRAPPER_H
