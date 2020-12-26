// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONKDBG_MAINWINDOW_H
#define GONKDBG_MAINWINDOW_H

#include <QMainWindow>

#include <debugger/client.h>

#include <typewriter/../../widget/codeeditor.h>

#include <QSettings>

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
  void onDebuggerRunning();
  void onDebuggerPaused();
  void onDebuggerFinished();
  void onMessageReceived(std::shared_ptr<gonk::debugger::DebuggerMessage> mssg);
  void pause();
  void run();
  void stepInto();
  void stepOver();
  void stepOut();

protected:
  void showEvent(QShowEvent *e);
  void closeEvent(QCloseEvent *e);

private:
  QSettings *m_settings = nullptr;
  typewriter::QTypewriter* m_editor = nullptr;
  std::string m_source_path;
  gonk::debugger::Client* m_client = nullptr;
  bool m_has_source = false;
  bool m_debugger_paused = false;
  bool m_debugger_done = false;
  QListWidget* m_callstack = nullptr;
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
