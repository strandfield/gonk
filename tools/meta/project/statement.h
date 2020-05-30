// Copyright (C) 2018 Vincent Chambrin
// This file is part of the Yasl project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef YASL_META_STATEMENT_H
#define YASL_META_STATEMENT_H

#include "project/node.h"

class Statement : public Node
{
public:
  Statement(const QString & val, Qt::CheckState c = Qt::Checked);
  ~Statement() = default;

  QString typeName() const override { return "statement"; }
  static const NodeType staticTypeCode = NodeType::Statement;
  NodeType typeCode() const override { return staticTypeCode; }

  int statement_id = -1;
};
typedef std::shared_ptr<Statement> StatementRef;

#endif // YASL_META_STATEMENT_H
