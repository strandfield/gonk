// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "classnodeeditor.h"

#include "controller.h"
#include "project-controller.h"

#include <QBoxLayout>
#include <QCheckBox>
#include <QFontMetrics>
#include <QLineEdit>

ClassNodeEditor::ClassNodeEditor(const std::shared_ptr<cxx::Class> & cla, QWidget *p)
  : AbstractNodeEditor(cla, p)
{
  QFontMetrics fm{ font() };

  mName = new QLineEdit();
  mName->setPlaceholderText("name");
  mName->setFixedWidth(fm.width("name+name+name+name") + 8);

  mFinal = new QCheckBox("final");

  mBase = new QLineEdit();
  mBase->setPlaceholderText("base");
  mBase->setFixedWidth(fm.width("name+name+name+name") + 8);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(mName);
  layout->addWidget(mFinal);
  layout->addWidget(mBase);

  setAutoFillBackground(true);

  read(cla);
}

void ClassNodeEditor::write()
{
  auto cla = getClass();

  Controller::Instance().projectController().update(*cla,
    mName->text(),
    mFinal->isChecked(),
    mBase->text());
}

void ClassNodeEditor::read(std::shared_ptr<cxx::Class> cla)
{
  setNode(cla);

  mName->setText(QString::fromStdString(cla->name));
  mFinal->setChecked(cla->is_final);
  
  if(!cla->bases.empty())
    mBase->setText(QString::fromStdString(cla->bases.front().base->name));
}
