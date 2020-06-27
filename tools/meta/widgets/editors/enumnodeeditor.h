// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_ENUM_NODE_EDITOR_H
#define METAGONK_ENUM_NODE_EDITOR_H

#include "abstractnodeeditor.h"

#include <cxx/enum.h>

class QCheckBox;
class QLineEdit;

class EnumNodeEditor : public AbstractNodeEditor
{
  Q_OBJECT
public:
  EnumNodeEditor(const std::shared_ptr<cxx::Enum> & enm, QWidget *p = nullptr);
  ~EnumNodeEditor() = default;

  void write() override;
  void read(std::shared_ptr<cxx::Enum> enm);

  inline std::shared_ptr<cxx::Enum> getEnum() const { return std::static_pointer_cast<cxx::Enum>(getNode()); }

private:
  QLineEdit *mName;
  QCheckBox *mEnumClass;
};

#endif // METAGONK_ENUM_NODE_EDITOR_H
