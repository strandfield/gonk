// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include "breakpointsview.h"
#include "callstackview.h"
#include "code-viewer.h"
#include "controller.h"
#include "variablesview.h"

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProcess>
#include <QSettings>
#include <QStatusBar>
#include <QTabWidget>

#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>

#include <QDebug>

MainWindow::MainWindow(int& argc, char** argv)
{
  m_settings = new QSettings("settings.ini", QSettings::IniFormat, this);

  m_controller = new Controller(argc, argv, this);

  m_tabs = new QTabWidget;
  m_tabs->setTabsClosable(false);
  m_tabs->setMovable(true);

  setCentralWidget(m_tabs);

  connect(&(m_controller->client()), &gonk::debugger::Client::connectionEstablished, this, &MainWindow::onSocketConnected);
  connect(&(m_controller->client()), &gonk::debugger::Client::stateChanged, this, &MainWindow::onDebuggerStateChanged);
  connect(m_controller, &Controller::callstackUpdated, this, &MainWindow::onCallstackUpdated);

  {
    m_variables = new VariablesView(*m_controller);
    auto* dock = new QDockWidget(QString("Locals"), this);
    dock->setObjectName("variables-dock");
    dock->setWidget(m_variables);
    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dock);
  }

  {
    m_callstack = new CallstackView(*m_controller);
    connect(m_callstack, &CallstackView::frameDoubleClicked, this, &MainWindow::onFrameSelected);

    auto* dock = new QDockWidget(QString("Callstack"), this);
    dock->setObjectName("callstack-dock");
    dock->setWidget(m_callstack);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, dock);
  }

  {
    m_breakpoints = new BreakpointsView(*m_controller);
    auto* dock = new QDockWidget(QString("Breakpoints"), this);
    dock->setObjectName("breakpoints-dock");
    dock->setWidget(m_breakpoints);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, dock);
  }

  if (m_controller->process())
  {
    m_output = new QPlainTextEdit();
    auto* dock = new QDockWidget(QString("Output"), this);
    dock->setObjectName("output-dock");
    dock->setWidget(m_output);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, dock);

    connect(m_controller->process(), &QProcess::readyReadStandardOutput, this, &MainWindow::onReadyReadStandardOutput);
    connect(m_controller->process(), &QProcess::readyReadStandardError, this, &MainWindow::onReadyReadStandardError);
  }

  {
    m_file_menu = menuBar()->addMenu("File");
    m_exit = m_file_menu->addAction("Exit", []() { QApplication::exit(); });
  }

  {
    m_debug_menu = menuBar()->addMenu("Debugger");

    m_run = m_debug_menu->addAction("Run", m_controller, &Controller::run);
    m_run->setShortcut(QKeySequence(Qt::Key_F5));

    m_pause = m_debug_menu->addAction("Pause", m_controller, &Controller::pause);

    m_step_into = m_debug_menu->addAction("Step into", m_controller, &Controller::stepInto);
    m_step_into->setShortcut(QKeySequence(Qt::Key_F11));

    m_step_over = m_debug_menu->addAction("Step over", m_controller, &Controller::stepOver);
    m_step_over->setShortcut(QKeySequence(Qt::Key_F10));

    m_step_out = m_debug_menu->addAction("Step out", m_controller, &Controller::stepOut);
    m_step_out->setShortcut(QKeySequence(Qt::SHIFT + Qt::Key_F11));
  }


  {
    m_help_menu = menuBar()->addMenu("Help");
    m_about_qt = m_help_menu->addAction("About Qt", []() { QApplication::aboutQt(); });
  }

  onDebuggerStateChanged();
}

void MainWindow::onSocketConnected()
{
  statusBar()->showMessage("Connected!", 1500);
}

void MainWindow::onDebuggerStateChanged()
{
  int s = m_controller->client().state();

  if (s == gonk::debugger::Client::Connecting)
    statusBar()->showMessage("Connecting...", 1000);
  else if (s == gonk::debugger::Client::Disconnected)
    statusBar()->showMessage("Disconnected...");

  if(s == gonk::debugger::Client::DebuggerRunning)
    statusBar()->showMessage("Running!", 1000);
  else if (s == gonk::debugger::Client::DebuggerPaused)
    statusBar()->showMessage("Pause...", 1000);
  else if (s == gonk::debugger::Client::DebuggerFinished)
    statusBar()->showMessage("Done!");

  m_debug_menu->setEnabled(m_controller->client().isConnected());

  if (s == gonk::debugger::Client::DebuggerRunning)
  {
    m_pause->setEnabled(true);
    m_run->setEnabled(false);
    m_step_into->setEnabled(false);
    m_step_over->setEnabled(false);
    m_step_out->setEnabled(false);

    m_callstack->setEnabled(false);
    m_variables->setEnabled(false);
  }
  else if (s == gonk::debugger::Client::DebuggerPaused)
  {
    m_pause->setEnabled(false);
    m_run->setEnabled(true);
    m_step_into->setEnabled(true);
    m_step_over->setEnabled(true);
    m_step_out->setEnabled(true);
  }

  // @TODO: Not the best place to do that, but that will do... for now!
  m_callstack->setEnabled(s == gonk::debugger::Client::DebuggerPaused);
  m_variables->setEnabled(s == gonk::debugger::Client::DebuggerPaused);
}

void MainWindow::onCallstackUpdated()
{
  auto callstack = m_controller->lastCallstackMessage();

  if (callstack == nullptr)
    return;

  const auto& callstack_top = callstack->entries.back();
  std::shared_ptr<gonk::debugger::SourceCode> src = m_controller->getSource(callstack_top.path);

  if (src)
  {
    setCurrentSourceCode(src);
  }
  else
  {
    connect(m_controller, &Controller::sourceCodeReceived, this, &MainWindow::setCurrentSourceCode);
    m_controller->client().getSource(callstack_top.path);
  }
}

void MainWindow::setCurrentSourceCode(std::shared_ptr<gonk::debugger::SourceCode> src)
{
  if (sender() != nullptr)
    disconnect(m_controller, &Controller::sourceCodeReceived, this, &MainWindow::setCurrentSourceCode);

  int index = [&, this]() -> int {

    for (int i(0); i < m_tabs->count(); ++i)
    {
      CodeViewer* viewer = qobject_cast<CodeViewer*>(m_tabs->widget(i));
      
      if (viewer && viewer->documentPath() == QString::fromStdString(src->path))
        return i;
    }

    return -1;
  }();

  if (index != -1)
  {
    m_tabs->setCurrentIndex(index);
    return;
  }

  QFileInfo info{ QString::fromStdString(src->path) };

  CodeViewer* codeview = new CodeViewer(*m_controller, src);
  m_tabs->addTab(codeview, info.fileName());
}

void MainWindow::onFrameSelected(int n)
{
  m_controller->setCurrentFrame(n);
}

void MainWindow::onReadyReadStandardOutput()
{
  m_output->appendPlainText(QString::fromUtf8(m_controller->process()->readAllStandardOutput()));
}

void MainWindow::onReadyReadStandardError()
{
  QString text = QString::fromUtf8(m_controller->process()->readAllStandardError());
  text = text.trimmed();
  QTextCursor cursor{ m_output->document() };
  cursor.movePosition(QTextCursor::End);
  QTextCharFormat fmt;
  fmt.setForeground(Qt::red);
  cursor.setCharFormat(fmt);
  cursor.insertText(text);
  cursor.setCharFormat(QTextCharFormat());
  cursor.insertText("\n");
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
