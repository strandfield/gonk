// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/cli.h"

#include "gonk/cli-parser.h"

#include <iostream>
#include <stdexcept>

namespace gonk
{

class CliParser : public GenericCliParser<CLI>
{
public:

  using GenericCliParser<CLI>::GenericCliParser;

  static bool isScriptName(const std::string& arg)
  {
    if (arg.empty())
      return false;

    if (arg.size() >= 4)
    {
      if (std::string(arg.cend() - 4, arg.end()) == ".gnk")
        return true;
    }

    // We cannot say at 100% but it's probable...
    return true;
  }

  void parse()
  {
    while (!atEnd())
    {
      std::string arg = read();
      
      if (arg == "--version" || arg == "-v")
      {
        cli.version = true;
      }
      else if (arg == "--help" || arg == "-h")
      {
        cli.help = true;
      }
      else if (arg == "--interactive")
      {
        cli.interactive = true;
      }
      else if (arg == "--list-modules")
      {
        cli.list_modules = true;
      }
      else if (arg == "--debug")
      {
        cli.debug = true;
      }
      else if (arg == "--debug-build")
      {
        cli.debugbuild = true;
      }
      else
      {
        if (isOption(arg))
          throw std::runtime_error("Unrecognized option");

        if (isScriptName(arg))
        {
          cli.script = arg;
          parseExtras();
        }
      }
    }
  }

protected:
  void parseExtras()
  {
    while(!atEnd())
    {
      std::string arg = read();
      cli.extras.push_back(arg);
    }
  }
};

CLI::CLI(int& argc_, char** argv_)
  : argc(argc_),
    argv(argv_)
{
  CliParser parser{ *this };
  parser.parse();
}

std::string CLI::argvAt(size_t index) const
{
  return std::string(argv[index]);
}

bool CLI::empty() const
{
  return !version && !help && !interactive && !list_modules && !script.has_value();
}

void CLI::displayHelp()
{
  std::cout << "gonk" << std::endl;
  std::cout << "----" << std::endl;
  std::cout << "Start interactive session:" << std::endl;
  std::cout << "  gonk --interactive" << std::endl;
  std::cout << "Execute program:" << std::endl;
  std::cout << "  gonk [--debug] file [options]" << std::endl;
  std::cout << "Print version:" << std::endl;
  std::cout << "  gonk -v" << std::endl;
  std::cout << "  gonk --version" << std::endl;
  std::cout << "Show help:" << std::endl;
  std::cout << "  gonk -h" << std::endl;
  std::cout << "  gonk --help" << std::endl;
}

} // namespace gonk
