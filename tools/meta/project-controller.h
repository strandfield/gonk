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
  ProjectRef project;

  ProjectController(QSqlDatabase & db, ProjectRef pro)
    : database(db), project(pro)
  {
  }

  void remove(NodeRef node);

protected:
};

#endif // METAGONK_PROJECTCONTROLLER_H
