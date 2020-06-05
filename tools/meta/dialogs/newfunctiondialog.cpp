// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "dialogs/newfunctiondialog.h"

#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

NewFunctionDialog::NewFunctionDialog(QWidget *parent)
  : QDialog(parent)
{
  setup();
  mFunction = std::make_shared<Function>("");
}

NewFunctionDialog::NewFunctionDialog(FunctionRef fun, QWidget *parent)
  : QDialog(parent)
{
  setup();
  mFunction = fun;

  mNameLineEdit->setText(fun->name);
  mReturnTypeLineEdit->setText(fun->returnType);
  mParametersLineEdit->setText(fun->parameters.join(";"));
  
  QStringList specifiers = fun->getSpecifiers();
  mSpecifiersLineEdit->setText(specifiers.join(","));

  mBindingMethodComboBox->setCurrentIndex(fun->bindingMethod - Function::FirstBindingMethod);

  m_impl_lineedit->setText(fun->implementation);
  m_condition_lineedit->setText(fun->condition);
}

void NewFunctionDialog::setup()
{
  mNameLineEdit = new QLineEdit();
  mReturnTypeLineEdit = new QLineEdit();
  mParametersLineEdit = new QLineEdit();
  mSpecifiersLineEdit = new QLineEdit();

  mBindingMethodComboBox = new QComboBox();
  for (int i(Function::FirstBindingMethod); i <= Function::LastBindingMethod; ++i)
    mBindingMethodComboBox->addItem(Function::serialize(static_cast<Function::BindingMethod>(i)));

  m_impl_lineedit = new QLineEdit();
  m_condition_lineedit = new QLineEdit();

  auto *form = new QFormLayout();
  form->addRow("Name:", mNameLineEdit);
  form->addRow("Return type:", mReturnTypeLineEdit);
  form->addRow("Parameters:", mParametersLineEdit);
  form->addRow("Specifiers:", mSpecifiersLineEdit);
  form->addRow("Binding:", mBindingMethodComboBox);
  form->addRow("Impl:", m_impl_lineedit);
  form->addRow("Condition:", m_condition_lineedit);

  auto *okButton = new QPushButton("OK");
  connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

  auto *layout = new QVBoxLayout(this);
  layout->addLayout(form);
  layout->addWidget(okButton, 0, Qt::AlignCenter);
}

void NewFunctionDialog::sync()
{
  mFunction->name = mNameLineEdit->text();
  mFunction->returnType = mReturnTypeLineEdit->text().simplified();
  mFunction->parameters = mParametersLineEdit->text().simplified().split(";", QString::SkipEmptyParts);
  mFunction->bindingMethod = Function::deserialize<Function::BindingMethod>(mBindingMethodComboBox->currentText());
  mFunction->implementation = m_impl_lineedit->text();
  mFunction->condition = m_condition_lineedit->text();

  QString specifiers = mSpecifiersLineEdit->text();
  mFunction->setSpecifiers(specifiers.split(','));
}

