// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_COMMONS_TYPES_H
#define GONK_COMMONS_TYPES_H

#include <script/string.h>
#include <script/types.h>

namespace script
{

template<typename T>
struct make_return_type_t
{
  inline static script::Type get()
  {
    return make_type_helper<T>::get();
  }
};

template<typename T>
struct make_return_type_t<const T>
{
  inline static script::Type get()
  {
    return make_type_helper<T>::get().withFlag(script::Type::ConstFlag);
  }
};

template<typename T>
struct make_return_type_t<T&>
{

};

template<typename T>
struct make_return_type_t<const T&>
{
  inline static script::Type get()
  {
    return make_return_type_t<T>::get();
  }
};

template<typename T>
script::Type make_return_type()
{
  return make_return_type_t<T>::get();
}

/// TODO 

template<> struct make_type_helper<short> { inline static script::Type get() { return script::Type::Int; } };

} // namespace script


#endif // GONK_COMMONS_TYPES_H
