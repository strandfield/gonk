// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project-merger.h"

#include "database.h"

#include <cxx/class.h>
#include <cxx/enum.h>
#include <cxx/function.h>
#include <cxx/namespace.h>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QFileInfo>

#include <QDebug>

#include <stdexcept>

void MGProjectMerger::merge()
{
  fetchTypes(other);

  for (const auto& e : m_types_map)
  {
    auto t = e.second;

    if (e.first->is<cxx::Class>())
    {
      auto c = std::static_pointer_cast<cxx::Class>(e.first);

      if (!project->hasType(t->name))
      {
        QSqlQuery query = Database::exec(QString("INSERT INTO types(name, typeid, is_enum, is_class) VALUES('%1', '%2', 0, 1)")
          .arg(QString::fromStdString(t->name), QString::fromStdString(t->id)));

        t->database_id = query.lastInsertId().toInt();
        project->entity_type_map[c] = t;

        project->types.push_back(t);
      }
    }
    else if (e.first->is<cxx::Enum>())
    {
      auto enm = std::static_pointer_cast<cxx::Enum>(e.first);

      if (!project->hasType(t->name))
      {
        QSqlQuery query = Database::exec(QString("INSERT INTO types(name, typeid, is_enum, is_class) VALUES('%1', '%2', 1, 0)")
          .arg(QString::fromStdString(t->name), QString::fromStdString(t->id)));

        t->database_id = query.lastInsertId().toInt();
        project->entity_type_map[enm] = t;

        project->types.push_back(t);
      }
    }
  }

  merge_recursively(project->modules, other->modules);
}

int MGProjectMerger::importedSymbolsCount() const
{
  return m_imported_symbols_count;
}

void MGProjectMerger::incrImportedSymbolsCount()
{
  m_imported_symbols_count++;
  Q_EMIT importedSymbolsCountChanged();
}

QString MGProjectMerger::parentId() const
{
  if (m_parent == nullptr && m_current_module == nullptr)
    return "NULL";

  if (m_parent != nullptr)
  {
    auto& dbid = project->dbid(m_parent);

    if (dbid.global_id == -1)
      throw std::runtime_error{ "missing parent id" };

    return QString::number(dbid.global_id);
  }
  else
  {
    auto& dbid = project->dbid(m_current_module);

    if (dbid.global_id == -1)
      throw std::runtime_error{ "missing parent id" };

    return QString::number(dbid.global_id);
  }
}

void MGProjectMerger::getIds(std::shared_ptr<cxx::Entity> elem)
{
  auto& ids = project->dbid(elem);

  if (ids.global_id != -1)
    return;

  if (elem->is<cxx::Namespace>())
  {
    auto ns = std::static_pointer_cast<cxx::Namespace>(elem);

    QSqlQuery query = Database::exec(QString("INSERT INTO namespaces(name) VALUES('%1')").arg(QString::fromStdString(ns->name)));
    ids.id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, namespace_id) VALUES(%1, %2)")
      .arg(parentId(), QString::number(ids.id)));
    ids.global_id = query.lastInsertId().toInt();
  }
  else if (elem->is<cxx::Class>())
  {
    auto c = std::static_pointer_cast<cxx::Class>(elem);

    auto t = m_types_map.at(c);

    QSqlQuery query = Database::exec(QString("INSERT INTO classes(name, base, type) VALUES('%1', '%2', %3)")
      .arg(QString::fromStdString(c->name), Database::base(*c), QString::number(t->database_id)));
    ids.id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, class_id) VALUES(%1, %2)")
      .arg(parentId(), QString::number(ids.id)));
    ids.global_id = query.lastInsertId().toInt();
  }
  else if (elem->is<cxx::Enum>())
  {
    auto e = std::static_pointer_cast<cxx::Enum>(elem);

    auto t = m_types_map.at(e);

    QSqlQuery query = Database::exec(QString("INSERT INTO enums(name, type) VALUES('%1', %2)")
      .arg(QString::fromStdString(e->name), QString::number(t->database_id)));
    ids.id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, enum_id) VALUES(%1, %2)")
      .arg(parentId(), QString::number(ids.id)));
    ids.global_id = query.lastInsertId().toInt();
  }
  else if (elem->is<cxx::EnumValue>())
  {
    auto e = std::static_pointer_cast<cxx::EnumValue>(elem);
    auto parent = std::static_pointer_cast<cxx::Enum>(m_parent);

    QSqlQuery query = Database::exec(QString("INSERT INTO enumerators(name, enum_id) VALUES('%1', %2)").arg(QString::fromStdString(e->name), QString::number(project->dbid(m_parent).id)));
    ids.id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, enumerator_id) VALUES(%1, %2)")
      .arg(parentId(), QString::number(ids.id)));
    ids.global_id = query.lastInsertId().toInt();
  }
  else if (elem->is<cxx::Function>())
  {
    auto f = std::static_pointer_cast<cxx::Function>(elem);

    QSqlQuery query = Database::exec(QString("INSERT INTO functions(name, return_type, parameters, specifiers) VALUES('%1', '%2', '%3', '%4')")
      .arg(QString::fromStdString(f->name), QString::fromStdString(f->return_type.toString()), Database::parameters(*f), Database::specifiers(*f)));
    ids.id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, function_id) VALUES(%1, %2)")
      .arg(parentId(), QString::number(ids.id)));
    ids.global_id = query.lastInsertId().toInt();
  }
  else
  {
    return;
  }

  incrImportedSymbolsCount();
}

void MGProjectMerger::getIds(MGModulePtr elem)
{
  auto& ids = project->dbid(elem);

  QSqlQuery query = Database::exec(QString("INSERT INTO modules(name) VALUES('%1')").arg(QString::fromStdString(elem->name)));
  ids.id = query.lastInsertId().toInt();

  query = Database::exec(QString("INSERT INTO entities(parent, module_id) VALUES(%1, %2)")
    .arg(parentId(), QString::number(ids.id)));
  ids.global_id = query.lastInsertId().toInt();
}

void MGProjectMerger::merge_recursively(std::vector<std::shared_ptr<cxx::Entity>>& target, const std::vector<std::shared_ptr<cxx::Entity>>& src)
{
  for (const auto& srcItem : src)
  {
    std::shared_ptr<cxx::Entity> node = find_or_set(target, srcItem);

    auto prev_parent = m_parent;
    m_parent = node;

    if (node == srcItem)
    {
      auto childs = ::children(*node);
      assignIds(childs);
      continue;
    }

    if (node->is<cxx::Namespace>())
    {
      auto& targetNamespace = static_cast<cxx::Namespace&>(*node);
      auto& srcNamespace = static_cast<cxx::Namespace&>(*srcItem);

      merge_recursively(targetNamespace.entities, srcNamespace.entities);
    }
    else if (node->is<cxx::Class>())
    {
      auto& target_class = static_cast<cxx::Class&>(*node);
      auto& src_class = static_cast<cxx::Class&>(*srcItem);

      merge_recursively(target_class.members, src_class.members);
    }
    else if (node->is<cxx::Enum>())
    {
      auto& target_enum = static_cast<cxx::Enum&>(*node);
      auto& src_enum = static_cast<cxx::Enum&>(*srcItem);

      for (auto v : src_enum.values)
      {
        auto it = std::find_if(target_enum.values.begin(), target_enum.values.end(), [&](const std::shared_ptr<cxx::Entity>& e) -> bool {
          return e->name == v->name;
          });

        if (it == target_enum.values.end())
          target_enum.appendChild(v);
      }
    }
    else
    {
      qDebug() << "Element already exists in project";
    }

    m_parent = prev_parent;
  }
}

MGModulePtr MGProjectMerger::find_or_set(std::vector<MGModulePtr>& list, const MGModulePtr& elem)
{
  for (const auto& item : list)
  {
    if (item->name == elem->name)
      return item;
  }

  getIds(elem);
  list.push_back(elem);
  return elem;
}

void MGProjectMerger::merge_recursively(std::vector<MGModulePtr>& target, const std::vector<MGModulePtr>& src)
{
  for (const auto& srcItem : src)
  {
    MGModulePtr m = find_or_set(target, srcItem);

    m_current_module = m;

    if (m == srcItem)
      assignIds(m->entities);
    else
      merge_recursively(m->entities, srcItem->entities);

    m_current_module = nullptr;
  }
}

void MGProjectMerger::fetch_types_recursively(MGProject& pro, const std::shared_ptr<cxx::Entity>& node)
{
  if (node->is<cxx::Namespace>())
  {
    auto& ns = static_cast<cxx::Namespace&>(*node);

    fetch_types_recursively(pro, ns.entities);
  }
  else if (node->is<cxx::Class>())
  {
    auto& c = static_cast<cxx::Class&>(*node);

    if (pro.hasType(node->name))
      return;

    std::string n = qualifiedName(c);

    auto type = std::make_shared<MGType>(n, QString::fromStdString(n).remove("::").toStdString());
    type->category = MGType::ClassType;
    m_types_map[node] = type;

    fetch_types_recursively(pro, c.members);

    pro.types.push_back(type);

  }
  else if (node->is<cxx::Enum>())
  {
    if (pro.hasType(node->name))
      return;

    std::string n = qualifiedName(*node);

    auto type = std::make_shared<MGType>(n, QString::fromStdString(n).remove("::").toStdString());
    type->category = MGType::EnumType;
    m_types_map[node] = type;

    pro.types.push_back(type);
  }
}

void MGProjectMerger::fetch_types_recursively(MGProject& pro, const MGModulePtr& node)
{
  for (auto n : node->entities)
    fetch_types_recursively(pro, n);
}

void MGProjectMerger::fetchTypes(MGProjectPtr pro)
{
  fetch_types_recursively(*pro, pro->modules);
}
