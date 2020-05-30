// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project-controller.h"

#include "project/node-visitor.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QDebug>

#include <list>

struct NodeDeleter : public NodeVisitor
{
  QSqlDatabase& database;

  NodeDeleter(QSqlDatabase& db)
    : database(db)
  {

  }

  void exec(QString q)
  {
    QSqlQuery query = database.exec(q);

    if (database.lastError().isValid())
      qDebug() << database.lastError().text();
  }


  void exec(QString table_name, int id)
  {
    exec(QString("DELETE FROM %1 WHERE id = %2").arg(table_name, QString::number(id)));
  }

  void visit(Class& c) override
  {
    exec("classes", c.class_id);
  }

  void visit(Module& m) override
  {
    exec("modules", m.module_id);
  }

  void visit(Enum& e) override
  {
    exec("enums", e.enum_id);
  }

  void visit(Enumerator& e) override
  {
    exec("enumerators", e.enumerator_id);
  }

  void visit(File& f)  override
  {
    exec("files", f.file_id);
  }

  void visit(Function& f)  override
  {
    exec("functions", f.function_id);
  }

  void visit(Namespace& n) override
  {
    exec("namespaces", n.namespace_id);
  }

  void visit(Statement& s)  override
  {
    exec("statements", s.statement_id);
  }

};

void ProjectController::remove(NodeRef node)
{
  std::vector<NodeRef> nodes_to_delete;

  {
    // Compute nodes to delete

    std::list<NodeRef> nodes_to_process;

    nodes_to_process.push_back(node);

    while (!nodes_to_process.empty())
    {
      NodeRef n = nodes_to_process.front();
      nodes_to_process.pop_front();

      nodes_to_delete.push_back(n);

      for (size_t i(0); i < n->childCount(); ++i)
        nodes_to_process.push_back(n->childAt(i));
    }

    std::reverse(nodes_to_delete.begin(), nodes_to_delete.end());
  }

  for (NodeRef n : nodes_to_delete)
  {
    QSqlQuery query = database.exec(QString("DELETE FROM entities WHERE id = %1")
      .arg(QString::number(n->entity_id)));

    if (database.lastError().isValid())
      qDebug() << database.lastError().text();
  }

  NodeDeleter deleter{ database };

  for (NodeRef n : nodes_to_delete)
  {
    n->accept(deleter);
  }

  {
    NodeRef parent = node->parent.lock();

    if (parent == nullptr)
    {
      ModuleRef m = std::static_pointer_cast<Module>(node);
      project->modules.removeOne(m);
    }
    else
    {
      for (size_t i(0); i < parent->childCount(); ++i)
      {
        if (parent->childAt(i) == node)
        {
          parent->removeChild(i);
          break;
        }
      }
    }
  }
}
