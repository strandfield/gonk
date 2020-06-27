// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project-controller.h"

#include "database.h"
#include "project-loader.h" // TODO: remove this include

#include <cxx/function.h>
#include <cxx/class.h>
#include <cxx/enum.h>
#include <cxx/namespace.h>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QDebug>

#include <list>

void ProjectController::insert(std::shared_ptr<cxx::Function> func, std::shared_ptr<cxx::Entity> parent)
{
  if (project->inDB(parent))
  {
    auto& ids = project->dbid(func);

    QSqlQuery query = Database::exec(QString("INSERT INTO functions(name, return_type, parameters, specifiers) VALUES('%1', '%2', '%3', '%4')")
      .arg(QString::fromStdString(func->name), QString::fromStdString(func->return_type.toString()), Database::parameters(*func), Database::specifiers(*func)));
    ids.id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, function_id) VALUES(%1, %2)")
      .arg(QString::number(project->dbid(parent).global_id), QString::number(ids.id)));
    ids.global_id = query.lastInsertId().toInt();
  }

  parent->appendChild(func);
}

void ProjectController::update(cxx::Class& c, const QString& name, bool is_final, const QString& base)
{
  if (project->inDB(c.shared_from_this()))
  {
    Database::exec(QString("UPDATE classes SET name='%1', base='%2', final=%3 WHERE id = %4").arg(
      name, base, is_final ? "1" : "0", QString::number(project->dbid(c.shared_from_this()).global_id)
    ));
  }

  c.name = name.toStdString();
  c.is_final = is_final;

  if (base.isEmpty() != c.bases.empty() || !c.bases.empty() && c.bases.front().base->name != base.toStdString())
  {
    c.bases.clear();

    if (!base.isEmpty())
    {
      cxx::BaseClass bc;
      bc.base = std::make_shared<cxx::Class>(base.toStdString());
      c.bases.push_back(bc);
    }
  }
}

bool ProjectController::update(cxx::Function& fun, const QString& name, const QString& return_type, const QStringList& parameters, const QStringList& specifiers)
{
  if (project->inDB(fun.shared_from_this()))
  {
    Database::exec(QString("UPDATE functions SET name='%1', return_type='%2', parameters='%3', specifiers='%4' "
      "WHERE id = %5")
      .arg(name, return_type, parameters.join(';'), specifiers.join(','), QString::number(project->dbid(fun.shared_from_this()).id)));
  }

  fun.name = name.toStdString();
  fun.return_type = cxx::Type(return_type.toStdString());
  fun.parameters.clear();
  fun.specifiers = 0;
  MGProjectLoader::writeSpecifiers(fun, specifiers.join(','));
  MGProjectLoader::write(fun.parameters, parameters.join(';'));

  return true;
}

void ProjectController::update(cxx::Namespace& ns, const QString& name)
{
  if (project->inDB(ns.shared_from_this()))
  {
    Database::exec(QString("UPDATE namespaces SET name='%1'"
      "WHERE id = %2")
      .arg(name, QString::number(project->dbid(ns.shared_from_this()).id)));
  }

  ns.name = name.toStdString();
}

struct DBNodeDeleter
{
  MGProjectPtr project;
  QSqlDatabase& database;

  DBNodeDeleter(MGProjectPtr pro, QSqlDatabase& db)
    : project(pro), database(db)
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

  void dewit(std::shared_ptr<cxx::Entity> e)
  {
    if(e->is<cxx::Class>())
      exec("classes", project->dbid(e).id);
    else if (e->is<cxx::Function>())
      exec("functions", project->dbid(e).id);
    else if (e->is<cxx::Namespace>())
      exec("namespaces", project->dbid(e).id);
    else if (e->is<cxx::Enum>())
      exec("enums", project->dbid(e).id);
    else if (e->is<cxx::EnumValue>())
      exec("enumerators", project->dbid(e).id);
  }
};

void ProjectController::remove(std::shared_ptr<cxx::Entity> node, MGProjectPtr pro)
{
  if (pro->inDB(node))
  {
    std::vector<std::shared_ptr<cxx::Entity>> nodes_to_delete;

    {
      // Compute nodes to delete

      std::list<std::shared_ptr<cxx::Entity>> nodes_to_process;

      nodes_to_process.push_back(node);

      while (!nodes_to_process.empty())
      {
        std::shared_ptr<cxx::Entity> n = nodes_to_process.front();
        nodes_to_process.pop_front();

        nodes_to_delete.push_back(n);

        for (size_t i(0); i < n->childCount(); ++i)
          nodes_to_process.push_back(std::static_pointer_cast<cxx::Entity>(n->childAt(i))); // TODO: check all childs are actually entities
      }

      std::reverse(nodes_to_delete.begin(), nodes_to_delete.end());
    }

    for (std::shared_ptr<cxx::Entity> n : nodes_to_delete)
    {
      QSqlQuery query = database.exec(QString("DELETE FROM entities WHERE id = %1")
        .arg(QString::number(project->dbid(n).global_id)));

      if (database.lastError().isValid())
        qDebug() << database.lastError().text();
    }

    DBNodeDeleter deleter{ pro, database };

    for (std::shared_ptr<cxx::Entity> n : nodes_to_delete)
    {
      deleter.dewit(n);
    }
  }

  {
    auto parent = node->weak_parent.lock();

    if (parent == nullptr)
    {
      for (auto m : pro->modules)
      {
        for (auto it = m->entities.begin(); it != m->entities.end(); ++it)
        {
          if (*it == node)
          {
            m->entities.erase(it);
            return;
          }
        }
      }
    }
    else
    {
      if (parent->is<cxx::Class>())
      {
        auto& p = static_cast<cxx::Class&>(*parent);

        for (auto it = p.members.begin(); it != p.members.end(); ++it)
        {
          if (*it == node)
          {
            p.members.erase(it);
            break;
          }
        }
      }
      else if (parent->is<cxx::Namespace>())
      {
        auto& p = static_cast<cxx::Namespace&>(*parent);

        for (auto it = p.entities.begin(); it != p.entities.end(); ++it)
        {
          if (*it == node)
          {
            p.entities.erase(it);
            break;
          }
        }
      }
      else if (parent->is<cxx::Enum>())
      {
        auto& p = static_cast<cxx::Enum&>(*parent);

        for (auto it = p.values.begin(); it != p.values.end(); ++it)
        {
          if (*it == node)
          {
            p.values.erase(it);
            break;
          }
        }
      }
    }
  }
}

void ProjectController::remove(MGModulePtr node, MGProjectPtr pro)
{
  while (!node->entities.empty())
  {
    remove(node->entities.back(), pro);
  }

  if (pro->inDB(node))
  {
    QSqlQuery query = database.exec(QString("DELETE FROM entities WHERE id = %1")
      .arg(QString::number(project->dbid(node).global_id)));

    if (database.lastError().isValid())
      qDebug() << database.lastError().text();
  }

  auto it = std::find(pro->modules.begin(), pro->modules.end(), node);

  if (it != pro->modules.end())
    pro->modules.erase(it);
}

void ProjectController::remove(MGTypePtr t, MGProjectPtr pro)
{
  auto it = std::find(pro->types.begin(), pro->types.end(), t);

  if (it == pro->types.end())
    return;

  if (t->database_id != -1)
  {
    Database::exec(QString("DELETE FROM types WHERE id = %1")
      .arg(QString::number(t->database_id)));
  }

  pro->types.erase(it);
}
