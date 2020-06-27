// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_PROJECTCONTROLLER_H
#define METAGONK_PROJECTCONTROLLER_H

#include "project.h"

#include <QObject>

class QSqlDatabase;

class ProjectController : QObject
{
  Q_OBJECT

public:
  QSqlDatabase& database;
  MGProjectPtr project;

  ProjectController(QSqlDatabase & db, MGProjectPtr pro)
    : database(db), project(pro)
  {
  }

  void insert(std::shared_ptr<cxx::Function> func, std::shared_ptr<cxx::Entity> parent);

  void update(cxx::Class& c, const QString& name, bool is_final, const QString& base);

  bool update(cxx::Function& fun, const QString& name, const QString& return_type, const QStringList& parameters, const QStringList& specifiers);

  void update(cxx::Namespace& ns, const QString& name);

  void remove(std::shared_ptr<cxx::Entity> node, MGProjectPtr pro);
  void remove(MGModulePtr node, MGProjectPtr pro);

  void remove(MGTypePtr t, MGProjectPtr pro);

protected:
};

#endif // METAGONK_PROJECTCONTROLLER_H
