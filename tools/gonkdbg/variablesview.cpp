// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "variablesview.h"

#include "controller.h"

#include <QTreeWidgetItem>

#include <QDebug>

static constexpr int NAME_COLUMN = 0;
static constexpr int VALUE_COLUMN = 1;
static constexpr int TYPE_COLUMN = 2;

VariablesView::VariablesView(Controller& c)
  : m_controller(c)
{
  setColumnCount(3);
  setHeaderLabels(QStringList() << "Name" << "Value" << "Type");

  connect(&m_controller, &Controller::variablesUpdated, this, &VariablesView::onVariablesUpdated);
}

void VariablesView::onVariablesUpdated()
{
  auto& variables = *m_controller.lastVariablesMessage();

  clear();

  for (const gonk::debugger::Variable& v : variables.variables)
  {
    QTreeWidgetItem* item = new QTreeWidgetItem;
    item->setFlags(Qt::ItemFlag::ItemIsEnabled | Qt::ItemFlag::ItemNeverHasChildren);

    item->setText(NAME_COLUMN, QString::fromStdString(v.name));
    item->setText(TYPE_COLUMN, QString::fromStdString(v.type));

    if (v.value.isBool())
      item->setText(VALUE_COLUMN, v.value.toBool() ? "true" : "false");
    else if (v.value.isDouble())
      item->setText(VALUE_COLUMN, QString::number(v.value.toDouble()));
    else if (v.value.isString())
      item->setText(VALUE_COLUMN, v.value.toString());

    addTopLevelItem(item);
  }
}
