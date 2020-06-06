// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project/statement.h"

#include "project/node-visitor.h"

#include <QJsonArray>

Statement::Statement(const QString & val, Qt::CheckState c)
  : Node(val, c)
{

}

void Statement::accept(NodeVisitor& visitor)
{
  visitor.visit(*this);
}
