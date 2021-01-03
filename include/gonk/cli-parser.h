// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_GONKCLIPARSER_H
#define GONK_GONKCLIPARSER_H

#include "gonk/gonk-defs.h"

#include <string>

namespace gonk
{

template<typename T>
class GenericCliParser
{
public:
  T& cli;
  size_t index = 1;

  GenericCliParser(T& c)
    : cli(c)
  {

  }

  static bool isOption(const std::string& arg)
  {
    return arg.empty() ? false : (arg.front() == '-');
  }

protected:
  bool atEnd() const
  {
    return index == static_cast<size_t>(cli.argc);
  }

  std::string peek() const
  {
    return std::string(cli.argv[index]);
  }

  std::string read()
  {
    return std::string(cli.argv[index++]);
  }

  void advance()
  {
    ++index;
  }
};

} // namesapce gonk

#endif // GONK_GONKCLIPARSER_H
