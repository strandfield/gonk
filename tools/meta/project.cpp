// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project.h"

#include <cxx/class.h>
#include <cxx/enum.h>
#include <cxx/function.h>
#include <cxx/namespace.h>

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

bool MGProject::isOperator(const cxx::Function& f)
{
  return f.name.find("operator", 0) == 0;
}

static int comp(const cxx::Namespace& lhs, const cxx::Namespace& rhs)
{
  return std::strcmp(lhs.name.c_str(), rhs.name.c_str());
}

static int comp(const cxx::Enum& lhs, const cxx::Enum& rhs)
{
  return std::strcmp(lhs.name.c_str(), rhs.name.c_str());
}

static int comp(const cxx::EnumValue& lhs, const cxx::EnumValue& rhs)
{
  return std::strcmp(lhs.name.c_str(), rhs.name.c_str());
}

static int comp(const cxx::Class& lhs, const cxx::Class& rhs)
{
  return std::strcmp(lhs.name.c_str(), rhs.name.c_str());
}

static int bool_comp(bool lhs, bool rhs)
{
  return (lhs && !rhs) ? 1 : (!lhs && rhs ? -1 : 0);
}

static int str_comp(const std::string& lhs, const std::string& rhs)
{
  return std::strcmp(lhs.c_str(), rhs.c_str());
}

static int comp(const cxx::Function& lhs, const cxx::Function& rhs)
{
  int c = bool_comp(!lhs.isConstructor(), !rhs.isConstructor());

  if (c != 0)
    return c;

  c = bool_comp(!lhs.isDestructor(), !rhs.isDestructor());

  if (c != 0)
    return c;

  c = bool_comp(MGProject::isOperator(lhs), MGProject::isOperator(rhs));

  if (c != 0)
    return c;

  c = std::strcmp(lhs.name.c_str(), rhs.name.c_str());

  if (c != 0)
    return c;

  c = static_cast<int>(lhs.parameters.size()) - static_cast<int>(rhs.parameters.size());

  if (c != 0)
    return c;

  for (size_t i(0); i < lhs.parameters.size(); ++i)
  {
    c = str_comp(lhs.parameters.at(i)->type.toString(), rhs.parameters.at(i)->type.toString());

    if (c != 0)
      return c;
  }

  return str_comp(lhs.return_type.toString(), lhs.return_type.toString());
}

int MGProject::comp(const cxx::Entity& lhs, const cxx::Entity& rhs)
{
  static const std::vector<cxx::NodeKind> node_kinds = {
    cxx::NodeKind::Namespace, cxx::NodeKind::Enum, cxx::NodeKind::Class, cxx::NodeKind::Function,
  };

  auto compare_node_kind = [&](const cxx::Entity& lhs, const cxx::Entity& rhs) -> int {
    auto lhs_it = std::find(node_kinds.begin(), node_kinds.end(), lhs.node_kind());
    auto rhs_it = std::find(node_kinds.begin(), node_kinds.end(), rhs.node_kind());

    size_t lhs_d = std::distance(node_kinds.begin(), lhs_it);
    size_t rhs_d = std::distance(node_kinds.begin(), rhs_it);

    if (lhs_d < rhs_d)
      return -1;
    else if (lhs_d > rhs_d)
      return 1;
    return 0;
  };

  int c = compare_node_kind(lhs, rhs);

  if (c != 0)
    return c;

  assert(lhs.node_kind() == rhs.node_kind());

  switch (lhs.node_kind())
  {
  case cxx::NodeKind::Namespace:
    return ::comp(static_cast<const cxx::Namespace&>(lhs), static_cast<const cxx::Namespace&>(rhs));
  case cxx::NodeKind::Class:
    return ::comp(static_cast<const cxx::Class&>(lhs), static_cast<const cxx::Class&>(rhs));
  case cxx::NodeKind::Enum:
    return ::comp(static_cast<const cxx::Enum&>(lhs), static_cast<const cxx::Enum&>(rhs));
  case cxx::NodeKind::EnumValue:
    return ::comp(static_cast<const cxx::EnumValue&>(lhs), static_cast<const cxx::EnumValue&>(rhs));
  case cxx::NodeKind::Function:
    return ::comp(static_cast<const cxx::Function&>(lhs), static_cast<const cxx::Function&>(rhs));
  default:
    return 0;
  }
}

void MGProject::sort(std::vector<std::shared_ptr<cxx::Entity>>& list)
{
  std::sort(list.begin(), list.end(), [](const std::shared_ptr<cxx::Entity>& lhs, const std::shared_ptr<cxx::Entity>& rhs) -> bool {
    return comp(*lhs, *rhs) < 0;
    });
}

void MGProject::sort()
{
  for (auto m : this->modules)
  {
    sort(m->entities);
  }
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
