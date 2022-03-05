// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_ENUM_H
#define GONK_BINDING_ENUM_H

#include "gonk/common/wrappers/enum-wrappers.h"

#include <script/class.h>
#include <script/enum.h>
#include <script/enumbuilder.h>
#include <script/namespace.h>

namespace gonk
{

namespace bind
{

template<typename T>
script::EnumBuilder enumeration(script::Namespace& ns, std::string name)
{
  auto result = ns.newEnum(std::move(name));

  try
  {
    result.id = ns.engine()->getType<T>().data();
  }
  catch (const script::UnknownTypeError&)
  {
    result.id = ns.engine()->registerType<T>().data();
  }

  result.assignment_callback = gonk::wrapper::enum_assignment<T>;
  result.copy_callback = gonk::wrapper::enum_copy<T>;
  result.from_int_callback = gonk::wrapper::enum_from_int<T>;
  return result;
}

template<typename T>
script::EnumBuilder enumeration(script::Class& cla, std::string name)
{
  auto result = cla.newEnum(std::move(name));

  try
  {
    result.id = cla.engine()->getType<T>().data();
  }
  catch (const script::UnknownTypeError&)
  {
    result.id = cla.engine()->registerType<T>().data();
  }

  result.assignment_callback = gonk::wrapper::enum_assignment<T>;
  result.copy_callback = gonk::wrapper::enum_copy<T>;
  result.from_int_callback = gonk::wrapper::enum_from_int<T>;
  return result;
}

} // namespace bind

} // namespace gonk

#endif // GONK_BINDING_ENUM_H
