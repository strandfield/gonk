// Copyright (C) 2018 Vincent Chambrin
// This file is part of the Yasl project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project/statement.h"

#include <QJsonArray>

Statement::Statement(const QString & val, Qt::CheckState c)
  : Node(val, c)
{

}
