// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_STD_INTTYPES_H
#define GONK_STD_INTTYPES_H

#include "std-inttypes-defs.h"

#include "gonk/common/types.h"

#include <cstdint>

#if UINT32_MAX == 4294967295
#define GONK_INT32_IS_INT
#endif

namespace gonk
{
class Plugin;
} // namespace gonk

extern "C"
{

  GONK_STD_INTTYPES_API gonk::Plugin* gonk_std_inttypes_module();

} // extern "C"

namespace script {
#if defined(GONK_INT32_IS_INT)
static_assert(std::is_same<int, std::int32_t>::value, "int32_t must be the same as int");
template<> struct make_type_helper<std::int64_t> { inline static script::Type get() { return (gonk::std_inttypes::class_type_id_offset() + gonk::std_inttypes::ClassTypeIds::Int64) | script::Type::ObjectFlag; } };
#else
static_assert(std::is_same<int, std::int64_t>::value, "int64_t must be the same as int");
template<> struct make_type_helper<std::int32_t> { inline static script::Type get() { return (gonk::std_inttypes::class_type_id_offset() + gonk::std_inttypes::ClassTypeIds::Int32) | script::Type::ObjectFlag; } };
#endif 
template<> struct make_type_helper<std::uint32_t> { inline static script::Type get() { return (gonk::std_inttypes::class_type_id_offset() + gonk::std_inttypes::ClassTypeIds::UInt32) | script::Type::ObjectFlag; } };
template<> struct make_type_helper<std::uint64_t> { inline static script::Type get() { return (gonk::std_inttypes::class_type_id_offset() + gonk::std_inttypes::ClassTypeIds::UInt64) | script::Type::ObjectFlag; } };
} // namespace script
#endif // GONK_STD_INTTYPES_H
