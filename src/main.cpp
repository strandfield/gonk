// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/gonk.h"

int main(int argc, char *argv[])
{
  Gonk app{ argc, argv };
  return app.exec();
}