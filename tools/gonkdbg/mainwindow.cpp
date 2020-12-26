// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QStatusBar>

#include <QDebug>

MainWindow::MainWindow()
{
  m_settings = new QSettings("settings.ini", QSettings::IniFormat, this);

  const char* source =
    "\n"
    "void main()\n"
    "{\n"
    "  print(66);\n"
    "}\n"
    "\n";

  m_editor = new typewriter::QTypewriter(new typewriter::TextDocument(source));

  setCentralWidget(m_editor);

  m_client = new gonk::debugger::Client;
  m_client->setParent(this);
  connect(m_client, &gonk::debugger::Client::connectionEstablished, this, &MainWindow::onSocketConnected);

  {
    m_variables = new QListWidget;
    auto* dock = new QDockWidget(this);
    dock->setWidget(m_variables);
    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dock);
  }

  {
    m_callstack = new QListWidget;
    auto* dock = new QDockWidget(this);
    dock->setWidget(m_callstack);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, dock);
  }

  {
    m_breakpoints = new QListWidget;
    auto* dock = new QDockWidget(this);
    dock->setWidget(m_breakpoints);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, dock);
  }

  {
    m_file_menu = menuBar()->addMenu("File");
    m_exit = m_file_menu->addAction("Exit", []() { QApplication::exit(); });
  }

  {
    m_debug_menu = menuBar()->addMenu("Debugger");

    m_run = m_debug_menu->addAction("Run", this, &MainWindow::run);
    m_run->setShortcut(QKeySequence(Qt::Key_F5));

    m_pause = m_debug_menu->addAction("Pause", this, &MainWindow::pause);

    m_step_into = m_debug_menu->addAction("Step into", this, &MainWindow::stepInto);
    m_step_into->setShortcut(QKeySequence(Qt::Key_F11));

    m_step_over = m_debug_menu->addAction("Step over", this, &MainWindow::stepOver);
    m_step_over->setShortcut(QKeySequence(Qt::Key_F10));

    m_step_out = m_debug_menu->addAction("Step out", this, &MainWindow::stepOut);
    m_step_out->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F11));
  }


  {
    m_help_menu = menuBar()->addMenu("Help");
    m_about_qt = m_help_menu->addAction("About Qt", []() { QApplication::aboutQt(); });
  }
}

void MainWindow::onSocketConnected()
{
  connect(m_client, &gonk::debugger::Client::debuggerRunning, this, &MainWindow::onDebuggerRunning);
  connect(m_client, &gonk::debugger::Client::debuggerPaused, this, &MainWindow::onDebuggerPaused);
  connect(m_client, &gonk::debugger::Client::debuggerFinished, this, &MainWindow::onDebuggerFinished);
  connect(m_client, &gonk::debugger::Client::sourceCodeReceived, this, &MainWindow::onSourceCodeReceived);
  connect(m_client, &gonk::debugger::Client::messageReceived, this, &MainWindow::onMessageReceived);

  statusBar()->showMessage("Connected!", 1500);
}

void MainWindow::onDebuggerRunning()
{
  m_debugger_paused = false;
  statusBar()->showMessage("Running!", 1000);

  m_pause->setEnabled(true);
  m_run->setEnabled(false);
  m_step_into->setEnabled(false);
  m_step_over->setEnabled(false);
  m_step_out->setEnabled(false);
}

void MainWindow::onDebuggerPaused()
{
  m_debugger_paused = true;

  m_pause->setEnabled(false);  
  m_run->setEnabled(true);
  m_step_into->setEnabled(true);
  m_step_over->setEnabled(true);
  m_step_out->setEnabled(true);

  statusBar()->showMessage("Pause...", 1000);

  if (!m_has_source)
  {
    m_client->getSource();
  }

  m_client->getCallstack();
  m_client->getBreakpoints();
  m_client->getVariables();
}

void MainWindow::onDebuggerFinished()
{
  statusBar()->showMessage("Done!");
  m_debug_menu->setEnabled(false);
  qDebug() << "done";
  m_debugger_done = true;
}

void MainWindow::onSourceCodeReceived(QString src)
{
  typewriter::TextCursor cursor{ m_editor->document() };
  cursor.setPosition(typewriter::Position{ m_editor->document()->lineCount() + 1, 0 }, typewriter::TextCursor::KeepAnchor);
  cursor.removeSelectedText();
  std::string src_str = src.toStdString();
  cursor.insertText(src_str);

  m_has_source = true;
}

void MainWindow::onMessageReceived(std::shared_ptr<gonk::debugger::DebuggerMessage> mssg)
{
  if (!mssg)
    return;

  if (dynamic_cast<gonk::debugger::Callstack*>(mssg.get()))
  {
    auto& callstack = static_cast<gonk::debugger::Callstack&>(*mssg);
    m_callstack->clear();

    for (const std::string& f : callstack.functions)
    {
      m_callstack->addItem(QString::fromStdString(f));
    }
  }
  else if (dynamic_cast<gonk::debugger::BreakpointList*>(mssg.get()))
  {
    auto& breakpoints = static_cast<gonk::debugger::BreakpointList&>(*mssg);
    m_breakpoints->clear();

    for (const gonk::debugger::BreakpointData& bp : breakpoints.list)
    {
      m_breakpoints->addItem(QString::fromStdString(bp.function) + " (" + QString::number(bp.line) + ")");
    }
  }
  else if (dynamic_cast<gonk::debugger::VariableList*>(mssg.get()))
  {
    auto& variables = static_cast<gonk::debugger::VariableList&>(*mssg);
    m_variables->clear();

    for (const gonk::debugger::Variable& v : variables.variables)
    {
      if(v.value.isBool())
        m_variables->addItem(QString::fromStdString(v.name) + " : " + (v.value.toBool() ? "true" : "false"));
      else if (v.value.isDouble())
        m_variables->addItem(QString::fromStdString(v.name) + " : " + QString::number(v.value.toDouble()));
      else if (v.value.isString())
        m_variables->addItem(QString::fromStdString(v.name) + " : " + v.value.toString());
    }
  }
}

void MainWindow::pause()
{
  if (m_debugger_paused)
  {
    m_client->action(gonk::debugger::Client::Action::Pause);
  }
}

void MainWindow::run()
{
  if (m_debugger_paused)
  {
    m_client->action(gonk::debugger::Client::Action::Run);
  }
}

void MainWindow::stepInto()
{
  if (m_debugger_paused)
  {
    m_client->action(gonk::debugger::Client::Action::StepInto);
  }
}

void MainWindow::stepOver()
{
  if (m_debugger_paused)
  {
    m_client->action(gonk::debugger::Client::Action::StepOver);
  }
}

void MainWindow::stepOut()
{
  if (m_debugger_paused)
  {
    m_client->action(gonk::debugger::Client::Action::StepOut);
  }
}

void MainWindow::showEvent(QShowEvent *e)
{
  if (m_settings->contains("geometry"))
    restoreGeometry(m_settings->value("geometry").toByteArray());

  if (m_settings->contains("windowState"))
    restoreState(m_settings->value("windowState").toByteArray());

  statusBar()->showMessage("Connecting...", 1500);

  QMainWindow::showEvent(e);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
  m_settings->setValue("geometry", saveGeometry());
  m_settings->setValue("windowState", saveState());
  QMainWindow::closeEvent(e);
}
