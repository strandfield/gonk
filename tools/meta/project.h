// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_PROJECT_H
#define METAGONK_PROJECT_H

#include "project/module.h"
#include "project/class.h"
#include "project/enum.h"
#include "project/file.h"
#include "project/function.h"
#include "project/namespace.h"
#include "project/statement.h"
#include "project/type.h"

#include <map>
#include <memory>

class Project
{
public:
  struct Types
  {
    QList<std::shared_ptr<Type>> fundamentals;
    QList<std::shared_ptr<Type>> enums;
    QList<std::shared_ptr<Type>> classes;
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
  std::map<int, std::shared_ptr<Type>> type_map;

  void removeUncheckedSymbols();

  bool hasEnumType(const QString & name) const;
  bool hasClassType(const QString & name) const;
  Type & getType(const QString & name);
  std::shared_ptr<Type> getType(int id) const;

  NodeRef getSymbol(const QString& module_name, const QString& name) const;

  template<typename T>
  std::shared_ptr<T> get(const QString & name)
  {
    for (const auto & e : modules)
    {
      if (e->is<T>() && e->name == name)
        return std::static_pointer_cast<T>(e);
    }

    auto ret = std::make_shared<T>(name);
    modules.append(ret);
    return ret;
  }

  static void sort(QList<Type> & types);

  int fileCount() const;
};

typedef std::shared_ptr<Project> ProjectRef;

#endif // METAGONK_PROJECT_H
