// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_CONVERSION_H
#define GONK_BINDING_CONVERSION_H

#include "gonk/common/values.h"

#include "gonk/common/wrappers/conv-wrapper.h"

#include <script/class.h>
#include <script/cast.h>
#include <script/functionbuilder.h>

namespace gonk
{

namespace bind
{

template<typename T, typename Dst>
void conversion(script::Class& cla)
{
  auto builder = script::FunctionBuilder::Cast(cla).setCallback(gonk::wrapper::conversion<T&, Dst>)
    .returns(make_type<Dst>(cla.engine()));

  if constexpr (std::is_const<T>::value)
    builder.setConst();

  builder.create();
}

} // namespace bind

} // namespace gonk

#endif // GONK_BINDING_CONVERSION_H
