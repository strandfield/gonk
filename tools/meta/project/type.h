// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_TYPE_H
#define METAGONK_TYPE_H

#include <QJsonObject>
#include <QString>

class Type
{
public:
  QString name;
  QString id;
  QString rename;
  QString header;
  QString tag;
  QString links;
  QString metatype;
  std::string condition;

public:
  Type() = default;
  Type(const QString & n, const QString & id);
  ~Type() = default;

  enum Category {
    FundamentalType,
    ClassType,
    EnumType,
  };

  QJsonObject toJson() const;
  static Type fromJson(const QJsonObject & obj);
};

#endif // METAGONK_TYPE_H
