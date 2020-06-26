// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_CLASS_NODE_EDITOR_H
#define METAGONK_CLASS_NODE_EDITOR_H

#include "abstractnodeeditor.h"

#include <cxx/class.h>

class QCheckBox;
class QLineEdit;

class ClassNodeEditor : public AbstractNodeEditor
{
  Q_OBJECT
public:
  ClassNodeEditor(const std::shared_ptr<cxx::Class>& cla, QWidget *p = nullptr);
  ~ClassNodeEditor() = default;

  void write() override;
  void read(std::shared_ptr<cxx::Class> cla);

  inline std::shared_ptr<cxx::Class> getClass() const { return std::static_pointer_cast<cxx::Class>(getNode()); }

private:
  QLineEdit *mName;
  QCheckBox *mFinal;
  QLineEdit *mBase;
};

#endif // METAGONK_CLASS_NODE_EDITOR_H
