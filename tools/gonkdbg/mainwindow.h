// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONKDBG_MAINWINDOW_H
#define GONKDBG_MAINWINDOW_H

#include <QMainWindow>

#include "debugger-client.h"

#include <QSettings>

#include <typewriter/../../widget/codeeditor.h>

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
  void onSourceCodeReceived(QString src);
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
  gonk::debugger::Client* m_client = nullptr;
  bool m_has_source = false;
  bool m_debugger_paused = false;
  bool m_debugger_done = false;
  QListWidget* m_callstack = nullptr;
  QMenu* m_debug_menu = nullptr;
  QAction* m_run = nullptr;
  QAction* m_pause = nullptr;
  QAction* m_step_into = nullptr;
  QAction* m_step_over = nullptr;
  QAction* m_step_out = nullptr;
};

#endif // GONKDBG_MAINWINDOW_H
