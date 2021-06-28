// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/version.h"

#include <stdexcept>

namespace gonk
{

std::string versionstr()
{
  return std::to_string(GONK_VERSION_MAJOR)
    + "." + std::to_string(GONK_VERSION_MINOR)
    + "." + std::to_string(GONK_VERSION_PATCH)
    + GONK_VERSION_SUFFIX;
}

} // namespace gonk
