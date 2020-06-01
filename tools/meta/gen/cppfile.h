// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_CPPFILE_H
#define METAGONK_CPPFILE_H

#include "headerfile.h"

#include <set>

class CppFile
{
public:
  QString copyright_message;
  QString header_guard;

  QString main_include;

  struct IncludeGroup
  {
    std::string group_name;
    std::set<QString> files;
  };

  std::vector<IncludeGroup> includes;

  std::vector<QString> lines;

public:

  CppFile();

  void include(const char* group, QString file);

  void write(const QFileInfo& fileinfo);
};

#endif // METAGONK_CPPFILE_H
