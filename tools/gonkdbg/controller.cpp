// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "controller.h"

#include <QFile>
#include <QJsonDocument>

#include <QDebug>

Controller::Controller(QObject* parent)
  : QObject(parent)
{
  m_client = new gonk::debugger::Client;
  m_client->setParent(this);

  connect(&client(), &gonk::debugger::Client::connectionEstablished, this, &Controller::onSocketConnected);
}

gonk::debugger::Client& Controller::client() const
{
  return *m_client;
}

int Controller::debuggerState() const
{
  return m_debugger_state;
}

bool Controller::debuggerPaused() const
{
  return debuggerState() == DebuggerState::Paused;
}

int Controller::currentFrame() const
{
  return m_current_frame;
}

void Controller::setCurrentFrame(int n)
{
  if (m_current_frame != n)
  {
    m_current_frame = n;
    Q_EMIT currentFrameChanged(n);
  }
}

bool Controller::hasSource(const std::string& path) const
{
  return m_source_codes.find(path) != m_source_codes.end();
}

std::shared_ptr<gonk::debugger::SourceCode> Controller::getSource(const std::string& path) const
{
  auto it = m_source_codes.find(path);
  return it == m_source_codes.end() ? nullptr : it->second;
}

bool Controller::hasBreakpoint(const std::string& script_path, int line) const
{
  if (m_last_breakpoints_message == nullptr)
    return false;

  auto& bps = m_last_breakpoints_message->list;

  for (const auto& bp : bps)
  {
    if (bp.script_path == script_path && bp.line == line)
      return true;
  }

  return false;
}

std::shared_ptr<gonk::debugger::Callstack> Controller::lastCallstackMessage() const
{
  return m_last_callstack_message;
}

std::shared_ptr<gonk::debugger::BreakpointList> Controller::lastBreakpointListMessage() const
{
  return m_last_breakpoints_message;
}

std::shared_ptr<gonk::debugger::VariableList> Controller::lastVariablesMessage() const
{
  return m_last_variables_message;
}

const std::vector<std::shared_ptr<gonk::debugger::VariableList>>& Controller::variables() const
{
  return m_variables;
}

void Controller::pause()
{
  if (debuggerPaused())
  {
    client().action(gonk::debugger::Client::Action::Pause);
  }
}

void Controller::run()
{
  if (debuggerPaused())
  {
    client().action(gonk::debugger::Client::Action::Run);
  }
}

void Controller::stepInto()
{
  if (debuggerPaused())
  {
    client().action(gonk::debugger::Client::Action::StepInto);
  }
}

void Controller::stepOver()
{
  if (debuggerPaused())
  {
    client().action(gonk::debugger::Client::Action::StepOver);
  }
}

void Controller::stepOut()
{
  if (debuggerPaused())
  {
    client().action(gonk::debugger::Client::Action::StepOut);
  }
}

void Controller::onSocketConnected()
{
  connect(&client(), &gonk::debugger::Client::debuggerRunning, this, &Controller::onDebuggerRunning);
  connect(&client(), &gonk::debugger::Client::debuggerPaused, this, &Controller::onDebuggerPaused);
  connect(&client(), &gonk::debugger::Client::debuggerFinished, this, &Controller::onDebuggerFinished);
  connect(&client(), &gonk::debugger::Client::messageReceived, this, &Controller::onMessageReceived);
}

void Controller::onDebuggerRunning()
{
  setDebuggerState(DebuggerState::Running);
  setCurrentFrame(-1);
}

void Controller::onDebuggerPaused()
{
  setDebuggerState(DebuggerState::Paused);

  client().getCallstack();
  client().getBreakpoints();
  client().getVariables();
}

void Controller::onDebuggerFinished()
{
  setDebuggerState(DebuggerState::Finished);
}

void Controller::onMessageReceived(std::shared_ptr<gonk::debugger::DebuggerMessage> mssg)
{
  if (!mssg)
    return;

  if (dynamic_cast<gonk::debugger::Callstack*>(mssg.get()))
  {
    m_last_callstack_message = std::static_pointer_cast<gonk::debugger::Callstack>(mssg);

    m_variables.clear();
    m_variables.resize(m_last_callstack_message->entries.size(), nullptr);

    setCurrentFrame(static_cast<int>(m_last_callstack_message->entries.size()) - 1);

    Q_EMIT callstackUpdated();
  }
  else if(dynamic_cast<gonk::debugger::BreakpointList*>(mssg.get()))
  {
    m_last_breakpoints_message = std::static_pointer_cast<gonk::debugger::BreakpointList>(mssg);
    Q_EMIT breakpointsUpdated();
  }
  else if (dynamic_cast<gonk::debugger::VariableList*>(mssg.get()))
  {
    m_last_variables_message = std::static_pointer_cast<gonk::debugger::VariableList>(mssg);
    m_variables[m_last_variables_message->callstack_depth] = m_last_variables_message;
    Q_EMIT variablesUpdated();
  }
  else if (dynamic_cast<gonk::debugger::SourceCode*>(mssg.get()))
  {
    auto src = std::static_pointer_cast<gonk::debugger::SourceCode>(mssg);
    m_source_codes[src->path] = src;

    {
      QFile stfile{ "syntax.json" };
      stfile.open(QIODevice::WriteOnly | QIODevice::Truncate);
      stfile.write(QJsonDocument(src->syntaxtree).toJson());
      stfile.close();
    }

    Q_EMIT sourceCodeReceived(src);
  }
}

void Controller::setDebuggerState(int s)
{
  if (m_debugger_state != s)
  {
    m_debugger_state = s;
    Q_EMIT debuggerStateChanged();
  }
}

