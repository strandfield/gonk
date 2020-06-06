// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "database.h"

#include "controller.h"

#include <QSqlError>

#include <QDebug>

QSqlQuery Database::exec(const QString& query)
{
  QSqlQuery result = Controller::Instance().database().exec(query);

  if (result.lastError().isValid())
  {
    qDebug() << result.lastError().text();
    throw std::runtime_error{ std::string("Database query error: ") + result.lastError().text().toStdString() };
  }

  return result;
}
