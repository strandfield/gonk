// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "dialogs/newfunctiondialog.h"

#include "controller.h"
#include "database.h"
#include "project-controller.h"

#include <QComboBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

NewFunctionDialog::NewFunctionDialog(QWidget *parent)
  : QDialog(parent)
{
  setup();
  mFunction = std::make_shared<cxx::Function>("");
}

NewFunctionDialog::NewFunctionDialog(std::shared_ptr<cxx::Function> fun, QWidget *parent)
  : QDialog(parent)
{
  setup();
  mFunction = fun;

  mNameLineEdit->setText(QString::fromStdString(fun->name));
  mReturnTypeLineEdit->setText(QString::fromStdString(fun->return_type.toString()));
  mParametersLineEdit->setText(Database::parameters(*fun));
  
  QString specifiers = Database::specifiers(*fun);
  mSpecifiersLineEdit->setText(specifiers);
}

void NewFunctionDialog::setup()
{
  mNameLineEdit = new QLineEdit();
  mReturnTypeLineEdit = new QLineEdit();
  mParametersLineEdit = new QLineEdit();
  mSpecifiersLineEdit = new QLineEdit();

  auto *form = new QFormLayout();
  form->addRow("Name:", mNameLineEdit);
  form->addRow("Return type:", mReturnTypeLineEdit);
  form->addRow("Parameters:", mParametersLineEdit);
  form->addRow("Specifiers:", mSpecifiersLineEdit);

  auto *okButton = new QPushButton("OK");
  connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

  auto *layout = new QVBoxLayout(this);
  layout->addLayout(form);
  layout->addWidget(okButton, 0, Qt::AlignCenter);
}

void NewFunctionDialog::sync()
{
  Controller::Instance().projectController().update(*mFunction,
    mNameLineEdit->text(),
    mReturnTypeLineEdit->text().simplified(),
    mParametersLineEdit->text().simplified().split(";", QString::SkipEmptyParts),
    mSpecifiersLineEdit->text().simplified().split(','));
}

