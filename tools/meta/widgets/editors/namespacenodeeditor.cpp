// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "namespacenodeeditor.h"

#include "controller.h"
#include "project-controller.h"

#include <QBoxLayout>
#include <QFontMetrics>
#include <QLineEdit>

NamespaceNodeEditor::NamespaceNodeEditor(const std::shared_ptr<cxx::Namespace>& ns, QWidget *p)
  : AbstractNodeEditor(ns, p)
{
  QFontMetrics fm{ font() };

  mName = new QLineEdit();
  mName->setPlaceholderText("name");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(mName);

  setAutoFillBackground(true);

  read(ns);
}

void NamespaceNodeEditor::write()
{
  auto ns = getNamespace();

  Controller::Instance().projectController().update(*ns, mName->text());
}

void NamespaceNodeEditor::read(std::shared_ptr<cxx::Namespace> ns)
{
  setNode(ns);

  mName->setText(QString::fromStdString(ns->name));
}
