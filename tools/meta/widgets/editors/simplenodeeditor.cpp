// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "simplenodeeditor.h"

#include "controller.h"
#include "project-controller.h"

#include <QBoxLayout>
#include <QFontMetrics>
#include <QLineEdit>

SimpleNodeEditor::SimpleNodeEditor(std::shared_ptr<cxx::Entity> node, QWidget *p)
  : AbstractNodeEditor(node, p)
{
  QFontMetrics fm{ font() };

  mName = new QLineEdit();
  mName->setPlaceholderText("name");
  mName->setFixedWidth(fm.width("name+name+name+name") + 8);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(mName);

  setAutoFillBackground(true);

  read(node);
}

void SimpleNodeEditor::write()
{
  // TODO: update the node name
  //Controller::Instance().projectController().update(*getNode(),
  //  mName->text());
}

void SimpleNodeEditor::read(std::shared_ptr<cxx::Entity> node)
{
  setNode(node);

  mName->setText(QString::fromStdString(node->name));
}
