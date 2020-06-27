// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project-loader.h"

#include "database.h"

#include <json-toolkit/parsing.h>

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QFileInfo>

#include <QDebug>

QString MGProjectLoader::state() const
{
  return m_state;
}

void MGProjectLoader::setState(const QString& st)
{
  m_state = st;
  Q_EMIT stateChanged();
}

void MGProjectLoader::write(std::vector<std::shared_ptr<cxx::FunctionParameter>>& o_params, QString param_list_str)
{
  if (param_list_str.isEmpty())
    return;

  QStringList plist = param_list_str.split(";", QString::SkipEmptyParts);

  for (QString pstr : plist)
  {
    QStringList type_and_rest = pstr.split("@", QString::SkipEmptyParts);
    QStringList name_and_default = type_and_rest.size() == 1 ? QStringList() : type_and_rest.at(1).split("#", QString::SkipEmptyParts);
    cxx::Type ptype{ type_and_rest.at(0).toStdString() };
    std::string name{ name_and_default.isEmpty() ? "" : name_and_default.at(0).toStdString() };
    std::string default_value{ name_and_default.size() == 2 ? name_and_default.at(1).toStdString() : "" };
    auto p = std::make_shared<cxx::FunctionParameter>(ptype, std::move(name));
    p->default_value = cxx::Expression(std::move(default_value));
    o_params.push_back(p);
  }
}

void MGProjectLoader::writeSpecifiers(cxx::Function& f, QString specifiers)
{
  QStringList specs = specifiers.split(",", QString::SkipEmptyParts);

  for (QString sp : specs)
  {
    if (sp == "const")
      f.specifiers |= cxx::FunctionSpecifier::Const;
    else if (sp == "delete")
      f.specifiers |= cxx::FunctionSpecifier::Delete;
    else if (sp == "explicit")
      f.specifiers |= cxx::FunctionSpecifier::Explicit;
    else if (sp == "static")
      f.specifiers |= cxx::FunctionSpecifier::Static;
    else if (sp == "ctor")
      f.kind = cxx::FunctionKind::Constructor;
    else if (sp == "dtor")
      f.kind = cxx::FunctionKind::Destructor;  
  }
}

void MGProjectLoader::loadTypes()
{
  setState("loading types");

  QSqlQuery query = database.exec("SELECT * FROM types WHERE is_enum = 0 AND is_class = 0");

  while (query.next())
  {
    auto result = std::make_shared<MGType>();
    result->database_id = query.value("id").toInt();
    result->name = query.value("name").toString().toStdString();
    result->id = query.value("typeid").toString().toStdString();

    project->types.push_back(result);
    m_type_map[result->database_id] = result;
  }

  query = database.exec("SELECT * FROM types WHERE is_enum = 1 AND is_class = 0");

  while (query.next())
  {
    auto result = std::make_shared<MGType>();
    result->database_id = query.value("id").toInt();
    result->name = query.value("name").toString().toStdString();
    result->id = query.value("typeid").toString().toStdString();
    result->category = MGType::EnumType;

    project->types.push_back(result);
    m_type_map[result->database_id] = result;
  }

  query = database.exec("SELECT * FROM types WHERE is_enum = 0 AND is_class = 1");

  while (query.next())
  {
    auto result = std::make_shared<MGType>();
    result->database_id = query.value("id").toInt();
    result->name = query.value("name").toString().toStdString();
    result->id = query.value("typeid").toString().toStdString();
    result->category = MGType::ClassType;

    project->types.push_back(result);
    m_type_map[result->database_id] = result;
  }
}

void MGProjectLoader::loadEntities()
{
  loadFunctions();
  loadClasses();
  loadEnums();
  loadEnumerators();
  loadModules();

  {
    QSqlQuery query = database.exec("SELECT id, module_id, namespace_id, class_id, function_id, enum_id, enumerator_id FROM entities");

    int ID = 0;
    int MODULE_ID = 1;
    int NAMESPACE_ID = 2;
    int CLASS_ID = 3;
    int FUNCTION_ID = 4;
    int ENUM_ID = 5;
    int ENUMERATOR_ID = 6;

    while (query.next())
    {
      int entity_id = query.value(ID).toInt();

      if (!query.value(MODULE_ID).isNull())
      {
        MGModulePtr m = m_modules_map[query.value(MODULE_ID).toInt()];
        project->dbid(m).global_id = entity_id;
      }
      else if (!query.value(NAMESPACE_ID).isNull())
      {
        std::shared_ptr<cxx::Namespace> ns = m_namespaces_map[query.value(NAMESPACE_ID).toInt()];
        project->dbid(ns).global_id = entity_id;

        m_entity_map[entity_id] = ns;
      }
      else if (!query.value(CLASS_ID).isNull())
      {
        std::shared_ptr<cxx::Class> c = m_classes_map[query.value(CLASS_ID).toInt()];
        project->dbid(c).global_id = entity_id;

        m_entity_map[entity_id] = c;

      }
      else if (!query.value(FUNCTION_ID).isNull())
      {
        std::shared_ptr<cxx::Function> fun = m_functions_map[query.value(FUNCTION_ID).toInt()];

        project->dbid(fun).global_id = entity_id;

        m_entity_map[entity_id] = fun;

      }
      else if (!query.value(ENUM_ID).isNull())
      {
        std::shared_ptr<cxx::Enum> enm = m_enums_map[query.value(ENUM_ID).toInt()];
        project->dbid(enm).global_id = entity_id;

        m_entity_map[entity_id] = enm;

      }
      else if (!query.value(ENUMERATOR_ID).isNull())
      {
        std::shared_ptr<cxx::EnumValue> enm = m_enumerators_map[query.value(ENUMERATOR_ID).toInt()];

        project->dbid(enm).global_id = entity_id;

       m_entity_map[entity_id] = enm;

      }
    }
  }

  loadMetadata();

  buildEntityTree();
}

void MGProjectLoader::loadModules()
{
  setState("loading modules");

  QSqlQuery query = database.exec("SELECT id, name FROM modules");

  int ID = 0;
  int NAME = 1;

  while (query.next())
  {
    int id = query.value(ID).toInt();
    auto m = std::make_shared<MGModule>("");
    m->name = query.value(NAME).toString().toStdString();
    m_modules_map[id] = m;
    project->dbid(m).id = id;
  }
}

void MGProjectLoader::loadFunctions()
{
  setState("loading functions");

  QSqlQuery query = Database::exec("SELECT id, name, return_type, parameters, specifiers FROM functions");

  int ID = 0;
  int NAME = 1;
  int RETURN_TYPE = 2;
  int PARAMETERS = 3;
  int SPECIFIERS = 4;

  while (query.next())
  {
    int id = query.value(ID).toInt();

    auto fun = std::make_shared<cxx::Function>(query.value(NAME).toString().toStdString());
    fun->return_type = cxx::Type(query.value(RETURN_TYPE).toString().toStdString());
    write(fun->parameters, query.value(PARAMETERS).toString().simplified());

    writeSpecifiers(*fun, query.value(SPECIFIERS).toString());

    m_functions_map[id] = fun;
    project->dbid(fun).id = id;
  }
}

void MGProjectLoader::loadClasses()
{
  setState("loading classes");

  QSqlQuery query = database.exec("SELECT id, name, type, base, final FROM classes");

  int ID = 0;
  int NAME = 1;
  int TYPE = 2;
  int BASE = 3;
  int FINAL = 4;

  while (query.next())
  {
    int id = query.value(ID).toInt();

    auto c = std::make_shared<cxx::Class>(query.value(NAME).toString().toStdString());
    c->is_final = query.value(FINAL).toInt();

    if (!query.value(BASE).toString().isEmpty())
    {
      // TODO: try to link to the actual class, if it exists
      cxx::BaseClass base;
      base.base = std::make_shared<cxx::Class>(query.value(BASE).toString().toStdString());
      c->bases.push_back(std::move(base));
    }

    project->entity_type_map[c] = m_type_map.at(query.value(TYPE).toInt());

    m_classes_map[id] = c;
    project->dbid(c).id = id;
  }
}

void MGProjectLoader::loadEnums()
{
  setState("loading enums");

  QSqlQuery query = database.exec("SELECT id, name, type FROM enums");

  int ID = 0;
  int NAME = 1;
  int TYPE = 2;

  while (query.next())
  {
    int id = query.value(ID).toInt();

    auto enm = std::make_shared<cxx::Enum>(query.value(NAME).toString().toStdString());

    project->entity_type_map[enm] = m_type_map.at(query.value(TYPE).toInt());

    m_enums_map[id] = enm;
    project->dbid(enm).id = id;
  }
}

void MGProjectLoader::loadEnumerators()
{
  setState("loading enumerators");

  QSqlQuery query = database.exec("SELECT id, name FROM enumerators");

  int ID = 0;
  int NAME = 1;

  while (query.next())
  {
    int id = query.value(ID).toInt();

    auto enm = std::make_shared<cxx::EnumValue>(query.value(NAME).toString().toStdString());

    m_enumerators_map[id] = enm;
    project->dbid(enm).id = id;
  }
}

void MGProjectLoader::loadMetadata()
{
  setState("loading metadata");

  QSqlQuery query = database.exec("SELECT entity_id, name, value FROM metadata");

  int ENTITY_ID = 0;
  int NAME = 1;
  int VALUE = 2;

  while (query.next())
  {
    int id = query.value(ENTITY_ID).toInt();

    json::Object& json_obj = [&]() {
      {
        auto it = m_entity_map.find(id);

        if (it != m_entity_map.end())
        {
          std::shared_ptr<cxx::Entity> e = it->second;
          return project->getMetadata(e);
        }
      }

      {
        auto it = m_modules_map.find(id);

        if (it != m_modules_map.end())
        {
          MGModulePtr e = it->second;
          return project->getMetadata(e);
        }
      }

      throw std::runtime_error{ "No such entity" };
    }();

    std::string key = query.value(NAME).toString().toStdString();
    std::string value = query.value(VALUE).toString().toStdString();

    json::Json json_value = json::parse(value);

    json_obj[key] = json_value;
  }
}

void MGProjectLoader::buildEntityTree()
{
  setState("building tree");

  std::list<std::shared_ptr<cxx::Entity>> queue;

  for (const auto& e : m_modules_map)
  {
    project->modules.push_back(e.second);

    QSqlQuery query = database.exec(QString("SELECT id FROM entities WHERE parent = %1").arg(project->dbid(e.second).global_id));

    int ID = 0;

    while (query.next())
    {
      int child_id = query.value(ID).toInt();
      auto child = m_entity_map[child_id];
      e.second->entities.push_back(child);

      queue.push_back(child);
    }
  }

  while (!queue.empty())
  {
    std::shared_ptr<cxx::Entity> n = queue.front();
    queue.pop_front();

    QSqlQuery query = database.exec(QString("SELECT id FROM entities WHERE parent = %1").arg(project->dbid(n).global_id));

    int ID = 0;

    while (query.next())
    {
      int child_id = query.value(ID).toInt();
      auto child = m_entity_map[child_id];
      n->appendChild(child);

      queue.push_back(child);
    }
  }
}
