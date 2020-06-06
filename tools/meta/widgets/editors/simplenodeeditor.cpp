// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "simplenodeeditor.h"

#include "controller.h"
#include "project-controller.h"

#include <QBoxLayout>
#include <QFontMetrics>
#include <QLineEdit>

SimpleNodeEditor::SimpleNodeEditor(NodeRef node, QWidget *p)
  : AbstractNodeEditor(node, p)
{
  QFontMetrics fm{ font() };

  mCondition = new QLineEdit;
  mCondition->setPlaceholderText("condition");
  mCondition->setFixedWidth(fm.width("condition") + 8);

  mName = new QLineEdit();
  mName->setPlaceholderText("name");
  mName->setFixedWidth(fm.width("name+name+name+name") + 8);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(mCondition);
  layout->addWidget(mName);

  setAutoFillBackground(true);

  read(node);
}

void SimpleNodeEditor::write()
{
  Controller::Instance().projectController().update(*getNode(),
    mName->text(),
    mCondition->text());
}

void SimpleNodeEditor::read(NodeRef node)
{
  setNode(node);

  mCondition->setText(node->condition);
  mName->setText(node->name);
}
