// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "controller.h"

#include "database.h"

#include "project-controller.h"
#include "project-loader.h"
#include "project-merger.h"

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
  m_singleton = this;
}

Controller::~Controller()
{
  if (m_database)
    m_database->close();

  m_singleton = nullptr;
}

Controller& Controller::Instance()
{
  return *m_singleton;
}


bool Controller::createSqlDatabase(const QFileInfo& db_dir, const QString& savepath)
{
  if (QFile::exists(savepath))
    QFile::remove(savepath);

  m_database.reset(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE")));
  database().setDatabaseName(savepath);

  if (!database().open())
    return false;

  m_database_path = savepath;

  return Database::run(db_dir.absoluteFilePath() + "/types.sql")
    && Database::run(db_dir.absoluteFilePath() + "/statements.sql")
    && Database::run(db_dir.absoluteFilePath() + "/namespaces.sql")
    && Database::run(db_dir.absoluteFilePath() + "/modules.sql")
    && Database::run(db_dir.absoluteFilePath() + "/enums.sql")
    && Database::run(db_dir.absoluteFilePath() + "/enumerators.sql")
    && Database::run(db_dir.absoluteFilePath() + "/functions.sql")
    && Database::run(db_dir.absoluteFilePath() + "/files.sql")
    && Database::run(db_dir.absoluteFilePath() + "/classes.sql")
    && Database::run(db_dir.absoluteFilePath() + "/entities.sql");
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
  m_project_controller.reset(new ProjectController(database(), m_project));
}

ProjectRef Controller::project() const
{
  return m_project;
}

void Controller::importSymbols(ProjectRef other)
{
  ProjectMerger merger{ database(), m_project, other };
  merger.merge();
}

ProjectController& Controller::projectController()
{
  return *m_project_controller;
}

