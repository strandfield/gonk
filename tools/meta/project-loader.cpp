// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project-loader.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QFileInfo>

#include <QDebug>

QString ProjectLoader::state() const
{
  return m_state;
}

void ProjectLoader::setState(const QString& st)
{
  m_state = st;
  Q_EMIT stateChanged();
}

void ProjectLoader::loadTypes()
{
  setState("loading types");

  QSqlQuery query = database.exec("SELECT * FROM types WHERE is_enum = 0 AND is_class = 0");

  while (query.next())
  {
    Type result;
    result.database_id = query.value("id").toInt();
    result.name = query.value("name").toString();
    result.id = query.value("typeid").toString();

    project->types.fundamentals.append(result);
  }

  query = database.exec("SELECT * FROM types WHERE is_enum = 1 AND is_class = 0");

  while (query.next())
  {
    Type result;
    result.database_id = query.value("id").toInt();
    result.name = query.value("name").toString();
    result.id = query.value("typeid").toString();

    project->types.enums.append(result);
  }

  query = database.exec("SELECT * FROM types WHERE is_enum = 0 AND is_class = 1");

  while (query.next())
  {
    Type result;
    result.database_id = query.value("id").toInt();
    result.name = query.value("name").toString();
    result.id = query.value("typeid").toString();

    project->types.classes.append(result);
  }
}

void ProjectLoader::loadEntities()
{
  loadFunctions();
  loadClasses();
  loadEnums();
  loadEnumerators();
  loadStatements();
  loadFiles();
  loadModules();

  {
    QSqlQuery query = database.exec("SELECT id, module_id, file_id, namespace_id, class_id, function_id, enum_id, enumerator_id, statement_id, condition, renaming, binding FROM entities ORDER BY 'order' ASC");

    int ID = 0;
    int MODULE_ID = 1;
    int FILE_ID = 2;
    int NAMESPACE_ID = 3;
    int CLASS_ID = 4;
    int FUNCTION_ID = 5;
    int ENUM_ID = 6;
    int ENUMERATOR_ID = 7;
    int STATEMENT_ID = 8;
    int CONDITION = 9;
    int RENAMING = 10;
    int BINDING = 11;

    while (query.next())
    {
      int entity_id = query.value(ID).toInt();

      if (!query.value(MODULE_ID).isNull())
      {
        ModuleRef m = project->modules_map[query.value(MODULE_ID).toInt()];
        m->entity_id = entity_id;
        project->entities[entity_id] = m;
      }
      else if (!query.value(FILE_ID).isNull())
      {
        FileRef f = project->files[query.value(FILE_ID).toInt()];
        f->entity_id = entity_id;
        project->entities[entity_id] = f;

        f->condition = query.value(CONDITION).toString().toStdString();
      }
      else if (!query.value(NAMESPACE_ID).isNull())
      {
        NamespaceRef ns = project->namespaces[query.value(NAMESPACE_ID).toInt()];
        ns->entity_id = entity_id;
        project->entities[entity_id] = ns;
      }
      else if (!query.value(CLASS_ID).isNull())
      {
        ClassRef c = project->classes[query.value(CLASS_ID).toInt()];
        c->entity_id = entity_id;
        project->entities[entity_id] = c;

        c->condition = query.value(CONDITION).toString().toStdString();
      }
      else if (!query.value(FUNCTION_ID).isNull())
      {
        FunctionRef fun = project->functions[query.value(FUNCTION_ID).toInt()];
        fun->entity_id = entity_id;
        project->entities[entity_id] = fun;

        fun->condition = query.value(CONDITION).toString().toStdString();
        fun->bindingMethod = Function::deserialize<Function::BindingMethod>(query.value(BINDING).toString());
      }
      else if (!query.value(ENUM_ID).isNull())
      {
        EnumRef enm = project->enums[query.value(ENUM_ID).toInt()];
        enm->entity_id = entity_id;
        project->entities[entity_id] = enm;

        enm->condition = query.value(CONDITION).toString().toStdString();
      }
      else if (!query.value(ENUMERATOR_ID).isNull())
      {
        EnumeratorRef enm = project->enumerators[query.value(ENUMERATOR_ID).toInt()];
        enm->entity_id = entity_id;
        project->entities[entity_id] = enm;

        enm->condition = query.value(CONDITION).toString().toStdString();
      }
      else if (!query.value(STATEMENT_ID).isNull())
      {
        StatementRef s = project->statements[query.value(STATEMENT_ID).toInt()];
        s->entity_id = entity_id;
        project->entities[entity_id] = s;

        s->condition = query.value(CONDITION).toString().toStdString();
      }
    }
  }

  buildEntityTree();
}

void ProjectLoader::loadModules()
{
  setState("loading modules");

  QSqlQuery query = database.exec("SELECT id, name FROM modules");

  int ID = 0;
  int NAME = 1;

  while (query.next())
  {
    auto m = std::make_shared<Module>("");
    m->module_id = query.value(ID).toInt();
    m->name = query.value(NAME).toString();

    project->modules_map[m->module_id] = m;
  }
}

void ProjectLoader::loadFiles()
{
  setState("loading files");

  QSqlQuery query = database.exec("SELECT id, name FROM files");

  int ID = 0;
  int NAME = 1;

  while (query.next())
  {
    auto f = std::make_shared<File>("");
    f->file_id = query.value(ID).toInt();
    f->name = query.value(NAME).toString();

    project->files[f->file_id] = f;
  }
}

void ProjectLoader::loadFunctions()
{
  setState("loading functions");

  QSqlQuery query = database.exec("SELECT id, name, return_type, parameters, specifiers FROM functions");

  int ID = 0;
  int NAME = 1;
  int RETURN_TYPE = 2;
  int PARAMETERS = 3;
  int SPECIFIERS = 4;

  while (query.next())
  {
    auto fun = std::make_shared<Function>("");
    fun->function_id = query.value(ID).toInt();
    fun->name = query.value(NAME).toString();
    fun->returnType = query.value(RETURN_TYPE).toString();
    fun->parameters = query.value(PARAMETERS).toString().split(';', QString::SkipEmptyParts);
    
    QStringList specifiers = query.value(SPECIFIERS).toString().split(',');
    fun->isConst = specifiers.contains("const");
    fun->isStatic = specifiers.contains("static");
    fun->isDeleted = specifiers.contains("delete");
    fun->isExplicit = specifiers.contains("explicit");

    project->functions[fun->function_id] = fun;
  }
}

void ProjectLoader::loadClasses()
{
  setState("loading classes");

  QSqlQuery query = database.exec("SELECT id, name FROM classes");

  int ID = 0;
  int NAME = 1;

  while (query.next())
  {
    auto c = std::make_shared<Class>("");
    c->class_id = query.value(ID).toInt();
    c->name = query.value(NAME).toString();

    project->classes[c->class_id] = c;
  }
}

void ProjectLoader::loadEnums()
{
  setState("loading enums");

  QSqlQuery query = database.exec("SELECT id, name FROM enums");

  int ID = 0;
  int NAME = 1;

  while (query.next())
  {
    auto enm = std::make_shared<Enum>("");
    enm->enum_id = query.value(ID).toInt();
    enm->name = query.value(NAME).toString();

    project->enums[enm->enum_id] = enm;
  }
}

void ProjectLoader::loadEnumerators()
{
  setState("loading enumerators");

  //for (const std::pair<int, EnumRef>& entries : project->enums)
  //{
  //  EnumRef enm = entries.second;

  //  QSqlQuery query = database.exec("SELECT id, name FROM enumerators WHERE enum_id = " + QString::number(entries.first));

  //  int ID = 0;
  //  int NAME = 1;

  //  while (query.next())
  //  {
  //    auto enumerator = EnumeratorRef::create(query.value(NAME).toString());
  //    enumerator->enumerator_id = query.value(ID).toInt();
  //    enm->enumerators.append(enumerator);
  //    project->enumerators[enumerator->enumerator_id] = enumerator;
  //  }
  //}

  QSqlQuery query = database.exec("SELECT id, name FROM enumerators");

  int ID = 0;
  int NAME = 1;

  while (query.next())
  {
    auto enm = std::make_shared<Enumerator>("");
    enm->enumerator_id = query.value(ID).toInt();
    enm->name = query.value(NAME).toString();

    project->enumerators[enm->enumerator_id] = enm;
  }
}

void ProjectLoader::loadStatements()
{
  setState("loading statements");

  QSqlQuery query = database.exec("SELECT id, content FROM statements");

  int ID = 0;
  int CONTENT = 1;

  while (query.next())
  {
    auto s = std::make_shared<Statement>("");
    s->statement_id = query.value(ID).toInt();
    s->name = query.value(CONTENT).toString();

    project->statements[s->statement_id] = s;
  }
}

void ProjectLoader::buildEntityTree()
{
  setState("building tree");

  std::list<NodeRef> queue;

  for (const auto& e : project->modules_map)
  {
    queue.push_back(e.second);
    project->modules.append(e.second);
  }

  while (!queue.empty())
  {
    NodeRef n = queue.front();
    queue.pop_front();

    QSqlQuery query = database.exec(QString("SELECT id FROM entities WHERE parent = %1 ORDER BY 'order' ASC").arg(n->entity_id));

    int ID = 0;

    while (query.next())
    {
      int child_id = query.value(ID).toInt();
      auto child = project->entities[child_id];
      n->appendChild(child);

      queue.push_back(child);
    }
 }
}
