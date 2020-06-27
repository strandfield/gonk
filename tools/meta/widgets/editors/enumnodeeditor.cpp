// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "enumnodeeditor.h"

#include "controller.h"
#include "project-controller.h"

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

  mEnumClass = new QCheckBox("enum-class");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(mName);
  layout->addWidget(mEnumClass);

  setAutoFillBackground(true);

  read(enm);
}

void EnumNodeEditor::write()
{
  auto enm = getEnum();

  Controller::Instance().projectController().update(*enm,
    mName->text(),
    mEnumClass->isChecked());
}

void EnumNodeEditor::read(std::shared_ptr<cxx::Enum> enm)
{
  setNode(enm);

  mName->setText(QString::fromStdString(enm->name));
  mEnumClass->setChecked(enm->enum_class);
}
