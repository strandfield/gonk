// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_STATEMENT_H
#define METAGONK_STATEMENT_H

#include "project/node.h"

class Statement : public Node
{
public:
  Statement(const QString & val, Qt::CheckState c = Qt::Checked);
  ~Statement() = default;

  QString typeName() const override { return "statement"; }
  static const NodeType staticTypeCode = NodeType::Statement;
  NodeType typeCode() const override { return staticTypeCode; }

  void accept(NodeVisitor& visitor) override;

  int statement_id = -1;
};
typedef std::shared_ptr<Statement> StatementRef;

#endif // METAGONK_STATEMENT_H
