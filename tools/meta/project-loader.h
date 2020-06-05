// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_PROJECTLOADER_H
#define METAGONK_PROJECTLOADER_H

#include "project.h"

#include <QObject>

#include <map>
#include <memory>

class QFileInfo;

class QSqlDatabase;

class ProjectLoader : QObject
{
  Q_OBJECT

  Q_PROPERTY(QString state READ state NOTIFY stateChanged)

public:
  QSqlDatabase& database;
  ProjectRef project;

  ProjectLoader(QSqlDatabase & db)
    : database(db)
  {
    project = std::make_shared<Project>();
  }

  void load()
  {
    loadTypes();
    loadEntities();
  }

  QString state() const;

Q_SIGNALS:
  void stateChanged();

protected:
  void setState(const QString& st);

  void write(Qt::CheckState& cs, QString& cond, QString db_cond);

  void loadTypes();
  void loadEntities();
  void loadModules();
  void loadFiles();
  void loadFunctions();
  void loadClasses();
  void loadEnums();
  void loadEnumerators();
  void loadStatements();
  void buildEntityTree();

private:
  QString m_state;
};

#endif // METAGONK_PROJECTLOADER_H
