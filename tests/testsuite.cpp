// Copyright (C) 2022 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk-test-resources.h"

#include "gonk/gonk.h"
#include "gonk/modules.h"
#include "gonk/templates/pointer-template.h"

#include <script/interpreter/executioncontext.h>

#include <cassert>
#include <iostream>
#include <filesystem>

int main(int argc, char* argv[])
{
  std::string gonk_str = "gonk";
  std::string debugbuild = "--debug-build";
  std::string emptystr = "gonk";

  int custom_argc = 3;
  char* custom_argv[3] = { gonk_str.data(), debugbuild.data(), emptystr.data() };

  std::string dirpath = gonk_test_resources_path() + std::string("suite");
  std::string modules_path = gonk_build_path() + std::string("modules");

  std::cout << "Module import path: " << modules_path << std::endl;

  int nb_fail = 0;

  for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(dirpath))
  {
    if (!entry.is_regular_file() || entry.path().extension() != ".gnk")
      continue;

    std::string path = entry.path().string();
    custom_argv[2] = path.data();

    std::cout << "Running " << path << std::endl;

    Gonk gonk{ custom_argc, custom_argv };
    gonk.moduleManager().addImportPath(modules_path);

    try
    {
      int n = gonk.exec();

      if (n != 0)
        nb_fail += 1;
    }
    catch (...)
    {
      std::cout << "... fail" << std::endl;
    }
  }

  return nb_fail;
}