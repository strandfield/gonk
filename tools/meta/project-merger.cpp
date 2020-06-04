// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project-merger.h"

#include "database.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QFileInfo>

#include <QDebug>

void ProjectMerger::merge()
{
  fetchTypes(other);

  for (const auto& e : m_types_map)
  {
    auto t = e.second;

    if (e.first->is<Class>())
    {
      auto c = std::static_pointer_cast<Class>(e.first);

      if (!project->hasClassType(t->name))
      {
        QSqlQuery query = Database::exec(QString("INSERT INTO types(name, typeid, header, is_enum, is_class) VALUES('%1', '%2', '%3', 0, 1)")
          .arg(t->name, t->id, t->header));

        t->database_id = query.lastInsertId().toInt();
        c->type_id = t->database_id;

        project->types.classes.append(t);
        project->type_map[t->database_id] = t;
      }
    }
    else if (e.first->is<Enum>())
    {
      auto enm = std::static_pointer_cast<Enum>(e.first);

      if (!project->hasEnumType(t->name))
      {
        QSqlQuery query = Database::exec(QString("INSERT INTO types(name, typeid, header, is_enum, is_class) VALUES('%1', '%2', '%3', 1, 0)")
          .arg(t->name, t->id, t->header));

        t->database_id = query.lastInsertId().toInt();
        enm->type_id = t->database_id;

        project->types.enums.append(t);
        project->type_map[t->database_id] = t;
      }
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

    QSqlQuery query = Database::exec(QString("INSERT INTO modules(name) VALUES('%1')").arg(m->name));
    m->module_id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, module_id, 'order') VALUES(%1, %2, %3)")
      .arg(parentId(), QString::number(m->module_id), QString::number(m->order)));
    m->entity_id = query.lastInsertId().toInt();

    project->modules_map[m->module_id] = m;
  }
  else if (elem->is<File>())
  {
    FileRef f = std::static_pointer_cast<File>(elem);

    QSqlQuery query = Database::exec(QString("INSERT INTO files(name) VALUES('%1')").arg(f->name));
    f->file_id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, file_id, 'order') VALUES(%1, %2, %3)")
      .arg(parentId(), QString::number(f->file_id), QString::number(f->order)));
    f->entity_id = query.lastInsertId().toInt();

    project->files[f->file_id] = f;
  }
  else if (elem->is<Namespace>())
  {
    NamespaceRef ns = std::static_pointer_cast<Namespace>(elem);

    QSqlQuery query = Database::exec(QString("INSERT INTO namespaces(name) VALUES('%1')").arg(ns->name));
    ns->namespace_id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, namespace_id, 'order') VALUES(%1, %2, %3)")
      .arg(parentId(), QString::number(ns->namespace_id), QString::number(ns->order)));
    ns->entity_id = query.lastInsertId().toInt();

    project->namespaces[ns->namespace_id] = ns;
  }
  else if (elem->is<Class>())
  {
    ClassRef c = std::static_pointer_cast<Class>(elem);

    QSqlQuery query = Database::exec(QString("INSERT INTO classes(name, type, base) VALUES('%1', %2, '%3')")
      .arg(c->name, QString::number(c->type_id), c->base));
    c->class_id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, class_id, 'order') VALUES(%1, %2, %3)")
      .arg(parentId(), QString::number(c->class_id), QString::number(c->order)));
    c->entity_id = query.lastInsertId().toInt();

    project->classes[c->class_id] = c;
  }
  else if (elem->is<Enum>())
  {
    EnumRef e = std::static_pointer_cast<Enum>(elem);

    QSqlQuery query = Database::exec(QString("INSERT INTO enums(name, type) VALUES('%1', %2)").arg(e->name, QString::number(e->type_id)));
    e->enum_id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, enum_id, 'order') VALUES(%1, %2, %3)")
      .arg(parentId(), QString::number(e->enum_id), QString::number(e->order)));
    e->entity_id = query.lastInsertId().toInt();

    project->enums[e->enum_id] = e;
  }
  else if (elem->is<Enumerator>())
  {
    EnumeratorRef e = std::static_pointer_cast<Enumerator>(elem);
    EnumRef parent = std::static_pointer_cast<Enum>(m_parent);

    QSqlQuery query = Database::exec(QString("INSERT INTO enumerators(name, enum_id) VALUES('%1', %2)").arg(e->name, QString::number(parent->enum_id)));
    e->enumerator_id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, enumerator_id, 'order') VALUES(%1, %2, %3)")
      .arg(parentId(), QString::number(e->enumerator_id), QString::number(e->order)));
    e->entity_id = query.lastInsertId().toInt();

    project->enumerators[e->enumerator_id] = e;
  }
  else if (elem->is<Function>())
  {
    FunctionRef f = std::static_pointer_cast<Function>(elem);

    QSqlQuery query = Database::exec(QString("INSERT INTO functions(name, return_type, parameters, specifiers) VALUES('%1', '%2', '%3', '%4')")
      .arg(f->name, f->returnType, f->parameters.join(';'), f->getSpecifiers().join(',')));
    f->function_id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, function_id, 'order') VALUES(%1, %2, %3)")
      .arg(parentId(), QString::number(f->function_id), QString::number(f->order)));
    f->entity_id = query.lastInsertId().toInt();

    project->functions[f->function_id] = f;
  }
  else if (elem->is<Statement>())
  {
    StatementRef s = std::static_pointer_cast<Statement>(elem);

    QSqlQuery query = Database::exec(QString("INSERT INTO statements(content) VALUES('%1')").arg(s->name));
    s->statement_id = query.lastInsertId().toInt();

    query = Database::exec(QString("INSERT INTO entities(parent, statement_id, 'order') VALUES(%1, %2, %3)")
      .arg(parentId(), QString::number(s->statement_id), QString::number(s->order)));
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

  elem->order = list.size();
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

void ProjectMerger::fetch_types_recursively(Project& pro, std::vector<NodeRef>& stack, const NodeRef& node)
{
  if (node->is<Namespace>())
  {
    Namespace& ns = node->as<Namespace>();

    stack.push_back(node);
    fetch_types_recursively(pro, stack, ns.elements);
    stack.pop_back();
  }
  else if (node->is<Module>())
  {
    Module& m = node->as<Module>();

    stack.push_back(node);
    fetch_types_recursively(pro, stack, m.elements);
    stack.pop_back();
  }
  else if (node->is<Class>())
  {
    if (pro.hasClassType(node->name))
      return;

    const QString name = Node::nameQualification(stack) + node->name;
    auto type = std::make_shared<Type>(name, QString{ name }.remove("::"));
    type->is_class = true;
    m_types_map[node] = type;

    stack.push_back(node);
    fetch_types_recursively(pro, stack, node->as<Class>().elements);
    stack.pop_back();
  }
  else if (node->is<Enum>())
  {
    if (pro.hasEnumType(node->name))
      return;

    const QString name = Node::nameQualification(stack) + node->name;
    auto type = std::make_shared<Type>(name, QString{ name }.remove("::"));
    type->is_enum = true;
    m_types_map[node] = type;

    pro.types.enums.append(std::make_shared<Type>(name, QString{ name }.remove("::")));
  }
}

void ProjectMerger::fetchTypes(ProjectRef pro)
{
  std::vector<NodeRef> stack;
  fetch_types_recursively(*pro, stack, pro->modules);
}
