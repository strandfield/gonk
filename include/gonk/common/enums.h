// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_COMMONS_ENUMS_H
#define GONK_COMMONS_ENUMS_H

#include <script/engine.h>
#include <script/value.h>
#include <script/enumerator.h>
#include <script/typesystem.h>

namespace script
{

inline script::Value make_enum(script::Engine *e, const script::Type &enum_type, int val)
{
  return script::Value::fromEnumerator(script::Enumerator{ e->typeSystem()->getEnum(enum_type), val });
}

} // namespace script

#endif // GONK_COMMONS_ENUMS_H
