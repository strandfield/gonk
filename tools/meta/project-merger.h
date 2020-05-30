// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_PROJECTMERGER_H
#define METAGONK_PROJECTMERGER_H

#include "project.h"

#include <QObject>

#include <map>
#include <memory>

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

    getIds(elem);
    list.append(elem);
    return elem;
  }

  QString parentId() const;
  void getIds(NodeRef elem);

  void merge_recursively(QList<NodeRef>& target, const QList<NodeRef>& src);
  ModuleRef find_or_set(QList<ModuleRef>& list, const ModuleRef& elem);
  void merge_recursively(QList<ModuleRef>& target, const QList<ModuleRef>& src);

  QList<NodeRef> getChildren(const NodeRef& node);

  template<typename T>
  void assignIds(QList<T>& list)
  {
    for (const auto& item : list)
    {
      getIds(item);

      RAIINodeGuard guard{ m_parent };
      m_parent = item;

      QList<NodeRef> children = getChildren(item);
      assignIds(children);
    }
  }

private:
  NodeRef m_parent = nullptr;
  int m_imported_symbols_count = 0;
};

#endif // METAGONK_PROJECTMERGER_H
