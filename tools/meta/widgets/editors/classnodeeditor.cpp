// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "classnodeeditor.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QFontMetrics>
#include <QLineEdit>

ClassNodeEditor::ClassNodeEditor(const ClassRef & cla, QWidget *p)
  : AbstractNodeEditor(cla, p)
{
  QFontMetrics fm{ font() };

  mCondition = new QLineEdit;
  mCondition->setPlaceholderText("condition");
  mCondition->setFixedWidth(fm.width("condition") + 8);

  mName = new QLineEdit();
  mName->setPlaceholderText("name");
  mName->setFixedWidth(fm.width("name+name+name+name") + 8);

  mFinal = new QCheckBox("final");

  mBase = new QLineEdit();
  mBase->setPlaceholderText("base");
  mBase->setFixedWidth(fm.width("name+name+name+name") + 8);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(mCondition);
  layout->addWidget(mName);
  layout->addWidget(mFinal);
  layout->addWidget(mBase);

  setAutoFillBackground(true);

  read(cla);
}

void ClassNodeEditor::write()
{
  auto cla = getClass();

  cla->condition = mCondition->text();
  cla->name = mName->text();
  cla->isFinal = mFinal->isChecked();
  cla->base = mBase->text();
}

void ClassNodeEditor::read(ClassRef cla)
{
  setNode(cla);

  mCondition->setText(cla->condition);
  mName->setText(cla->name);
  mFinal->setChecked(cla->isFinal);
  mBase->setText(cla->base);
}
