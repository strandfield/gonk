// Copyright (C) 2022 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_MODULEFILE_H
#define GONK_MODULEFILE_H

#include "gonk/gonk-defs.h"

#include <filesystem>
#include <optional>
#include <string>
#include <vector>

namespace gonk
{

class GonkModuleFile
{
public:
  std::string name;
  std::optional<std::string> sourcefile;
  std::optional<std::string> entry_point;
  std::optional<std::vector<std::string>> dependencies;

public:
  static GonkModuleFile read(const std::filesystem::path& filepath);
};

} // namespace gonk

#endif // GONK_MODULEFILE_H
