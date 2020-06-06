// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_NEWFUNCTIONDIALOG_H
#define METAGONK_NEWFUNCTIONDIALOG_H

#include <QDialog>

#include "project/function.h"

class QComboBox;
class QLineEdit;

class NewFunctionDialog : public QDialog
{
  Q_OBJECT
public:
  NewFunctionDialog(QWidget *parent = nullptr);
  NewFunctionDialog(FunctionRef fun, QWidget *parent = nullptr);
  ~NewFunctionDialog() = default;

  const FunctionRef & function() const { return mFunction; }
  void sync();

private:
  void setup();

private:
  QLineEdit *mNameLineEdit;
  QLineEdit *mReturnTypeLineEdit;
  QLineEdit *mParametersLineEdit;
  QLineEdit *mSpecifiersLineEdit;
  QComboBox *mBindingMethodComboBox;
  QLineEdit* m_impl_lineedit;
  QLineEdit* m_condition_lineedit;
  FunctionRef mFunction;
};

#endif // METAGONK_NEWFUNCTIONDIALOG_H
