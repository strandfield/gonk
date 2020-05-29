// Copyright (C) 2018 Vincent Chambrin
// This file is part of the Yasl project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_CLASS_NODE_EDITOR_H
#define METAGONK_CLASS_NODE_EDITOR_H

#include "abstractnodeeditor.h"

#include "project/class.h"

class QCheckBox;
class QLineEdit;

class ClassNodeEditor : public AbstractNodeEditor
{
  Q_OBJECT
public:
  ClassNodeEditor(const ClassRef & cla, QWidget *p = nullptr);
  ~ClassNodeEditor() = default;

  void write() override;
  void read(ClassRef cla);

  inline ClassRef getClass() const { return qSharedPointerCast<Class>(getNode()); }

private:
  QLineEdit *mCondition;
  QLineEdit *mName;
  QCheckBox *mFinal;
  QLineEdit *mBase;
};

#endif // METAGONK_CLASS_NODE_EDITOR_H
