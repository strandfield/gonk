// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "enumnodeeditor.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QFontMetrics>
#include <QLineEdit>

EnumNodeEditor::EnumNodeEditor(const EnumRef & enm, QWidget *p)
  : AbstractNodeEditor(enm, p)
{
  QFontMetrics fm{ font() };

  mCondition = new QLineEdit;
  mCondition->setPlaceholderText("condition");
  mCondition->setFixedWidth(fm.width("condition") + 8);

  mName = new QLineEdit();
  mName->setPlaceholderText("name");
  mName->setFixedWidth(fm.width("name+name+name+name") + 8);

  mToEnumClass = new QCheckBox("to-enum-class");
  mFromEnumClass = new QCheckBox("from-enum-class");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(mCondition);
  layout->addWidget(mName);
  layout->addWidget(mToEnumClass);
  layout->addWidget(mFromEnumClass);

  setAutoFillBackground(true);

  read(enm);
}

void EnumNodeEditor::write()
{
  auto enm = getEnum();

  enm->condition = mCondition->text().toStdString();
  enm->name = mName->text();
  enm->isEnumClass = mToEnumClass->isChecked();
  enm->isCppEnumClass = mFromEnumClass->isChecked();
}

void EnumNodeEditor::read(EnumRef enm)
{
  setNode(enm);

  mCondition->setText(QString::fromStdString(enm->condition));
  mName->setText(enm->name);
  mToEnumClass->setChecked(enm->isEnumClass);
  mFromEnumClass->setChecked(enm->isCppEnumClass);
}
