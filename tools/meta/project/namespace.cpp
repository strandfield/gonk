// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project/namespace.h"

#include <QJsonArray>

Namespace::Namespace(const QString & n, Qt::CheckState c)
  : Node(n, c)
{

}

void Namespace::appendChild(NodeRef n)
{
  elements.append(n);
}

size_t Namespace::childCount() const
{
  return elements.size();
}

std::shared_ptr<Node> Namespace::childAt(size_t index) const
{
  return elements.at(index);
}

void Namespace::removeChild(size_t index)
{
  elements.removeAt(index);
}

QList<std::shared_ptr<Node>> Namespace::children() const
{
  return elements;
}

std::shared_ptr<Namespace> Namespace::getNamespace(const QString & name)
{
  for (const auto & e : elements)
  {
    if (e->is<Namespace>() && e->name == name)
      return std::static_pointer_cast<Namespace>(e);
  }

  auto ret = std::make_shared<Namespace>(name);
  elements.append(ret);
  return ret;
}

