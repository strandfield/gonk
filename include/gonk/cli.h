// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_GONKCLI_H
#define GONK_GONKCLI_H

#include "gonk/gonk-defs.h"

#include <optional>
#include <string>
#include <vector>

namespace gonk
{

class GONK_API CLI
{
public:
  CLI(int& argc, char** argv);

public:
  int argc;
  char** argv;

public:
  std::string argvAt(size_t index) const;

  bool empty() const;

public:
  bool version = false;
  bool interactive = false;
  bool list_modules = false;
  bool debug = false;
  std::optional<std::string> script;
  std::vector<std::string> extras;

private:
  std::vector<std::string> m_args;
};

} // namesapce gonk

#endif // GONK_GONKCLI_H
