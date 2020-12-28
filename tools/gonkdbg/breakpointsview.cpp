// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "breakpointsview.h"

#include "controller.h"

#include <QTreeWidgetItem>

#include <QDebug>

static constexpr int NUMBER_COLUMN = 0;
static constexpr int FUNCTION_COLUMN = 1;
static constexpr int FILE_COLUMN = 2;
static constexpr int LINE_COLUMN = 3;

BreakpointsView::BreakpointsView(Controller& c)
  : m_controller(c)
{
  setColumnCount(3);
  setHeaderLabels(QStringList() << "#" << "Function" << "File" << "Line");

  connect(&m_controller, &Controller::breakpointsUpdated, this, &BreakpointsView::onBreakpointsUpdated);
}

void BreakpointsView::onBreakpointsUpdated()
{
  auto& breakpoints = *m_controller.lastBreakpointListMessage();

  clear();

  for (const gonk::debugger::BreakpointData& bp : breakpoints.list)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem;
    item->setFlags(Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemNeverHasChildren | Qt::ItemFlag::ItemIsSelectable);

    item->setText(NUMBER_COLUMN, QString::number(bp.id));
    item->setText(FUNCTION_COLUMN, QString::fromStdString(bp.function));
    item->setText(FILE_COLUMN, QString::fromStdString(bp.script_path));
    item->setText(LINE_COLUMN, QString::number(bp.line + 1));

    addTopLevelItem(item);
  }
}
