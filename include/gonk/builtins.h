// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_BUILTINS_H
#define GONK_BUILTINS_H

#include "gonk/gonk-defs.h"

#include <script/engine.h>

namespace gonk
{

void register_builtins(script::Namespace& ns);

} // namesapce gonk

#endif // GONK_BUILTINS_H
