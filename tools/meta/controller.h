// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_CONTROLLER_H
#define METAGONK_CONTROLLER_H

#include <QObject>

#include <memory>

class QFileInfo;

class QSqlDatabase;

class Controller : public QObject
{
  Q_OBJECT
public:
  explicit Controller(QObject* parent = nullptr);
  ~Controller();

  bool createSqlDatabase(const QFileInfo& sql_file);
  QSqlDatabase& database() const;

  static Controller& Instance();

protected Q_SLOTS:

private:
  static Controller* m_singleton;
  std::unique_ptr<QSqlDatabase> m_database;
  QString m_database_path;
};

#endif // METAGONK_CONTROLLER_H
