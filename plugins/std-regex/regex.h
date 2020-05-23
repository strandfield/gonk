// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "std-regex-defs.h"

#include "gonk/common/types.h"

#include <regex>

namespace script {
template<> struct make_type_helper<std::regex> { inline static script::Type get() { return (gonk::std_regex::class_type_id_offset() + gonk::std_regex::ClassTypeIds::Regex) | script::Type::ObjectFlag; } };
} // namespace script
