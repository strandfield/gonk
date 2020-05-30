// Copyright (C) 2018 Vincent Chambrin
// This file is part of the Yasl project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project/enum.h"

#include "project/node-visitor.h"

#include <QDebug>
#include <QJsonArray>

Enumerator::Enumerator(const QString & n, Qt::CheckState c)
  : Node(n, c)
{

}

void Enumerator::accept(NodeVisitor& visitor)
{
  visitor.visit(*this);
}


Enum::Enum(const QString & n, Qt::CheckState c)
  : Node(n, c)
  , isEnumClass(false)
  , isCppEnumClass(false)
{

}

QString Enum::display() const
{
  QString ret = name;
  if (isCppEnumClass)
    ret += " [from-enum-class]";
  if (isEnumClass)
    ret += " [to-enum-class]";
  return ret;
}

void Enum::accept(NodeVisitor& visitor)
{
  visitor.visit(*this);
}

void Enum::appendChild(NodeRef n)
{
  if (!n->is<Enumerator>())
    throw std::runtime_error{ "Enum::appendChild() : child must be an Enumerator" };

  this->enumerators.push_back(std::static_pointer_cast<Enumerator>(n));
  n->parent = shared_from_this();
}

size_t Enum::childCount() const
{
  return enumerators.size();
}

std::shared_ptr<Node> Enum::childAt(size_t index) const
{
  return enumerators.at(index);
}

void Enum::removeChild(size_t index)
{
  enumerators.removeAt(index);
}

QList<std::shared_ptr<Node>> Enum::children() const
{
  QList<NodeRef> ret;

  for (auto e : enumerators)
    ret.append(e);

  return ret;
}

static bool contains(const QList<EnumeratorRef> & enumerators, const EnumeratorRef & e)
{
  for (const auto & item : enumerators)
  {
    if (item->name == e->name)
      return true;
  }

  return false;
}

void Enum::merge(const Enum & other)
{
  for (const auto & e : other.enumerators)
  {
    if (!contains(this->enumerators, e))
      this->enumerators.append(e);
  }
}
