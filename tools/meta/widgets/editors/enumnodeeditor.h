// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_ENUM_NODE_EDITOR_H
#define METAGONK_ENUM_NODE_EDITOR_H

#include "abstractnodeeditor.h"

#include "project/enum.h"

class QCheckBox;
class QLineEdit;

class EnumNodeEditor : public AbstractNodeEditor
{
  Q_OBJECT
public:
  EnumNodeEditor(const EnumRef & enm, QWidget *p = nullptr);
  ~EnumNodeEditor() = default;

  void write() override;
  void read(EnumRef enm);

  inline EnumRef getEnum() const { return std::static_pointer_cast<Enum>(getNode()); }

private:
  QLineEdit *mCondition;
  QLineEdit *mName;
  QCheckBox *mToEnumClass;
  QCheckBox *mFromEnumClass;
};

#endif // METAGONK_ENUM_NODE_EDITOR_H
