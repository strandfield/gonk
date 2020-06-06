// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_FILE_H
#define METAGONK_FILE_H

#include "project/namespace.h"

class File : public Namespace
{
public:
  int file_id = -1;
  QStringList hincludes;
  QStringList cppincludes;

public:
  File(const QString & n, Qt::CheckState c = Qt::Checked);
  ~File() = default;

  QString typeName() const override { return "file"; }
  static const NodeType staticTypeCode = NodeType::File;
  NodeType typeCode() const override { return staticTypeCode; }

  QString display() const override;

  void accept(NodeVisitor& visitor) override;

};
typedef std::shared_ptr<File> FileRef;

#endif // METAGONK_FILE_H
