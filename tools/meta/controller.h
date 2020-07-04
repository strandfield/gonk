// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_CONTROLLER_H
#define METAGONK_CONTROLLER_H

#include <QObject>

#include <memory>

#include "project.h"

class QFileInfo;

class QSqlDatabase;

class ProjectController;

class Controller : public QObject
{
  Q_OBJECT
public:
  explicit Controller(QObject* parent = nullptr);
  ~Controller();

  bool createSqlDatabase(const QFileInfo& db_dir, const QString& savepath);
  bool loadDatabase(const QFileInfo& db_file);
  void exportDatabase();
  QSqlDatabase& database() const;

  void loadProject();
  MGProjectPtr project() const;
  void importSymbols(MGProjectPtr other);
  ProjectController& projectController();

  static Controller& Instance();

protected Q_SLOTS:

private:
  static Controller* m_singleton;
  std::unique_ptr<QSqlDatabase> m_database;
  QString m_database_path;
  MGProjectPtr m_project;
  std::unique_ptr<ProjectController> m_project_controller;
};

#endif // METAGONK_CONTROLLER_H
