// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_DATABASE_H
#define METAGONK_DATABASE_H

#include <QSqlQuery>

namespace cxx
{
class Class;
class Function;
} // namespace cxx

class Database
{
public:
  static QSqlQuery exec(const QString& query);

  static bool run(const QString& filepath);

  static QString sqlEscape(QString str);
  static QString sqlEscape(const std::string str);

  static QString base(const cxx::Class& c);
  static QString parameters(const cxx::Function& f);
  static QString specifiers(const cxx::Function& f);
};

#endif // METAGONK_DATABASE_H
