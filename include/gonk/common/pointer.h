// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_COMMONS_POINTER_H
#define GONK_COMMONS_POINTER_H

#include "gonk/gonk-defs.h"

#include <script/engine.h>

namespace gonk
{

template<typename T>
class Pointer
{
public:
  T* ptr = nullptr;

public:
  Pointer(T& val)
    : ptr(&val)
  {

  }

  Pointer(T* val)
    : ptr(val)
  {

  }

  Pointer() = default;
  Pointer(const Pointer<T>&) = default;
  ~Pointer() = default;

  Pointer<T>& operator=(const Pointer<T>&) = default;

  operator T* () const
  {
    return this->ptr;
  }

  operator T& () const
  {
    if (!this->ptr)
      throw script::RuntimeError{"bad pointer access"};

    return *(this->ptr);
  }
};

} // namespace gonk

#endif // GONK_COMMONS_POINTER_H
