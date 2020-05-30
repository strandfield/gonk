// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project/class.h"

#include "project/node-visitor.h"

#include <QJsonArray>

Class::Class(const QString & n, Qt::CheckState c)
  : Node(n, c)
  , isFinal(false)
{

}

QString Class::display() const
{
  QString ret = name;
  if (isFinal)
    ret += " final";
  if (!base.isEmpty())
    ret += " : " + base;

  if (!condition.empty())
    ret += " [" + QString::fromStdString(condition) + "]";

  return ret;
}

void Class::accept(NodeVisitor& visitor)
{
  visitor.visit(*this);
}

void Class::appendChild(NodeRef n)
{
  elements.append(n);
  n->parent = shared_from_this();
}

size_t Class::childCount() const
{
  return elements.size();
}

std::shared_ptr<Node> Class::childAt(size_t index) const
{
  return elements.at(index);
}

void Class::removeChild(size_t index)
{
  elements.removeAt(index);
}

QList<std::shared_ptr<Node>> Class::children() const
{
  return elements;
}
