// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BINDING_DESTRUCTOR_H
#define GONK_BINDING_DESTRUCTOR_H

#include "gonk/common/values.h"

#include "gonk/common/binding/destructor_binder.h"

#include <script/class.h>
#include <script/destructorbuilder.h>

namespace gonk
{

namespace bind
{

template<typename T>
script::DestructorBuilder destructor(script::Class& c)
{
  return c.newDestructor(destructor_binder<T>::destructor);
}

} // namespace bind

} // namespace gonk

#endif // GONK_BINDING_DESTRUCTOR_H
