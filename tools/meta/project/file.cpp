// Copyright (C) 2018 Vincent Chambrin
// This file is part of the Yasl project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project/file.h"

#include "project/node-visitor.h"

#include <QJsonArray>
#include <QJsonObject>

File::File(const QString & n, Qt::CheckState c)
  : Namespace(n, c)
{

}

QString File::display() const
{
  QString ret = this->name;

  if (!hincludes.isEmpty())
  {
    ret += " [h-includes:" + hincludes.join(",") + "]";
  }

  if (!cppincludes.isEmpty())
  {
    ret += " [cpp-includes:" + cppincludes.join(",") + "]";
  }

  return ret;
}

void File::accept(NodeVisitor& visitor)
{
  visitor.visit(*this);
}
