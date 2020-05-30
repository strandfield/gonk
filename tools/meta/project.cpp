// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project.h"

#include "project/class.h"
#include "project/enum.h"
#include "project/file.h"
#include "project/namespace.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStack>

#include <QDebug>

template<typename T>
void remove_unchekced_recursively(QList<T> & nodes)
{
  for (int i(0); i < nodes.size(); ++i)
  {
    if (nodes.at(i)->checkState == Qt::Unchecked)
    {
      nodes.removeAt(i);
      --i;
    }
    else
    {
      remove_unchekced_recursively(nodes.at(i));
    }
  }
}

static void remove_unchekced_recursively(const NodeRef & node)
{
  if (node->is<Namespace>())
  {
    Namespace & ns = node->as<Namespace>();
    remove_unchekced_recursively(ns.elements);
  }
  else if (node->is<Module>())
  {
    Module &m = node->as<Module>();
    remove_unchekced_recursively(m.elements);
  }
  else if (node->is<Class>())
  {
    Class &cla = node->as<Class>();
    remove_unchekced_recursively(cla.elements);
  }
  else if (node->is<Enum>())
  {
    Enum &enm = node->as<Enum>();
    remove_unchekced_recursively(enm.enumerators);
  }
}

void Project::removeUncheckedSymbols()
{
  remove_unchekced_recursively(this->modules);
}

template<typename T>
void fetch_types_recursively(Project & pro, QStack<NodeRef> & stack, const QList<T> & nodes)
{
  for (const auto & n : nodes)
    fetch_types_recursively(pro, stack, n);
}

static void fetch_types_recursively(Project & pro, QStack<NodeRef> & stack, const NodeRef & node)
{
  stack.push(node);

  if (node->is<Namespace>())
  {
    Namespace & ns = node->as<Namespace>();
    fetch_types_recursively(pro, stack, ns.elements);
  }
  else if (node->is<Module>())
  {
    Module &m = node->as<Module>();
    fetch_types_recursively(pro, stack, m.elements);
  }
  else if (node->is<Class>())
  {
    if (pro.hasClassType(node->name))
      return;

    stack.pop();
    const QString name = Node::nameQualification(stack) + node->name;
    pro.types.classes.append(Type{ name, QString{ name }.remove("::") });
    stack.push(node);

    fetch_types_recursively(pro, stack, node->as<Class>().elements);
  }
  else if (node->is<Enum>())
  {
    if (pro.hasEnumType(node->name))
      return;

    stack.pop();
    const QString name = Node::nameQualification(stack) + node->name;
    pro.types.enums.append(Type{ name, QString{ name }.remove("::") });
    stack.push(node);
  }

  stack.pop();
}

void Project::fetchTypes()
{
  QStack<NodeRef> stack;
  fetch_types_recursively(*this, stack, this->modules);
}

bool Project::hasEnumType(const QString & name) const
{
  for (const auto & t : types.enums)
  {
    if (t.name == name)
      return true;
  }

  return false;
}

bool Project::hasClassType(const QString & name) const
{
  for (const auto & t : types.classes)
  {
    if (t.name == name)
      return true;
  }

  return false;
}

Type & Project::getType(const QString & name)
{
  for (auto & t : types.fundamentals)
  {
    if (t.name == name)
      return t;
  }

  for (auto & t : types.classes)
  {
    if (t.name == name)
      return t;
  }

  for (auto & t : types.enums)
  {
    if (t.name == name)
      return t;
  }

  throw std::runtime_error{ "Project::getType() : Unsupported type" };
}

void Project::sort(QList<Type> & types)
{
  struct LessThan
  {
    bool operator()(const Type & a, const Type & b)
    {
      return QString::compare(a.name, b.name, Qt::CaseInsensitive) < 0;
    }
  };

  qSort(types.begin(), types.end(), LessThan{});
}

int Project::fileCount() const
{
  int n = 0;

  for (const auto & m : modules)
  {
    for (const auto & e : m->elements)
    {
      if (e->is<File>())
        n++;
    }
  }

  return n;
}
