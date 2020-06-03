// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_COMMONS_TYPES_H
#define GONK_COMMONS_TYPES_H

#include <script/string.h>
#include <script/types.h>

namespace gonk
{

template<typename T>
script::Type make_type()
{
  return script::make_type<T>();
}

} // namespace gonk

#endif // GONK_COMMONS_TYPES_H
