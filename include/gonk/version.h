// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_VERSION_H
#define GONK_VERSION_H

#define GONK_VERSION_MAJOR 0
#define GONK_VERSION_MINOR 3
#define GONK_VERSION_PATCH 0
#define GONK_VERSION_SUFFIX ""

#include "gonk/gonk-defs.h"

#include <string>

namespace gonk
{

GONK_API std::string versionstr();

} // namesapce gonk

#endif // GONK_VERSION_H
