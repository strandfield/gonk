// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_SOURCEFILE_H
#define METAGONK_SOURCEFILE_H

#include "headerfile.h"

class SourceFile
{
public:
  QFileInfo file;

  QString header;

  Includes bindingIncludes;
  Includes generalIncludes;
  Includes libscriptIncludes;

  QStringList functions;

public:
  SourceFile();
  
  void write();
};

#endif // METAGONK_SOURCEFILE_H
