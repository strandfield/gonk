// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_ABSTRACT_NODE_EDITOR_H
#define METAGONK_ABSTRACT_NODE_EDITOR_H

#include <QWidget>

#include "project.h"

class AbstractNodeEditor : public QWidget
{
  Q_OBJECT
public:
  AbstractNodeEditor(std::shared_ptr<cxx::Entity> n, QWidget *p = nullptr);
  ~AbstractNodeEditor() = default;

  virtual void write() = 0;

  inline std::shared_ptr<cxx::Entity> getNode() const { return mNode; }

protected:
  inline void setNode(const std::shared_ptr<cxx::Entity>& n) { mNode = n; }

private:
  std::shared_ptr<cxx::Entity> mNode;
};

#endif // METAGONK_ABSTRACT_NODE_EDITOR_H
