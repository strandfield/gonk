// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_PROJECT_H
#define METAGONK_PROJECT_H

#include <cxx/entity.h>
#include <cxx/program.h>

#include <json-toolkit/json.h>

#include <map>
#include <memory>
#include <unordered_map>

namespace cxx
{
class Class;
class Enum;
class Function;
class Namespace;
} // namespace cxx

class MGType
{
public:
  enum Category {
    FundamentalType,
    ClassType,
    EnumType,
  };

public:
  int database_id = -1;
  std::string name;
  std::string id;
  Category category = FundamentalType;

public:
  MGType() = default;
  MGType(std::string n, std::string id);
  ~MGType() = default;

  bool isClass() const { return category == ClassType; }
  bool isEnum() const { return category == EnumType; }
};

typedef std::shared_ptr<MGType> MGTypePtr;

struct MGDatabaseId
{
  int global_id = -1;
  int id = -1;
};

class MGProject
{
public:
  std::vector<MGTypePtr> types;
  std::unordered_map<void*, MGDatabaseId> database_ids;
  std::map<std::shared_ptr<cxx::Entity>, MGTypePtr> entity_type_map;
  std::unordered_map<void*, json::Object> metadata;
  std::map<std::string, std::shared_ptr<cxx::File>> files;
  std::string module_name;
  std::string module_folder;
  std::string module_namespace;
  std::shared_ptr<cxx::Program> program;

  MGProject()
  {
    program = std::make_shared<cxx::Program>();
  }

  MGDatabaseId& dbid(std::shared_ptr<cxx::Entity> e)
  {
    return database_ids[e.get()];
  }

  bool hasType(const std::string& name) const;

  MGTypePtr getTypeById(const std::string& id) const;

  bool inDB(std::shared_ptr<cxx::Entity> e) const;

  bool getMetadata(std::shared_ptr<cxx::Entity> e, json::Object& out) const;
  json::Object& getMetadata(std::shared_ptr<cxx::Entity> e);

  static bool isOperator(const cxx::Function& f);
  static int comp(const cxx::Entity& lhs, const cxx::Entity& rhs);
  static void sort(std::vector<std::shared_ptr<cxx::Entity>>& list);
  void sort();

  std::shared_ptr<cxx::Entity> getSymbol(const std::string& name) const;
  std::vector<std::shared_ptr<cxx::Entity>> getSymbolsByLocation(const std::string& filename) const;
};

typedef std::shared_ptr<MGProject> MGProjectPtr;

class RAIICxxElemGuard
{
public:
  std::shared_ptr<cxx::Entity>& target;
  std::shared_ptr<cxx::Entity> backup_value;

  RAIICxxElemGuard(std::shared_ptr<cxx::Entity>& e)
    : target(e),
    backup_value(e)
  {

  }

  ~RAIICxxElemGuard()
  {
    target = backup_value;
  }
};

bool eq(const std::shared_ptr<cxx::Entity>& a, const std::shared_ptr<cxx::Entity>& b);
std::string qualifiedName(const cxx::Entity& e);
std::vector<std::shared_ptr<cxx::Entity>> children(const cxx::Entity& e);
std::string signature(const cxx::Function& f);

#endif // METAGONK_PROJECT_H
