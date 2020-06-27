// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project.h"

#include <cxx/function.h>

#include <QDebug>

#include <algorithm>
#include <stdexcept>

MGType::MGType(std::string n, std::string id)
  : name(std::move(n)), id(std::move(id))
{

}

static std::vector<std::string> split_str(std::string s)
{
  std::vector<std::string> result;
  size_t pos = 0;
  while ((pos = s.find("::")) != std::string::npos) {
    result.push_back(s.substr(0, pos));
    s.erase(0, pos + 1);
  }
  result.push_back(s);
  return result;
}



std::shared_ptr<cxx::Entity> MGModule::getSymbol(const std::string& name) const
{
  std::vector<std::string> names = split_str(name);

  auto elem = [&]() -> std::shared_ptr<cxx::Entity> {
    for (auto e : this->entities)
    {
      if (e->name == names.front())
        return e;
    }

    return nullptr;
  }();

  names.erase(names.begin());

  while (!names.empty())
  {
    elem = [&]() -> std::shared_ptr<cxx::Entity> {
      for (size_t i(0); i < elem->childCount(); ++i)
      {
        if (std::static_pointer_cast<cxx::Entity>(elem->childAt(i))->name == names.front())
          return std::static_pointer_cast<cxx::Entity>(elem->childAt(i));
      }

      return nullptr;
    }();

    names.erase(names.begin());
  }

  return elem;
}

MGModulePtr MGProject::getModule(const std::string& name) const
{
  auto it = std::find_if(modules.begin(), modules.end(), [&name](const MGModulePtr& m) {
    return m->name == name;
    });

  return it != modules.end() ? *it : nullptr;
}

MGModulePtr MGProject::getOrCreateModule(const std::string& name)
{
  MGModulePtr m = getModule(name);

  if (!m)
  {
    m = std::make_shared<MGModule>(name);
    this->modules.push_back(m);
  }

  return m;
}

bool MGProject::hasType(const std::string& name) const
{
  return std::any_of(types.begin(), types.end(), [&name](const MGTypePtr& t) -> bool {
    return t->name == name;
    });
}

MGTypePtr MGProject::getTypeById(const std::string& id) const
{
  auto it = std::find_if(types.begin(), types.end(), [&id](const MGTypePtr& t) {
    return t->id == id;
    });

  return it != types.end() ? *it : nullptr;
}

bool MGProject::inDB(std::shared_ptr<cxx::Entity> e) const
{
  auto it = this->database_ids.find(e.get());
  return it != this->database_ids.end() && it->second.global_id != -1;
}

bool MGProject::inDB(MGModulePtr m) const
{
  auto it = this->database_ids.find(m.get());
  return it != this->database_ids.end() && it->second.global_id != -1;
}

bool MGProject::getMetadata(std::shared_ptr<cxx::Entity> e, json::Object& out) const
{
  auto it = this->metadata.find(e.get());
  if (it != this->metadata.end())
    return out = it->second, true;
  else
    return false;
}

json::Object& MGProject::getMetadata(std::shared_ptr<cxx::Entity> e)
{
  return this->metadata[e.get()];
}

bool MGProject::getMetadata(MGModulePtr e, json::Object& out) const
{
  auto it = this->metadata.find(e.get());
  if (it != this->metadata.end())
    return out = it->second, true;
  else
    return false;
}

json::Object& MGProject::getMetadata(MGModulePtr e)
{
  return this->metadata[e.get()];
}

bool eq(const std::shared_ptr<cxx::Entity>& a, const std::shared_ptr<cxx::Entity>& b)
{
  if (a->node_kind() != b->node_kind())
    return false;

  if (a == b)
    return true;

  if (a->is<cxx::Function>())
  {
    if (a->name != b->name)
      return false;

    const cxx::Function& af = static_cast<const cxx::Function&>(*a);
    const cxx::Function& bf = static_cast<const cxx::Function&>(*b);

    if (af.parameters.size() != bf.parameters.size())
      return false;

    for (size_t i(0); i < af.parameters.size(); ++i)
    {
      if (af.parameters.at(i)->type.toString() != bf.parameters.at(i)->type.toString())
        return false;
    }

    if (af.return_type.toString() != bf.return_type.toString())
      return false;

    return true;

  }
  else
  {
    return a->name == b->name;
  }
}

std::string qualifiedName(const cxx::Entity& e)
{
  std::string result = e.name;

  auto p = e.weak_parent.lock();

  while (p != nullptr)
  {
    result = p->name + "::" + result;
    p = p->weak_parent.lock();
  }

  return result;
}

std::vector<std::shared_ptr<cxx::Entity>> children(const cxx::Entity& e)
{
  std::vector<std::shared_ptr<cxx::Entity>> ret;

  for (size_t i(0); i < e.childCount(); ++i)
  {
    auto child = e.childAt(i);

    if (child->isEntity())
      ret.push_back(std::static_pointer_cast<cxx::Entity>(child));
  }

  return ret;
}

std::string signature(const cxx::Function& f)
{
  if (f.isConstructor())
  {
    std::string result;
    if (f.isExplicit())
      result += "explicit ";
    result += f.name;
    result += "(";
    {
      QStringList params;

      for (auto p : f.parameters)
      {
        QString p_str = QString::fromStdString(p->type.toString());

        if (!p->name.empty())
          p_str += " " + QString::fromStdString(p->name);

        params.append(p_str);
      }

      result += params.join(", ").toStdString();
    }
    result += ")";

    if (f.specifiers & cxx::FunctionSpecifier::Delete)
      result += " = delete";

    result += ";";

    return result;
  }
  else if (f.isDestructor())
  {
    std::string result;

    result += f.name;
    result += "()";

    if (f.specifiers & cxx::FunctionSpecifier::Delete)
      result += " = delete";

    result += ";";
    return result;
  }

  std::string result;
  if (f.isExplicit())
    result += "explicit ";
  if (f.isStatic())
    result += "static ";

  result += f.return_type.toString();
  result += " " + f.name;
  result += "(";
  {
    QStringList params;

    for (auto p : f.parameters)
    {
      QString p_str = QString::fromStdString(p->type.toString());

      if (!p->name.empty())
        p_str += " " + QString::fromStdString(p->name);

      params.append(p_str);
    }

    result += params.join(", ").toStdString();
  }
  result += ")";

  if (f.isConst())
    result += " const";

  if (f.specifiers & cxx::FunctionSpecifier::Delete)
    result += " = delete";

  result += ";";

  return result;
}
