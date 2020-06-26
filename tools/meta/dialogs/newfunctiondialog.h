// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_NEWFUNCTIONDIALOG_H
#define METAGONK_NEWFUNCTIONDIALOG_H

#include <QDialog>

#include <cxx/function.h>

class QComboBox;
class QLineEdit;

class NewFunctionDialog : public QDialog
{
  Q_OBJECT
public:
  NewFunctionDialog(QWidget *parent = nullptr);
  NewFunctionDialog(std::shared_ptr<cxx::Function> fun, QWidget *parent = nullptr);
  ~NewFunctionDialog() = default;

  const std::shared_ptr<cxx::Function>& function() const { return mFunction; }
  void sync();

private:
  void setup();

private:
  QLineEdit *mNameLineEdit;
  QLineEdit *mReturnTypeLineEdit;
  QLineEdit *mParametersLineEdit;
  QLineEdit *mSpecifiersLineEdit;
  std::shared_ptr<cxx::Function> mFunction;
};

#endif // METAGONK_NEWFUNCTIONDIALOG_H
