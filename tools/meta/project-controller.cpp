// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project-controller.h"

#include "project/node-visitor.h"

#include "database.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QDebug>

#include <list>

void ProjectController::addStatement(Node& node, const QString& content)
{
  auto stmt = std::make_shared<Statement>(content);
  stmt->order = static_cast<int>(node.childCount());

  if (node.entity_id != -1)
  {
    QSqlQuery query = Database::exec(QString("INSERT INTO statements(content) VALUES('%1')").arg(stmt->name));
    stmt->statement_id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, statement_id, rank) VALUES(%1, %2, %3)")
      .arg(QString::number(node.entity_id), QString::number(stmt->statement_id), QString::number(stmt->order)));
    stmt->entity_id = query.lastInsertId().toInt();

    project->statements[stmt->statement_id] = stmt;
  }

  node.appendChild(stmt);
}

void ProjectController::insert(FunctionRef f, NodeRef parent)
{
  f->order = static_cast<int>(parent->childCount());

  if (parent->entity_id != -1)
  {
    QSqlQuery query = Database::exec(QString("INSERT INTO functions(name, return_type, parameters, specifiers) VALUES('%1', '%2', '%3', '%4')")
      .arg(f->name, f->returnType, f->parameters.join(';'), f->getSpecifiers().join(',')));
    f->function_id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, function_id, rank) VALUES(%1, %2, %3)")
      .arg(QString::number(parent->entity_id), QString::number(f->function_id), QString::number(f->order)));
    f->entity_id = query.lastInsertId().toInt();

    project->functions[f->function_id] = f;
    project->entities[f->entity_id] = f;
  }

  parent->appendChild(f);
}

bool ProjectController::update(File& file, const QString& name, const QStringList& hincludes, const QStringList& cppincludes)
{
  QSqlQuery query = database.exec(QString("UPDATE files SET name='%1', hincludes='%2', cppincludes='%3' WHERE id = %4").arg(
    name, hincludes.join(','), cppincludes.join(','), QString::number(file.file_id)
  ));

  if (query.lastError().isValid())
    return false;

  file.name = name;
  file.hincludes = hincludes;
  file.cppincludes = cppincludes;

  return true;
}

bool ProjectController::update(Function& fun, const QString& name, const QString& return_type, const QStringList& parameters, const QStringList& specifiers, Function::BindingMethod method, const QString& impl, const QString& condition)
{
  if (fun.function_id != -1)
  {
    QString bm = Function::serialize(method);
    
    if (bm == "auto")
    {
      Database::exec(QString("UPDATE functions SET name='%1', return_type='%2', parameters='%3', "
        "specifiers='%4', binding=NULL, implementation='%6', condition='%7' "
        "WHERE id = %8")
        .arg(name, return_type, parameters.join(';'), specifiers.join(','), impl, condition, QString::number(fun.function_id)));
    }
    else
    {
      Database::exec(QString("UPDATE functions SET name='%1', return_type='%2', parameters='%3', "
        "specifiers='%4', binding='%5', implementation='%6', condition='%7' "
        "WHERE id = %8")
        .arg(name, return_type, parameters.join(';'), specifiers.join(','), bm, impl, condition, QString::number(fun.function_id)));
    }
  }

  fun.name = name;
  fun.returnType = return_type;
  fun.parameters = parameters;
  fun.setSpecifiers(specifiers);
  fun.bindingMethod = method;
  fun.condition = condition;
  fun.implementation = impl;

  return true;
}

void ProjectController::update(Statement& stmt, const QString& content)
{
  if (stmt.statement_id != -1)
  {
    Database::exec(QString("UPDATE statements SET content='%1'"
      "WHERE id = %2")
      .arg(content, QString::number(stmt.statement_id)));
  }

  stmt.name = content;
}

void ProjectController::update(Node& node, const QString& name, const QString& condition)
{
  if (node.entity_id != -1)
  {
    if (node.is<Statement>())
    {
      Database::exec(QString("UPDATE statements SET content='%1'"
        "WHERE id = %2")
        .arg(name, QString::number(node.as<Statement>().statement_id)));
    }
    else if (node.is<Enumerator>())
    {
      Database::exec(QString("UPDATE enumerators SET name='%1'"
        "WHERE id = %2")
        .arg(name, QString::number(node.as<Enumerator>().enumerator_id)));
    }
  }

  node.name = name;
  node.condition = condition;
}

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

void ProjectController::remove(NodeRef node, ProjectRef pro)
{
  if (node->entity_id != -1)
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

    if (node->parent.lock())
    {
      NodeRef parent = node->parent.lock();

      Database::exec(QString("UPDATE entities SET rank = rank - 1 WHERE parent = %1 AND rank > %2")
        .arg(QString::number(parent->entity_id), QString::number(node->order)));

      for (size_t i(static_cast<size_t>(node->order)); i < parent->childCount(); ++i)
        parent->childAt(i)->order -= 1;
    }

    NodeDeleter deleter{ database };

    for (NodeRef n : nodes_to_delete)
    {
      n->accept(deleter);
    }
  }

  {
    NodeRef parent = node->parent.lock();

    if (parent == nullptr)
    {
      ModuleRef m = std::static_pointer_cast<Module>(node);
      pro->modules.removeOne(m);
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

void ProjectController::remove(std::shared_ptr<Type> t, ProjectRef pro)
{
  auto& list = [&]() -> QList<std::shared_ptr<Type>>& {
    if (t->is_class)
      return pro->types.classes;
    else if (t->is_enum)
      return pro->types.enums;
    else
      return pro->types.fundamentals;
  }();

  int index = list.indexOf(t);

  if (index == -1)
    return;

  if (t->database_id != -1)
  {
    Database::exec(QString("DELETE FROM types WHERE id = %1")
      .arg(QString::number(t->database_id)));
  }

  list.removeAt(index);
}

void ProjectController::move(NodeRef node, ProjectRef pro, int delta)
{
  Q_ASSERT(delta == -1 || delta == 1);

  if (node->order == 0 && delta == -1)
    return;
  // @TODO: add check for delta == 1

  if (node->entity_id != -1)
  {
    QString parent_id = node->parent.lock() != nullptr ? QString::number(node->parent.lock()->entity_id) : QString("NULL");

    Database::exec(QString("UPDATE entities SET rank = %1 WHERE parent = %2 AND rank = %3")
      .arg(QString::number(node->order), parent_id, QString::number(node->order + delta)));

    Database::exec(QString("UPDATE entities SET rank = %1 WHERE id = %2")
      .arg(QString::number(node->order + delta), QString::number(node->entity_id)));

    if (node->parent.lock() != nullptr)
      node->parent.lock()->childAt(node->order + delta)->order = node->order;
    else
      pro->modules.at(node->order + delta)->order = node->order;

    node->order += delta;
  }

  if (node->parent.lock() != nullptr)
  {
    auto parent = node->parent.lock();

    if (parent->is<Class>())
      parent->as<Class>().elements.swap(node->order, node->order + delta);
    else if (parent->is<Namespace>())
      parent->as<Namespace>().elements.swap(node->order, node->order + delta);
    else if (parent->is<Module>())
      parent->as<Module>().elements.swap(node->order, node->order + delta);
    else if (parent->is<Enum>())
      parent->as<Enum>().enumerators.swap(node->order, node->order + delta);
  }
  else
  {
    pro->modules.swap(node->order, node->order + delta);
  }
}
