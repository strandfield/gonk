// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "functionnodeeditor.h"

#include "database.h"
#include "controller.h"
#include "project-controller.h"

#include <QBoxLayout>
#include <QFontMetrics>
#include <QLineEdit>

FunctionNodeEditor::FunctionNodeEditor(const std::shared_ptr<cxx::Function>& func, QWidget *p)
  : AbstractNodeEditor(func, p)
{
  QFontMetrics fm{ font() };

  mReturnType = new QLineEdit();
  mReturnType->setPlaceholderText("return-type");
  mReturnType->setFixedWidth(fm.width("return-type") + 8);

  mName = new QLineEdit();
  mName->setPlaceholderText("name");
  mName->setFixedWidth(fm.width("name") + 20);

  mParameters = new QLineEdit;
  mParameters->setPlaceholderText("param1;...");

  mSpecifiers = new QLineEdit;
  mSpecifiers->setPlaceholderText("const,static,...");
  mSpecifiers->setFixedWidth(fm.width("const,static,...") + 8);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(mReturnType);
  layout->addWidget(mName);
  layout->addWidget(mParameters);
  layout->addWidget(mSpecifiers);

  setAutoFillBackground(true);

  read(func);
}

void FunctionNodeEditor::write()
{
  auto mFunction = getFunction();

  Controller::Instance().projectController().update(*mFunction,
    mName->text(),
    mReturnType->text(),
    mParameters->text().simplified().split(';', QString::SkipEmptyParts),
    mSpecifiers->text().split(','));
}

void FunctionNodeEditor::read(std::shared_ptr<cxx::Function> fun)
{
  setNode(fun);

  mReturnType->setText(QString::fromStdString(fun->return_type.toString()));
  mName->setText(QString::fromStdString(fun->name));

  QStringList params;
  for (auto p : fun->parameters)
  {
    params.append(QString::fromStdString(p->type.toString()));
  }
  mParameters->setText(params.join(";"));

  mSpecifiers->setText(Database::specifiers(*fun));
}

