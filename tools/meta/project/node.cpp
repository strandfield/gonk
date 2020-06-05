// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project/node.h"

#include "project/file.h"
#include "project/function.h"
#include "project/enum.h"
#include "project/class.h"
#include "project/module.h"

Node::Node(const QString & n, Qt::CheckState c)
  : checkState(c)
  , name(n)
{

}

void Node::appendChild(std::shared_ptr<Node> n)
{
  throw std::runtime_error{ "Node does not support child insertion" };
}

size_t Node::childCount() const
{
  return 0;
}

std::shared_ptr<Node> Node::childAt(size_t index) const
{
  throw std::runtime_error{ "Node does not support childAt()" };
}

void Node::removeChild(size_t index)
{
  throw std::runtime_error{ "Node does not support removeChild()" };
}

QList<std::shared_ptr<Node>> Node::children() const
{
  QList<std::shared_ptr<Node>> ret;

  for (size_t i(0); i < childCount(); ++i)
    ret.append(childAt(i));

  return ret;
}

QString Node::nameQualification(const QStack<std::shared_ptr<Node>> & nodes)
{
  if (nodes.isEmpty())
    return "";

  QStringList names;
  for (const auto & node : nodes)
  {
    if (node->is<File>() || node->is<Module>())
      continue;

    names << node->name;
  }

  if (names.empty())
    return "";
  return names.join("::") + "::";
}

QString Node::nameQualification(const std::vector<std::shared_ptr<Node>>& nodes)
{
  if (nodes.empty())
    return "";

  QStringList names;
  for (const auto& node : nodes)
  {
    if (node->is<File>() || node->is<Module>())
      continue;

    names << node->name;
  }

  if (names.empty())
    return "";
  return names.join("::") + "::";
}

int Node::compare(const Node & a, const Node & b)
{
  if (a.typeCode() < b.typeCode())
    return -1;
  else if (a.typeCode() > b.typeCode())
    return 1;

  return a.compareTo(b);
}

int Node::compareTo(const Node & other) const
{
  return this->name.compare(other.name);
}

bool eq(const NodeRef & lhs, const NodeRef & rhs)
{
  return Node::compare(*lhs, *rhs) == 0;
}

void sort(QList<NodeRef> & list)
{
  qSort(list.begin(), list.end(), [](const NodeRef & a, const NodeRef & b) -> bool {
    return Node::compare(*a, *b) < 0;
  });
}
