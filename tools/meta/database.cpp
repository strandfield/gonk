// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "database.h"

#include "controller.h"

#include <cxx/class.h>
#include <cxx/function.h>

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
    qDebug() << query;
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

int Database::getFileId(const cxx::File& file)
{
  QSqlQuery q = exec(QString("SELECT id FROM files WHERE path = '%1'")
    .arg(QString::fromStdString(file.path())));

  if (q.next())
    return q.value(0).toInt();

  q = exec(QString("INSERT INTO files(path) VALUES('%1')")
    .arg(QString::fromStdString(file.path())));

  return q.lastInsertId().toInt();
}

QString Database::sqlEscape(QString str)
{
  return str.replace("'", "' || CHAR(39) || '");
}

QString Database::sqlEscape(const std::string str)
{
  return sqlEscape(QString::fromStdString(str));
}

QString Database::base(const cxx::Class& c)
{
  if (c.bases.empty())
    return QString();

  // TODO: should be the qualified name I believe
  return QString::fromStdString(c.bases.front().base->name);
}

QString Database::parameters(const cxx::Function& f)
{
  QStringList params;

  for (auto p : f.parameters)
  {
    QString p_str = QString::fromStdString(p->type.toString());

    if (!p->name.empty())
      p_str += "@" + QString::fromStdString(p->name);

    if(p->default_value != cxx::Expression())
      p_str += "#" + sqlEscape(QString::fromStdString(p->default_value.toString()));

    params.push_back(p_str);
  }

  return params.join(";");
}

QString Database::specifiers(const cxx::Function& f)
{
  QStringList specs;

  if (f.isConstructor())
    specs.push_back("ctor");
  else if (f.isDestructor())
    specs.push_back("dtor");
  
  if (f.isConst())
    specs.push_back("const");
  if (f.isConstexpr())
    specs.push_back("constexpr");
  if (f.isExplicit())
    specs.push_back("explicit");
  if (f.isStatic())
    specs.push_back("static");
  if (f.isInline())
    specs.push_back("inline");
  if (f.isVirtual())
    specs.push_back("virtual");
  if (f.isNoexcept())
    specs.push_back("noexcept");

  return specs.join(",");
}
