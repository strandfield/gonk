// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_SIMPLE_NODE_EDITOR_H
#define METAGONK_SIMPLE_NODE_EDITOR_H

#include "abstractnodeeditor.h"

#include "project/enum.h"

class QCheckBox;
class QLineEdit;

class SimpleNodeEditor : public AbstractNodeEditor
{
  Q_OBJECT
public:
  explicit SimpleNodeEditor(NodeRef node, QWidget *p = nullptr);
  ~SimpleNodeEditor() = default;

  void write() override;
  void read(NodeRef node);

private:
  QLineEdit *mCondition;
  QLineEdit *mName;
};

#endif // METAGONK_SIMPLE_NODE_EDITOR_H
