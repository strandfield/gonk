// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "database.h"

#include "controller.h"

#include <QSqlError>

#include <QFile>

#include <QDebug>

#include <stdexcept>

class QueryParser
{
public:
  QString text;
  int index = 0;

  QString next()
  {
    int offset = text.indexOf(";\n", index);
    QString result = text.mid(index, offset - index);
    index = offset + 2;
    return result;
  }
};

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

bool Database::run(const QString& filepath)
{
  QByteArray db_content;

  {
    QFile file{ filepath };
    file.open(QIODevice::ReadOnly);
    db_content = file.readAll();
  }

  QueryParser parser;
  parser.text = QString::fromUtf8(db_content).replace("\r\n", "\n");

  while (parser.index != parser.text.length())
  {
    QString q = parser.next();

    qDebug() << "Executing " << q;

    try
    {
      exec(q);    
    }
    catch (std::runtime_error &)
    {
      return false;
    }
  }

  return true;
}
