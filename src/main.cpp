// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/gonk.h"

#include <iostream>

int main(int argc, char *argv[])
{
  try 
  {
    Gonk app{ argc, argv };
    return app.exec();
  }
  catch (const std::exception& ex)
  {
    std::cerr << ex.what() << std::endl;
    return 1;
  }
}
