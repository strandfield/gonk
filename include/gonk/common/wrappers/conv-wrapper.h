// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_WRAPPERS_CONVERSION_WRAPPER_H
#define GONK_WRAPPERS_CONVERSION_WRAPPER_H

#include "gonk/common/values.h"

#include <script/interpreter/executioncontext.h>

namespace gonk
{

namespace wrapper
{

template<typename Src, typename Dst>
script::Value conversion(script::FunctionCall *c)
{
  using namespace script;
  return make_value_perfect<Dst>(value_cast<Src>(c->arg(0)), c->engine());
}

} // namespace wrapper

} // namespace gonk

#endif // GONK_WRAPPERS_CONVERSION_WRAPPER_H
