// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "controller.h"

#include "project-loader.h"

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

#include <QFile>
#include <QFileInfo>

#include <QDebug>

Controller* Controller::m_singleton = nullptr;

Controller::Controller(QObject* parent)
  : QObject(parent)
{

}

Controller::~Controller()
{
  if (m_database)
    m_database->close();
}

Controller& Controller::Instance()
{
  return *m_singleton;
}

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

bool Controller::createSqlDatabase(const QFileInfo& sql_file, const QString& savepath)
{
  if (QFile::exists(savepath))
    QFile::remove(savepath);

  m_database.reset(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE")));
  database().setDatabaseName(savepath);

  if (!database().open())
    return false;

  m_database_path = savepath;

  QByteArray db_content;

  {
    QFile file{ sql_file.absoluteFilePath() };
    file.open(QIODevice::ReadOnly);
    db_content = file.readAll();
  }

  QueryParser parser;
  parser.text = QString::fromUtf8(db_content).replace("\r\n", "\n");

  while (parser.index != parser.text.length())
  {
    QString q = parser.next();

    qDebug() << "Executing " << q;

    QSqlQuery query = database().exec(q);

    if (database().lastError().isValid())
    {
      return false;
    }
  }

  return true;
}

bool Controller::loadDatabase(const QFileInfo& db_file)
{
  m_database.reset(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE")));
  database().setDatabaseName(db_file.absoluteFilePath());

  return database().open();
}

QSqlDatabase& Controller::database() const
{
  return *m_database;
}

void Controller::loadProject()
{
  ProjectLoader loader{ database() };
  loader.load();
  m_project = loader.project;
}

ProjectRef Controller::project() const
{
  return m_project;
}
