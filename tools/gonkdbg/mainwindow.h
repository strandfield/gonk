// Copyright (C) 2020-2022 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONKDBG_MAINWINDOW_H
#define GONKDBG_MAINWINDOW_H

#include <QMainWindow>

#include "client.h"

#include <QSettings>

class BreakpointsView;
class CallstackView;
class Controller;
class VariablesView;

class QAction;
class QPlainTextEdit;
class QTabWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(int& argc, char** argv);
  ~MainWindow() = default;

protected Q_SLOTS:
  void onSocketConnected();
  void onDebuggerStateChanged();
  void onCallstackUpdated();
  void setCurrentSourceCode(std::shared_ptr<gonk::debugger::SourceCode> src);
  void onFrameSelected(int n);
  void onReadyReadStandardOutput();
  void onReadyReadStandardError();

protected:
  void showEvent(QShowEvent *e);
  void closeEvent(QCloseEvent *e);

private:
  QSettings *m_settings = nullptr;
  Controller* m_controller = nullptr;
  QTabWidget* m_tabs = nullptr;
  CallstackView* m_callstack = nullptr;
  BreakpointsView* m_breakpoints = nullptr;
  VariablesView* m_variables = nullptr;
  QPlainTextEdit* m_output = nullptr;
  /* File menu */
  QMenu* m_file_menu = nullptr;
  QAction* m_exit = nullptr;
  /* Debugger menu */
  QMenu* m_debug_menu = nullptr;
  QAction* m_run = nullptr;
  QAction* m_pause = nullptr;
  QAction* m_step_into = nullptr;
  QAction* m_step_over = nullptr;
  QAction* m_step_out = nullptr;
  /* Help menu */
  QMenu* m_help_menu = nullptr;
  QAction* m_about_qt = nullptr;
};

#endif // GONKDBG_MAINWINDOW_H
