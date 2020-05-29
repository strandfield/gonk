// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_HEADERFILE_H
#define METAGONK_HEADERFILE_H

#include <QFileInfo>
#include <QMap>

#include "project/type.h"

class QTextStream;

class Includes
{
public:
  QMap<QString, bool> data;

  void insert(const QString & inc);
  void remove(const QString & inc);
  QStringList get() const;
  inline bool isEmpty() const { return data.isEmpty(); }
};

class HeaderFile
{
public:
  QFileInfo file;

  QString moduleName;
  Includes bindingIncludes;
  Includes generalIncludes;

  QMap<QString, Type> types;

public:
  HeaderFile();

  void write();

  static void writeCopyrightMessage(QTextStream & out);
  static void writeInclude(QTextStream & out, const QString & inc);

  static QByteArray readall(const QString & filepath);
  static void validate(const QFileInfo & finfo);

protected:
  QStringList generateBindingDefinitions();
};

#endif // METAGONK_HEADERFILE_H
