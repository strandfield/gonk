// Copyright (C) 2020-2022 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONKDBG_CALLSTACKVIEW_H
#define GONKDBG_CALLSTACKVIEW_H

#include <QTreeWidget>

#include "client.h"

class Controller;

class CallstackView : public QTreeWidget
{
  Q_OBJECT
public:
  explicit CallstackView(Controller& c);
  ~CallstackView() = default;

Q_SIGNALS:
  void frameDoubleClicked(int n);

protected Q_SLOTS:
  void onCallstackUpdated();
  void onItemDoubleClicked(QTreeWidgetItem* item, int column);

private:
  Controller& m_controller;
};

#endif // GONKDBG_CALLSTACKVIEW_H
