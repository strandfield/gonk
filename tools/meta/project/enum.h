// Copyright (C) 2018 Vincent Chambrin
// This file is part of the Yasl project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef YASL_META_ENUM_H
#define YASL_META_ENUM_H

#include "project/node.h"

class Enumerator : public Node
{
public:
  int enumerator_id = -1;
  QString rename;

public:
  Enumerator(const QString & n, Qt::CheckState c = Qt::Checked);
  ~Enumerator() = default;

  QString typeName() const override { return "enumerator"; }
  static const NodeType staticTypeCode = NodeType::Enumerator;
  NodeType typeCode() const override { return staticTypeCode; }
};

typedef std::shared_ptr<Enumerator> EnumeratorRef;


class Enum : public Node
{
public:
  int enum_id = -1;
  QList<EnumeratorRef> enumerators;
  bool isCppEnumClass;
  bool isEnumClass;

public:
  Enum(const QString & n, Qt::CheckState c = Qt::Checked);
  ~Enum() = default;

  QString display() const override;

  QString typeName() const override { return "enum"; }
  static const NodeType staticTypeCode = NodeType::Enum;
  NodeType typeCode() const override { return staticTypeCode; }

  void appendChild(NodeRef n) override;
  size_t childCount() const override;
  std::shared_ptr<Node> childAt(size_t index) const override;
  void removeChild(size_t index) override;
  QList<std::shared_ptr<Node>> children() const override;

  void merge(const Enum & other);
};
typedef std::shared_ptr<Enum> EnumRef;

#endif // YASL_META_ENUM_H
