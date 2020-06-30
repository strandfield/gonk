// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_PROJECTLOADER_H
#define METAGONK_PROJECTLOADER_H

#include "project.h"

#include <cxx/class.h>
#include <cxx/enum.h>
#include <cxx/function.h>
#include <cxx/namespace.h>

#include <QObject>

#include <map>
#include <memory>

class QFileInfo;

class QSqlDatabase;

class MGProjectLoader : QObject
{
  Q_OBJECT

    Q_PROPERTY(QString state READ state NOTIFY stateChanged)

public:
  QSqlDatabase& database;
  MGProjectPtr project;

  MGProjectLoader(QSqlDatabase& db)
    : database(db)
  {
    project = std::make_shared<MGProject>();
  }

  void load()
  {
    loadTypes();
    loadEntities();
    project->sort();
  }

  QString state() const;

  // TODO: move these elsewhete to a better place, they shouldn't be public
  static void write(std::vector<std::shared_ptr<cxx::FunctionParameter>>& o_params, QString param_list_str);
  static void writeSpecifiers(cxx::Function& f, QString specifiers);

Q_SIGNALS:
  void stateChanged();

protected:
  void setState(const QString& st);

  void loadTypes();
  void loadEntities();
  void loadModules();
  void loadFunctions();
  void loadClasses();
  void loadEnums();
  void loadEnumerators();
  void loadMetadata();
  void buildEntityTree();

private:
  QString m_state;
  std::map<int, MGTypePtr> m_type_map;
  std::map<int, MGModulePtr> m_modules_map;
  std::map<int, std::shared_ptr<cxx::Entity>> m_entity_map;
  std::map<int, std::shared_ptr<cxx::Function>> m_functions_map;
  std::map<int, std::shared_ptr<cxx::Namespace>> m_namespaces_map;
  std::map<int, std::shared_ptr<cxx::Class>> m_classes_map;
  std::map<int, std::shared_ptr<cxx::Enum>> m_enums_map;
  std::map<int, std::shared_ptr<cxx::EnumValue>> m_enumerators_map;
};

#endif // METAGONK_PROJECTLOADER_H
