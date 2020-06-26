// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "enumnodeeditor.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QFontMetrics>
#include <QLineEdit>

EnumNodeEditor::EnumNodeEditor(const std::shared_ptr<cxx::Enum>& enm, QWidget *p)
  : AbstractNodeEditor(enm, p)
{
  QFontMetrics fm{ font() };

  mName = new QLineEdit();
  mName->setPlaceholderText("name");
  mName->setFixedWidth(fm.width("name+name+name+name") + 8);

  mToEnumClass = new QCheckBox("to-enum-class");
  mFromEnumClass = new QCheckBox("from-enum-class");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(mName);
  layout->addWidget(mToEnumClass);
  layout->addWidget(mFromEnumClass);

  setAutoFillBackground(true);

  read(enm);
}

void EnumNodeEditor::write()
{
  auto enm = getEnum();

  enm->name = mName->text().toStdString();
  //enm->isEnumClass = mToEnumClass->isChecked();
  //enm->isCppEnumClass = mFromEnumClass->isChecked();
}

void EnumNodeEditor::read(std::shared_ptr<cxx::Enum> enm)
{
  setNode(enm);

  mName->setText(QString::fromStdString(enm->name));
  //mToEnumClass->setChecked(enm->isEnumClass);
  //mFromEnumClass->setChecked(enm->isCppEnumClass);
}
