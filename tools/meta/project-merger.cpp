// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project-merger.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QFileInfo>

#include <QDebug>

void ProjectMerger::merge()
{
  for (auto t : other->types.classes)
  {
    if (!project->hasClassType(t->name))
    {
      QSqlQuery query = database.exec(QString("INSERT INTO types(name, typeid, header, is_enum, is_class) VALUES('%1', '%2', '%3', 0, 1)")
        .arg(t->name, t->id, t->header));

      t->database_id = query.lastInsertId().toInt();

      project->types.classes.append(t);
    }
  }

  for (auto t : other->types.enums)
  {
    if (!project->hasEnumType(t->name))
    {
      QSqlQuery query = database.exec(QString("INSERT INTO types(name, typeid, header, is_enum, is_class) VALUES('%1', '%2', '%3', 1, 0)")
        .arg(t->name, t->id, t->header));

      t->database_id = query.lastInsertId().toInt();

      project->types.enums.append(t);
    }
  }

  merge_recursively(project->modules, other->modules);
}

int ProjectMerger::importedSymbolsCount() const
{
  return m_imported_symbols_count;
}

void ProjectMerger::incrImportedSymbolsCount() 
{
  m_imported_symbols_count++;
  Q_EMIT importedSymbolsCountChanged();
}

QString ProjectMerger::parentId() const
{
  return m_parent == nullptr ? QString("NULL") : QString::number(m_parent->entity_id);
}

void ProjectMerger::getIds(NodeRef elem)
{
  if (elem->entity_id != -1)
    return;

  if (elem->is<Module>())
  {
    ModuleRef m = std::static_pointer_cast<Module>(elem);

    QSqlQuery query = database.exec(QString("INSERT INTO modules(name) VALUES('%1')").arg(m->name));
    m->module_id = query.lastInsertId().toInt();

    query = database.exec(QString("INSERT INTO entities(parent, module_id) VALUES(%1, %2)").arg(parentId(), QString::number(m->module_id)));
    m->entity_id = query.lastInsertId().toInt();

    project->modules_map[m->module_id] = m;
  }
  else if (elem->is<File>())
  {
    FileRef f = std::static_pointer_cast<File>(elem);

    QSqlQuery query = database.exec(QString("INSERT INTO files(name) VALUES('%1')").arg(f->name));
    f->file_id = query.lastInsertId().toInt();

    query = database.exec(QString("INSERT INTO entities(parent, file_id) VALUES(%1, %2)").arg(parentId(), QString::number(f->file_id)));
    f->entity_id = query.lastInsertId().toInt();

    project->files[f->file_id] = f;
  }
  else if (elem->is<Namespace>())
  {
    NamespaceRef ns = std::static_pointer_cast<Namespace>(elem);

    QSqlQuery query = database.exec(QString("INSERT INTO namespaces(name) VALUES('%1')").arg(ns->name));
    ns->namespace_id = query.lastInsertId().toInt();

    query = database.exec(QString("INSERT INTO entities(parent, namespace_id) VALUES(%1, %2)").arg(parentId(), QString::number(ns->namespace_id)));
    ns->entity_id = query.lastInsertId().toInt();

    project->namespaces[ns->namespace_id] = ns;
  }
  else if (elem->is<Class>())
  {
    ClassRef c = std::static_pointer_cast<Class>(elem);

    QSqlQuery query = database.exec(QString("INSERT INTO classes(name) VALUES('%1')").arg(c->name));
    c->class_id = query.lastInsertId().toInt();

    query = database.exec(QString("INSERT INTO entities(parent, class_id) VALUES(%1, %2)").arg(parentId(), QString::number(c->class_id)));
    c->entity_id = query.lastInsertId().toInt();

    project->classes[c->class_id] = c;
  }
  else if (elem->is<Enum>())
  {
    EnumRef e = std::static_pointer_cast<Enum>(elem);

    QSqlQuery query = database.exec(QString("INSERT INTO enums(name) VALUES('%1')").arg(e->name));
    e->enum_id = query.lastInsertId().toInt();

    query = database.exec(QString("INSERT INTO entities(parent, enum_id) VALUES(%1, %2)").arg(parentId(), QString::number(e->enum_id)));
    e->entity_id = query.lastInsertId().toInt();

    project->enums[e->enum_id] = e;
  }
  else if (elem->is<Enumerator>())
  {
    EnumeratorRef e = std::static_pointer_cast<Enumerator>(elem);
    EnumRef parent = std::static_pointer_cast<Enum>(m_parent);

    QSqlQuery query = database.exec(QString("INSERT INTO enumerators(name, enum_id) VALUES('%1', %2)").arg(e->name, QString::number(parent->enum_id)));
    e->enumerator_id = query.lastInsertId().toInt();

    query = database.exec(QString("INSERT INTO entities(parent, enumerator_id) VALUES(%1, %2)").arg(parentId(), QString::number(e->enumerator_id)));
    e->entity_id = query.lastInsertId().toInt();

    project->enumerators[e->enumerator_id] = e;
  }
  else if (elem->is<Function>())
  {
    FunctionRef f = std::static_pointer_cast<Function>(elem);

    QSqlQuery query = database.exec(QString("INSERT INTO functions(name, return_type, parameters, specifiers) VALUES('%1', '%2', '%3', '%4')")
      .arg(f->name, f->returnType, f->parameters.join(';'), f->getSpecifiers().join(',')));
    f->function_id = query.lastInsertId().toInt();

    query = database.exec(QString("INSERT INTO entities(parent, function_id) VALUES(%1, %2)").arg(parentId(), QString::number(f->function_id)));
    f->entity_id = query.lastInsertId().toInt();

    project->functions[f->function_id] = f;
  }
  else if (elem->is<Statement>())
  {
    StatementRef s = std::static_pointer_cast<Statement>(elem);

    QSqlQuery query = database.exec(QString("INSERT INTO statements(content) VALUES('%1')").arg(s->name));
    s->statement_id = query.lastInsertId().toInt();

    query = database.exec(QString("INSERT INTO entities(parent, statement_id) VALUES(%1, %2)").arg(parentId(), QString::number(s->statement_id)));
    s->entity_id = query.lastInsertId().toInt();

    project->statements[s->statement_id] = s;
  }
  else
  {
    throw std::runtime_error{ "Not implemented" };
  }

  incrImportedSymbolsCount();
}

void ProjectMerger::merge_recursively(QList<NodeRef>& target, const QList<NodeRef>& src)
{
  for (const auto& srcItem : src)
  {
    if (srcItem->checkState == Qt::Unchecked)
      continue;

    NodeRef node = find_or_set(target, srcItem);

    RAIINodeGuard guard{ m_parent };
    m_parent = node;

    if (node == srcItem)
    {
      auto children = node->children();
      assignIds(children);
      continue;
    }

    if (node->is<Module>())
    {
      ModuleRef targetModule = std::static_pointer_cast<Module>(node);
      ModuleRef srcModule = std::static_pointer_cast<Module>(srcItem);

      merge_recursively(targetModule->elements, srcModule->elements);
    }
    else if (node->is<Namespace>())
    {
      NamespaceRef targetNamespace = std::static_pointer_cast<Namespace>(node);
      NamespaceRef srcNamespace = std::static_pointer_cast<Namespace>(srcItem);

      merge_recursively(targetNamespace->elements, srcNamespace->elements);
    }
    else if (node->is<Class>())
    {
      ClassRef target_class = std::static_pointer_cast<Class>(node);
      ClassRef src_class = std::static_pointer_cast<Class>(srcItem);

      merge_recursively(target_class->elements, src_class->elements);
    }
    else if (node->is<Enum>())
    {
      EnumRef target_enum = std::static_pointer_cast<Enum>(node);
      EnumRef src_enum = std::static_pointer_cast<Enum>(srcItem);

      target_enum->merge(*src_enum);
    }
    else
    {
      qDebug() << "Element already exists in project";
    }
  }
}

ModuleRef ProjectMerger::find_or_set(QList<ModuleRef>& list, const ModuleRef& elem)
{
  for (const auto& item : list)
  {
    if (item->name == elem->name)
      return item;
  }

  getIds(elem);
  list.append(elem);
  return elem;
}

void ProjectMerger::merge_recursively(QList<ModuleRef>& target, const QList<ModuleRef>& src)
{
  for (const auto& srcItem : src)
  {
    if (srcItem->checkState == Qt::Unchecked)
      continue;

    ModuleRef m = find_or_set(target, srcItem);

    RAIINodeGuard guard{ m_parent };
    m_parent = m;

    if (m == srcItem)
      assignIds(m->elements);
    else
      merge_recursively(m->elements, srcItem->elements);
  }
}
