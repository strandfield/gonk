// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_CPPPARSER_H
#define METAGONK_CPPPARSER_H

#include "project.h"

#include "project.h"

#include <cxx/parsers/parser.h>

#include <QStringList>

class CppParser
{
private:
  QStringList mIncludeDirectories;
  MGProjectPtr mProject;
  cxx::parsers::LibClangParser m_parser;

public:
  CppParser(const MGProjectPtr& pro);

  void addIncludeDirectory(const QString & str);
  inline void setIncludeDirectories(const QStringList & incdirs) { mIncludeDirectories = incdirs; }
  inline const QStringList & includeDirectories() const { return mIncludeDirectories; }

  inline const MGProjectPtr& project() const { return mProject; }

  void parse(const QString & file);
};

#endif // METAGONK_CPPPARSER_H
