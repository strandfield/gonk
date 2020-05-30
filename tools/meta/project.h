// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_PROJECT_H
#define METAGONK_PROJECT_H

#include <QSharedPointer>

#include "project/module.h"
#include "project/class.h"
#include "project/enum.h"
#include "project/file.h"
#include "project/function.h"
#include "project/namespace.h"
#include "project/statement.h"
#include "project/type.h"

class Project
{
public:
  struct Types
  {
    QList<Type> fundamentals;
    QList<Type> enums;
    QList<Type> classes;
  };

public:
  QList<ModuleRef> modules;
  Types types;
  std::map<int, ModuleRef> modules_map;
  std::map<int, FileRef> files;
  std::map<int, FunctionRef> functions;
  std::map<int, NamespaceRef> namespaces;
  std::map<int, ClassRef> classes;
  std::map<int, EnumRef> enums;
  std::map<int, EnumeratorRef> enumerators;
  std::map<int, StatementRef> statements;
  std::map<int, NodeRef> entities;

  QJsonObject toJson() const;
  static QSharedPointer<Project> fromJson(const QJsonObject & obj);
  static QSharedPointer<Project> load(const QString & filename);

  void save(const QString & filename);

  void removeUncheckedSymbols();
  void fetchTypes();

  bool hasEnumType(const QString & name) const;
  bool hasClassType(const QString & name) const;
  Type & getType(const QString & name);

  template<typename T>
  QSharedPointer<T> get(const QString & name)
  {
    for (const auto & e : modules)
    {
      if (e->is<T>() && e->name == name)
        return QSharedPointer<T>{e};
    }

    auto ret = QSharedPointer<T>::create(name);
    modules.append(ret);
    return ret;
  }

  static void sort(QList<Type> & types);

  int fileCount() const;
};

typedef QSharedPointer<Project> ProjectRef;

Q_DECLARE_METATYPE(QSharedPointer<Project>);

#endif // METAGONK_PROJECT_H
