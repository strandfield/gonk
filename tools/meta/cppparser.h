// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_CPPPARSER_H
#define METAGONK_CPPPARSER_H

#include "project.h"

#include "project/module.h"

#include <cxx/parsers/parser.h>

class CppParser
{
private:
  QStringList mIncludeDirectories;
  QtVersion mVersion;
  ProjectRef mProject;
  ModuleRef mActiveModule;
  cxx::parsers::LibClangParser m_parser;

public:
  CppParser(const ProjectRef & pro);

  void addIncludeDirectory(const QString & str);
  inline void setIncludeDirectories(const QStringList & incdirs) { mIncludeDirectories = incdirs; }
  inline const QStringList & includeDirectories() const { return mIncludeDirectories; }

  inline void setVersion(const QtVersion & v) { mVersion = v; }
  inline const QtVersion & version() const { return mVersion; }

  inline const ProjectRef & project() const { return mProject; }
  inline const ModuleRef & activeModule() const { return mActiveModule; }

  void parse(const QString & file, const ModuleRef & mo);
};

#endif // METAGONK_CPPPARSER_H
