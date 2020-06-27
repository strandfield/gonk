// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_PROJECT_H
#define METAGONK_PROJECT_H

#include <cxx/entity.h>

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

class MGModule : public std::enable_shared_from_this<MGModule>
{
public:
  std::string name;
  std::vector<std::shared_ptr<cxx::Entity>> entities;

public:
  MGModule() = default;
  MGModule(std::string n): name(std::move(n)) { }

  std::shared_ptr<cxx::Entity> getSymbol(const std::string& name) const;

};

typedef std::shared_ptr<MGModule> MGModulePtr;

struct MGDatabaseId
{
  int global_id = -1;
  int id = -1;
};

class MGProject
{
public:
  std::vector<MGTypePtr> types;
  std::vector<MGModulePtr> modules;
  std::unordered_map<void*, MGDatabaseId> database_ids;
  std::map<std::shared_ptr<cxx::Entity>, MGTypePtr> entity_type_map;
  std::unordered_map<void*, json::Object> metadata;

  MGDatabaseId& dbid(MGModulePtr m)
  {
    return database_ids[m.get()];
  }

  MGDatabaseId& dbid(std::shared_ptr<cxx::Entity> e)
  {
    return database_ids[e.get()];
  }

  MGModulePtr getModule(const std::string& name) const;
  MGModulePtr getOrCreateModule(const std::string& name);

  bool hasType(const std::string& name) const;

  MGTypePtr getTypeById(const std::string& id) const;

  bool inDB(std::shared_ptr<cxx::Entity> e) const;
  bool inDB(MGModulePtr m) const;

  bool getMetadata(std::shared_ptr<cxx::Entity> e, json::Object& out) const;
  json::Object& getMetadata(std::shared_ptr<cxx::Entity> e);
  bool getMetadata(MGModulePtr e, json::Object& out) const;
  json::Object& getMetadata(MGModulePtr e);
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
