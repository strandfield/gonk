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
#include <QSettings>

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
    int pname_index = pstr.indexOf("@");
    int pvalue_index = pstr.indexOf("#");

    if (pname_index == -1)
      pname_index = pstr.length();
    if (pvalue_index == -1)
      pvalue_index = pstr.length();

    QString ptype_str = pstr.mid(0, std::min(pname_index, pvalue_index));
    QString pname = pstr.mid(pname_index + 1, pvalue_index - pname_index - 1);
    QString pvalue = pstr.mid(pvalue_index + 1);

    cxx::Type ptype{ ptype_str.toStdString() };
    std::string name{ pname.toStdString() };
    std::string default_value{ pvalue.toStdString() };
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
    else if (sp == "noexcept")
      f.specifiers |= cxx::FunctionSpecifier::Noexcept;
    else if (sp == "virtual")
      f.specifiers |= cxx::FunctionSpecifier::Virtual;
    else if (sp == "ctor")
      f.kind = cxx::FunctionKind::Constructor;
    else if (sp == "dtor")
      f.kind = cxx::FunctionKind::Destructor;  
  }
}

void MGProjectLoader::loadIni()
{
  QFileInfo fileinfo{ database.databaseName() };
  QSettings config{ fileinfo.absolutePath() + "/" + "module_info.ini", QSettings::IniFormat };
  project->module_name = config.value("module_name", "").toString().toStdString();
  project->module_folder = config.value("module_folder", "").toString().toStdString();
  project->module_namespace = config.value("module_namespace", "").toString().toStdString();
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

  QSqlQuery query = database.exec("SELECT id, name, enum_class, type FROM enums");

  int ID = 0;
  int NAME = 1;
  int ENUM_CLASS = 2;
  int TYPE = 3;

  while (query.next())
  {
    int id = query.value(ID).toInt();

    auto enm = std::make_shared<cxx::Enum>(query.value(NAME).toString().toStdString());
    enm->enum_class = query.value(ENUM_CLASS).toInt() != 0;

    int type_id = query.value(TYPE).toInt();
    if (type_id != -1)
    {
      project->entity_type_map[enm] = m_type_map.at(type_id);
    }
    else
    {
      qDebug() << "Enum is not associated with a type: " << enm->name.c_str();
    }

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

    auto& json_obj = [&]() -> json::Object& {
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

    json::Json json_value = [&]() -> json::Json {
      if (value == "true")
        return true;
      else if (value == "false")
        return false;
      else if (value.at(0) == '\"')
        return std::string(value.begin() + 1, value.end() - 1);
      else if (value.at(0) == '[' || value.at(0) == '{')
        return json::parse(value);
      else
        return std::stoi(value);
    }();

    json_obj[key] = json_value;
  }
}

void MGProjectLoader::buildEntityTree()
{
  setState("building tree");

  std::list<std::shared_ptr<cxx::Entity>> queue;

  QSqlQuery query{ database };
  bool ok = query.prepare(
    "SELECT entities.id FROM entities"
    " LEFT JOIN source_locations ON entities.id = source_locations.entity_id"
    " LEFT JOIN files ON source_locations.file_id = files.id"
    " WHERE parent = :parent_id"
    " ORDER BY files.path ASC, source_locations.line ASC");

  if (!ok)
  {
    qDebug() << query.lastError().text();
    return;
  }

  int ID = 0;

  for (const auto& e : m_modules_map)
  {
    project->modules.push_back(e.second);

    query.bindValue(":parent_id", project->dbid(e.second).global_id);
    query.exec();

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

    query.bindValue(":parent_id", project->dbid(n).global_id);
    query.exec();

    while (query.next())
    {
      int child_id = query.value(ID).toInt();
      auto child = m_entity_map.at(child_id);
      n->appendChild(child);
      // @TODO: should have been done by appendChild()
      child->weak_parent = n;

      queue.push_back(child);
    }
  }
}

void MGProjectLoader::loadFiles()
{
  setState("loading files");

  QSqlQuery query = database.exec("SELECT id, path FROM files");

  int ID = 0;
  int PATH = 1;

  while (query.next())
  {
    int id = query.value(ID).toInt();

    auto f = std::make_shared<cxx::File>(query.value(PATH).toString().toStdString());

    m_files_map[id] = f;
  }
}

void MGProjectLoader::loadSourceLocations()
{
  setState("loading source locations");

  QSqlQuery query = database.exec("SELECT entity_id, file_id, line, column FROM source_locations");

  int ENTITY_ID = 0;
  int FILE_ID = 1;
  int LINE = 2;
  int COLUMN = 3;

  while (query.next())
  {
    auto e = m_entity_map.at(query.value(ENTITY_ID).toInt());
    auto f = m_files_map.at(query.value(FILE_ID).toInt());

    e->location = cxx::SourceLocation(f, query.value(LINE).toInt(), query.value(COLUMN).toInt());
  }
}
