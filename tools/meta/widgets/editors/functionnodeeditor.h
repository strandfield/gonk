// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_FUNCTION_NODE_EDITOR_H
#define METAGONK_FUNCTION_NODE_EDITOR_H

#include "abstractnodeeditor.h"

#include <cxx/function.h>

class QComboBox;
class QLineEdit;

class FunctionNodeEditor : public AbstractNodeEditor
{
  Q_OBJECT
public:
  FunctionNodeEditor(const std::shared_ptr<cxx::Function>& func, QWidget *p = nullptr);
  ~FunctionNodeEditor() = default;

  void write() override;
  void read(std::shared_ptr<cxx::Function> fun);

  inline std::shared_ptr<cxx::Function> getFunction() const { return std::static_pointer_cast<cxx::Function>(getNode()); }

private:
  QLineEdit *mReturnType;
  QLineEdit *mName;
  QLineEdit *mParameters;
  QLineEdit* mSpecifiers;
};

#endif // METAGONK_FUNCTION_NODE_EDITOR_H
