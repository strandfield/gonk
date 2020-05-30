// Copyright (C) 2018 Vincent Chambrin
// This file is part of the Yasl project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project/module.h"

#include "project/node-visitor.h"

#include <QJsonArray>
#include <QJsonObject>

Module::Module(const QString & n, Qt::CheckState c)
  : Node(n, c)
{

}

void Module::accept(NodeVisitor& visitor)
{
  visitor.visit(*this);
}

void Module::appendChild(NodeRef child)
{
  elements.push_back(child);
  child->parent = shared_from_this();
}

size_t Module::childCount() const
{
  return elements.size();
}

std::shared_ptr<Node> Module::childAt(size_t index) const
{
  return elements.at(index);
}

void Module::removeChild(size_t index)
{
  elements.removeAt(index);
}

QList<std::shared_ptr<Node>> Module::children() const
{
  return elements;
}

//yaml::Value Module::toYaml() const
//{
//  yaml::Array elems;
//  for (const auto & e : elements)
//  {
//    elems.push(e->toYaml());
//  }
//
//  yaml::Object ret;
//  ret[name] = elems;
//  return ret;
//}
//
//QSharedPointer<Module> Module::fromYaml(const yaml::Object & inputobj)
//{
//  auto it = inputobj.underlyingMap().begin();
//
//  ModuleRef ret = ModuleRef::create(it.key(), Qt::Checked);
//
//  yaml::Array elements = it.value().toArray();
//  ret->elements.reserve(elements.size());
//  for (const auto & item : elements)
//    ret->elements.push_back(Node::fromYaml(item.toObject()));
//
//  return ret;
//}
