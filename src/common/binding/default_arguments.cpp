// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/common/binding/default_arguments.h"

#include <script/program/expression.h>

namespace script
{

namespace bind
{

std::shared_ptr<script::program::Expression> make_default_argument(script::Value val)
{
  val.engine()->manage(val);
  return script::program::VariableAccess::New(val);
}

} // namespace bind

} // namespace script
