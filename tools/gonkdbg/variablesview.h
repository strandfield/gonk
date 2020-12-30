// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONKDBG_VARIABLESVIEW_H
#define GONKDBG_VARIABLESVIEW_H

#include <QTreeWidget>

#include <debugger/client.h>

class Controller;

class QTreeWidgetItem;

class VariablesView : public QTreeWidget
{
  Q_OBJECT
public:
  explicit VariablesView(Controller& c);
  ~VariablesView() = default;

protected Q_SLOTS:
  void onVariablesUpdated();
  void onCurrentFrameChanged(int n);

protected:
  QTreeWidgetItem* createItem(const gonk::debugger::Variable& var);

private:
  Controller& m_controller;
};

#endif // GONKDBG_VARIABLESVIEW_H
