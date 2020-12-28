// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "callstackview.h"

#include "controller.h"

#include <QTreeWidgetItem>

#include <QDebug>

static constexpr int NUMBER_COLUMN = 0;
static constexpr int FUNCTION_COLUMN = 1;
static constexpr int FILE_COLUMN = 2;
static constexpr int LINE_COLUMN = 3;

CallstackView::CallstackView(Controller& c)
  : m_controller(c)
{
  setColumnCount(4);
  setHeaderLabels(QStringList() << "#" << "Function" << "File" << "Line");

  connect(&m_controller, &Controller::callstackUpdated, this, &CallstackView::onCallstackUpdated);

  connect(this, &QTreeWidget::itemDoubleClicked, this, &CallstackView::onItemDoubleClicked);
}

void CallstackView::onCallstackUpdated()
{
  auto& callstack = *m_controller.lastCallstackMessage();

  clear();

  int d = static_cast<int>(callstack.entries.size());

  for (auto it = callstack.entries.rbegin(); it != callstack.entries.rend(); ++it)
  {
    const gonk::debugger::CallstackEntry& e = *it;

    QTreeWidgetItem* item = new QTreeWidgetItem;
    item->setFlags(Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemNeverHasChildren);

    item->setText(NUMBER_COLUMN, QString::number(d--));
    item->setText(FUNCTION_COLUMN, QString::fromStdString(e.function));
    item->setText(FILE_COLUMN, QString::fromStdString(e.path));

    if (e.line != -1)
    {
      item->setFlags(item->flags() | Qt::ItemFlag::ItemIsSelectable);
      item->setText(LINE_COLUMN, QString::number(e.line + 1));
    }

    addTopLevelItem(item);
  }
}

void CallstackView::onItemDoubleClicked(QTreeWidgetItem* item, int column)
{
  if (item)
  {
    int n = item->text(NUMBER_COLUMN).toInt() - 1;
    Q_EMIT frameDoubleClicked(n);
  }
}
