// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_WRAPPERS_ENUM_WRAPPERS_H
#define GONK_WRAPPERS_ENUM_WRAPPERS_H

#include "gonk/common/values.h"

#include <script/interpreter/executioncontext.h>

namespace gonk
{

namespace wrapper
{

template<typename T>
script::Value enum_assignment(script::FunctionCall* c)
{
  script::get<T>(c->arg(0)) = script::get<T>(c->arg(1));
  return c->arg(0);
}

template<typename T>
script::Value enum_copy(script::FunctionCall* c)
{
  return script::Value(c->engine()->construct<T>(gonk::value_cast<T>(c->arg(0))));
}

template<typename T>
script::Value enum_from_int(script::FunctionCall* c)
{
  return script::Value(c->engine()->construct<T>(static_cast<T>(c->arg(0).toInt())));
}

} // namespace wrapper

} // namespace gonk

#endif // GONK_WRAPPERS_ENUM_WRAPPERS_H
