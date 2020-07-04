// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_DATABASE_EXPORTER_H
#define METAGONK_DATABASE_EXPORTER_H

#include "database.h"

#include <QDir>


enum class DBFieldType
{
  INTEGER,
  TEXT,
};

struct FieldDescription
{
  DBFieldType type = DBFieldType::INTEGER;
  QString name;
  bool not_null = false;
};

struct TableDescription
{
  QString name;
  std::vector<FieldDescription> fields;
};

class DatabaseExporter
{
  QDir m_export_dir;
public:
  
  DatabaseExporter(QDir exportDir)
    : m_export_dir(exportDir)
  {

  }

  void exportDatabase();

protected:

  static FieldDescription INTEGER_NOT_NULL(const QString& name);
  static FieldDescription INTEGER(const QString& name);
  static FieldDescription TEXT_NOT_NULL(const QString& name);
  static FieldDescription TEXT(const QString& name);

  void exportTable(const TableDescription& table, const QString& createStatement);

};

#endif // METAGONK_DATABASE_EXPORTER_H
