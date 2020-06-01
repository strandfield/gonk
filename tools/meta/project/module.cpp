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

QString Module::module_snake_name() const
{
  return this->name.toLower().replace(".", "_");
}

QString Module::module_camel_case() const
{
  QString result = module_snake_name();
  
  if (result.isEmpty())
    return result;

  result[0] = result[0].toUpper();

  int offset = result.indexOf("_");

  while (offset != -1)
  {
    result.remove(offset, 1);

    if(offset < result.length())
      result[offset] = result[offset].toUpper();

    offset = result.indexOf("_");
  }

  return result;
}

QString Module::module_dir_name() const
{
  return module_snake_name().replace("_", "-");
}
