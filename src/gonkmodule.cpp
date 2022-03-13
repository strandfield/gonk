// Copyright (C) 2022 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/gonkmodule.h"

#include <fstream>
#include <stdexcept>

namespace gonk
{

static std::vector<std::string> parse_dependencies(std::string value)
{
  std::vector<std::string> r;

  const char* delim = ",";

  char* token = std::strtok(value.data(), delim);
  
  while (token) 
  {
    r.push_back(std::string(token));
    token = std::strtok(nullptr, delim);
  }

  return r;
}

static void process_line(GonkModuleFile& file, const std::string& key, const std::string& value)
{
  if (key == "name")
  {
    file.name = value;
  }
  else if (key == "sourcefile")
  {
    file.sourcefile = value;
  }
  else if (key == "entry_point")
  {
    file.entry_point = value;
  }
  else if (key == "dependencies")
  {
    file.dependencies = parse_dependencies(value);
  }
}

GonkModuleFile GonkModuleFile::read(const std::filesystem::path& filepath)
{
  std::ifstream stream{ filepath.string(), std::ios::in };

  if (!stream.good())
    throw std::runtime_error("Could not load gonkmodule file " + filepath.string());

  GonkModuleFile r;
  
  while (!stream.eof())
  {
    std::string l;
    std::getline(stream, l);

    size_t split_index = l.find('=');

    if (split_index == std::string::npos)
      continue;

    std::string key{ l.begin(), l.begin() + split_index };
    std::string value{ l.begin() + split_index + 1, l.end() };

    process_line(r, key, value);
  }

  return r;
}

} // namespace gonk
