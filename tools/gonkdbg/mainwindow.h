// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONKDBG_MAINWINDOW_H
#define GONKDBG_MAINWINDOW_H

#include <QMainWindow>

#include <debugger/client.h>

#include <typewriter/../../widget/codeeditor.h>

#include <QSettings>

class CallstackView;
class Controller;

class QAction;
class QListWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow();
  ~MainWindow() = default;

protected Q_SLOTS:
  void onSocketConnected();
  void onDebuggerStateChanged();
  void onCallstackUpdated();
  void onBreakpointsUpdated();
  void onVariablesUpdated();
  void setSourceCode(std::shared_ptr<gonk::debugger::SourceCode> src);

protected:
  void showEvent(QShowEvent *e);
  void closeEvent(QCloseEvent *e);

private:
  QSettings *m_settings = nullptr;
  Controller* m_controller = nullptr;
  typewriter::QTypewriter* m_editor = nullptr;
  std::string m_source_path;
  CallstackView* m_callstack = nullptr;
  QListWidget* m_breakpoints = nullptr;
  QListWidget* m_variables = nullptr;
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
