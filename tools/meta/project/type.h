// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_TYPE_H
#define METAGONK_TYPE_H

#include <QString>

#include <memory>

class Type
{
public:
  int database_id = -1;
  QString name;
  QString id;
  QString header;
  QString condition;
  bool is_enum = false;
  bool is_class = false;

public:
  Type() = default;
  Type(const QString & n, const QString & id);
  ~Type() = default;

  enum Category {
    FundamentalType,
    ClassType,
    EnumType,
  };
};

typedef std::shared_ptr<Type> TypePtr;

#endif // METAGONK_TYPE_H
