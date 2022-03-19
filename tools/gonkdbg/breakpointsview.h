// Copyright (C) 2020-2022 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONKDBG_BREAKPOINTSVIEW_H
#define GONKDBG_BREAKPOINTSVIEW_H

#include <QTreeWidget>

#include "client.h"

class Controller;

class BreakpointsView : public QTreeWidget
{
  Q_OBJECT
public:
  explicit BreakpointsView(Controller& c);
  ~BreakpointsView() = default;

protected Q_SLOTS:
  void onBreakpointsUpdated();

private:
  Controller& m_controller;
};

#endif // GONKDBG_BREAKPOINTSVIEW_H
