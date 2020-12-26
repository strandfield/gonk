// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "callstackview.h"

#include "controller.h"

#include <QTreeWidgetItem>

#include <QDebug>

CallstackView::CallstackView(Controller& c)
  : m_controller(c)
{
  setColumnCount(3);
  setHeaderLabels(QStringList() << "Function" << "File" << "Line");

  connect(&m_controller, &Controller::callstackUpdated, this, &CallstackView::onCallstackUpdated);
}

void CallstackView::onCallstackUpdated()
{
  auto& callstack = *m_controller.lastCallstackMessage();

  clear();

  for (const gonk::debugger::CallstackEntry& e : callstack.entries)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem;
    item->setFlags(Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemNeverHasChildren);

    item->setText(0, QString::fromStdString(e.function));
    item->setText(1, QString::fromStdString(e.path));

    if (e.line != -1)
    {
      item->setFlags(item->flags() | Qt::ItemFlag::ItemIsSelectable);
      item->setText(2, QString::number(e.line + 1));
    }

    addTopLevelItem(item);
  }
}
