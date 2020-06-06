// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_ABSTRACT_NODE_EDITOR_H
#define METAGONK_ABSTRACT_NODE_EDITOR_H

#include <QWidget>

#include "project/node.h"

class AbstractNodeEditor : public QWidget
{
  Q_OBJECT
public:
  AbstractNodeEditor(NodeRef n, QWidget *p = nullptr);
  ~AbstractNodeEditor() = default;

  virtual void write() = 0;

  inline NodeRef getNode() const { return mNode; }

protected:
  inline void setNode(const NodeRef & n) { mNode = n; }

private:
  NodeRef mNode;
};

#endif // METAGONK_ABSTRACT_NODE_EDITOR_H
