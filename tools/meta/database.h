// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_DATABASE_H
#define METAGONK_DATABASE_H

#include <QSqlQuery>

class Database
{
public:
  static QSqlQuery exec(const QString& query);
};

#endif // METAGONK_DATABASE_H
