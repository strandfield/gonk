// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include "breakpointsview.h"
#include "callstackview.h"
#include "controller.h"
#include "syntaxhighlighter.h"
#include "variablesview.h"

#include <QAction>
#include <QApplication>
#include <QDockWidget>
#include <QListWidget>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QSettings>
#include <QStatusBar>

#include <QFile>
#include <QJsonDocument>

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

  typewriter::TextFormat fmt = m_editor->defaultFormat();
  fmt.text_color = QColor(Qt::yellow);
  m_editor->setTextFormat(1, fmt);
  fmt.text_color = QColor(Qt::blue);
  m_editor->setTextFormat(2, fmt);
  fmt.text_color = QColor(Qt::green);
  m_editor->setTextFormat(3, fmt);

  connect(m_editor->gutter(), &typewriter::QTypewriterGutter::clicked, this, &MainWindow::onGutterLineClicked);

  setCentralWidget(m_editor);

  m_controller = new Controller(this);

  connect(&(m_controller->client()), &gonk::debugger::Client::connectionEstablished, this, &MainWindow::onSocketConnected);
  connect(m_controller, &Controller::debuggerStateChanged, this, &MainWindow::onDebuggerStateChanged);
  connect(m_controller, &Controller::callstackUpdated, this, &MainWindow::onCallstackUpdated);
  connect(m_controller, &Controller::breakpointsUpdated, this, &MainWindow::onBreakpointsUpdated);

  {
    m_variables = new VariablesView(*m_controller);
    auto* dock = new QDockWidget(this);
    dock->setObjectName("variables-dock");
    dock->setWidget(m_variables);
    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea, dock);
  }

  {
    m_callstack = new CallstackView(*m_controller);

    auto* dock = new QDockWidget(this);
    dock->setObjectName("callstack-dock");
    dock->setWidget(m_callstack);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, dock);
  }

  {
    m_breakpoints = new BreakpointsView(*m_controller);
    auto* dock = new QDockWidget(this);
    dock->setObjectName("breakpoints-dock");
    dock->setWidget(m_breakpoints);
    addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, dock);
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
}

void MainWindow::onSocketConnected()
{
  statusBar()->showMessage("Connected!", 1500);
}

void MainWindow::onDebuggerStateChanged()
{
  int s = m_controller->debuggerState();

  if(s == Controller::DebuggerState::Running)
    statusBar()->showMessage("Running!", 1000);
  else if (s == Controller::DebuggerState::Paused)
    statusBar()->showMessage("Pause...", 1000);
  else if (s == Controller::DebuggerState::Finished)
    statusBar()->showMessage("Done!");

  m_debug_menu->setEnabled(s != Controller::DebuggerState::Finished);

  if (s == Controller::DebuggerState::Running)
  {
    m_pause->setEnabled(true);
    m_run->setEnabled(false);
    m_step_into->setEnabled(false);
    m_step_over->setEnabled(false);
    m_step_out->setEnabled(false);

    m_callstack->setEnabled(false);
    m_variables->setEnabled(false);
  }
  else if (s == Controller::DebuggerState::Paused)
  {
    m_pause->setEnabled(false);
    m_run->setEnabled(true);
    m_step_into->setEnabled(true);
    m_step_over->setEnabled(true);
    m_step_out->setEnabled(true);

    // @TODO: Not the best place to do that, but that will do... for now!
    m_callstack->setEnabled(true);
    m_variables->setEnabled(true);
  }

  updateMarkers();
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
    setSourceCode(src);

    updateMarkers();
  }
  else
  {
    connect(m_controller, &Controller::sourceCodeReceived, this, &MainWindow::setSourceCode);
    m_controller->client().getSource(callstack_top.path);
  }
}

void MainWindow::onBreakpointsUpdated()
{
  updateMarkers();
}

void MainWindow::setSourceCode(std::shared_ptr<gonk::debugger::SourceCode> src)
{
  if (sender() != nullptr)
    disconnect(m_controller, &Controller::sourceCodeReceived, this, &MainWindow::setSourceCode);

  if (m_source_path == src->path)
    return;

  typewriter::TextCursor cursor{ m_editor->document() };
  cursor.setPosition(typewriter::Position{ m_editor->document()->lineCount() + 1, 0 }, typewriter::TextCursor::KeepAnchor);
  cursor.removeSelectedText();
  cursor.insertText(src->source);

  GonkSyntaxHighlighter highlighter{ m_editor->view() };
  highlighter.highlight(src);

  m_source_path = src->path;

  updateMarkers();
}

void MainWindow::onGutterLineClicked(int line)
{
  if (m_controller->hasBreakpoint(m_source_path, line))
  {
    m_controller->client().removeBreakpoint(m_source_path, line);
  }
  else
  {
    m_controller->client().addBreakpoint(m_source_path, line);
  }

  m_controller->client().getBreakpoints();
}

void MainWindow::updateMarkers()
{
  m_editor->gutter()->clearMarkers();

  auto callstack = m_controller->lastCallstackMessage();

  if (callstack != nullptr)
  {
    const auto& callstack_top = callstack->entries.back();
    std::shared_ptr<gonk::debugger::SourceCode> src = m_controller->getSource(callstack_top.path);

    m_editor->gutter()->addMarker(callstack_top.line, typewriter::MarkerType::Breakposition);
  }

  auto breakpoints = m_controller->lastBreakpointListMessage();

  if (breakpoints)
  {
    for (const gonk::debugger::BreakpointData& bp : breakpoints->list)
    {
      if (bp.script_path == m_source_path)
      {
        m_editor->gutter()->addMarker(bp.line, typewriter::MarkerType::Breakpoint);
      }
    }
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


  //{

  //  typewriter::SyntaxHighlighter highlighter{ m_editor->view() };
  //  highlighter.setFormat(1, 0, 4, 1);
  //  highlighter.setFormat(3, 8, 2, 2);
  //  highlighter.setFormat(3, 2, 5, 3);

  //  /*
  //  const char* source =
  //  "\n"
  //  "void main()\n"
  //  "{\n"
  //  "  print(66);\n"
  //  "}\n"
  //  "\n";
  //  */
  //}

}

void MainWindow::closeEvent(QCloseEvent *e)
{
  m_settings->setValue("geometry", saveGeometry());
  m_settings->setValue("windowState", saveState());
  QMainWindow::closeEvent(e);
}
