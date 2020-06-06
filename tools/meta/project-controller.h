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


  void addStatement(Node& node, const QString& content);

  void insert(FunctionRef func, NodeRef parent);

  bool update(File& file, const QString& name, const QStringList& hincludes, const QStringList& cppincludes);

  void update(Class& c, const QString& name, bool is_final, const QString& base);

  bool update(Function& fun, const QString& name, const QString& return_type, const QStringList& parameters, const QStringList& specifiers, Function::BindingMethod method, const QString& impl, const QString& condition);

  void update(Statement& stmt, const QString& content);

  void update(Node& node, const QString& name, const QString& condition);

  void update(Node& node, Qt::CheckState cs);

  void remove(NodeRef node, ProjectRef pro);

  void remove(std::shared_ptr<Type> t, ProjectRef pro);

  void move(NodeRef node, ProjectRef pro, int dest);

protected:
};

#endif // METAGONK_PROJECTCONTROLLER_H
