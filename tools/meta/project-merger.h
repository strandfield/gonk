// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_PROJECTMERGER_H
#define METAGONK_PROJECTMERGER_H

#include "project.h"

#include <QObject>

#include <map>
#include <memory>
#include <unordered_map>

class QFileInfo;

class QSqlDatabase;

class ProjectMerger : QObject
{
  Q_OBJECT

  Q_PROPERTY(int importedSymbolsCount READ importedSymbolsCount NOTIFY importedSymbolsCountChanged)

public:
  QSqlDatabase& database;
  ProjectRef project;
  ProjectRef other;

  ProjectMerger(QSqlDatabase & db, ProjectRef pro, ProjectRef o)
    : database(db), project(pro), other(o)
  {
  }

  void merge();
  int importedSymbolsCount() const;

Q_SIGNALS:
  void importedSymbolsCountChanged();

protected:
  void incrImportedSymbolsCount();

protected:

  template<typename T>
  T find_or_set(QList<T>& list, const NodeRef& elem)
  {
    for (const auto& item : list)
    {
      if (eq(item, elem))
        return item;
    }

    elem->order = list.size();
    getIds(elem);
    list.append(elem);
    return elem;
  }

  QString parentId() const;
  void getIds(NodeRef elem);

  void merge_recursively(QList<NodeRef>& target, const QList<NodeRef>& src);
  ModuleRef find_or_set(QList<ModuleRef>& list, const ModuleRef& elem);
  void merge_recursively(QList<ModuleRef>& target, const QList<ModuleRef>& src);

  template<typename T>
  void assignIds(QList<T>& list)
  {
    for (int i(0); i < list.size(); ++i)
    {
      auto item = list.at(i);
      item->order = i;

      getIds(item);

      RAIINodeGuard guard{ m_parent };
      m_parent = item;

      QList<NodeRef> children = item->children();
      assignIds(children);
    }
  }

protected:

  template<typename T>
  void fetch_types_recursively(Project& pro, std::vector<NodeRef>& stack, const QList<T>& nodes)
  {
    for (const auto& n : nodes)
      fetch_types_recursively(pro, stack, n);
  }

  void fetch_types_recursively(Project& pro, std::vector<NodeRef>& stack, const NodeRef& node);

  void fetchTypes(ProjectRef pro);

private:
  NodeRef m_parent = nullptr;
  int m_imported_symbols_count = 0;
  std::unordered_map<NodeRef, TypePtr> m_types_map;
};

#endif // METAGONK_PROJECTMERGER_H
