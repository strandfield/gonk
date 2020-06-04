// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_COMMONS_DEPENDENT_FALSE_H
#define GONK_COMMONS_DEPENDENT_FALSE_H

#include "gonk/gonk-defs.h"

#include <type_traits>

namespace gonk
{

template <class T>
struct dependent_false : std::false_type {};

} // namespace gonk

#endif // GONK_COMMONS_DEPENDENT_FALSE_H
