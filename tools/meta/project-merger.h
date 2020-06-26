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

class MGProjectMerger : QObject
{
  Q_OBJECT

    Q_PROPERTY(int importedSymbolsCount READ importedSymbolsCount NOTIFY importedSymbolsCountChanged)

public:
  QSqlDatabase& database;
  MGProjectPtr project;
  MGProjectPtr other;

  MGProjectMerger(QSqlDatabase& db, MGProjectPtr pro, MGProjectPtr o)
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
  T find_or_set(std::vector<T>& list, const std::shared_ptr<cxx::Entity>& elem)
  {
    for (const auto& item : list)
    {
      if (eq(item, elem))
        return item;
    }

    getIds(elem);
    list.push_back(elem);
    return elem;
  }

  QString parentId() const;
  void getIds(std::shared_ptr<cxx::Entity> elem);
  void getIds(MGModulePtr elem);

  void merge_recursively(std::vector<std::shared_ptr<cxx::Entity>>& target, const std::vector<std::shared_ptr<cxx::Entity>>& src);
  MGModulePtr find_or_set(std::vector<MGModulePtr>& list, const MGModulePtr& elem);
  void merge_recursively(std::vector<MGModulePtr>& target, const std::vector<MGModulePtr>& src);

  template<typename T>
  void assignIds(std::vector<std::shared_ptr<T>>& list)
  {
    for (int i(0); i < list.size(); ++i)
    {
      auto item = list.at(i);

      getIds(item);

      auto prev_parent = m_parent;
      m_parent = item;

      std::vector<std::shared_ptr<cxx::Entity>> children = ::children(*item);
      assignIds(children);

      m_parent = prev_parent;
    }
  }

protected:

  template<typename T>
  void fetch_types_recursively(MGProject& pro, const std::vector<T>& nodes)
  {
    for (const auto& n : nodes)
      fetch_types_recursively(pro, n);
  }

  void fetch_types_recursively(MGProject& pro, const std::shared_ptr<cxx::Entity>& node);
  void fetch_types_recursively(MGProject& pro, const MGModulePtr& node);

  void fetchTypes(MGProjectPtr pro);

private:
  MGModulePtr m_current_module = nullptr;
  std::shared_ptr<cxx::Entity> m_parent = nullptr;
  int m_imported_symbols_count = 0;
  std::unordered_map<std::shared_ptr<cxx::Entity>, MGTypePtr> m_types_map;
};

#endif // METAGONK_PROJECTMERGER_H
