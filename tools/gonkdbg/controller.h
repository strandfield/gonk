// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONKDBG_CONTROLLER_H
#define GONKDBG_CONTROLLER_H

#include <QObject>

#include <debugger/client.h>

#include <map>

class Controller : public QObject
{
  Q_OBJECT

  Q_PROPERTY(int debuggerState READ debuggerState NOTIFY debuggerStateChanged)
public:
  explicit Controller(QObject* parent = nullptr);
  ~Controller() = default;

  gonk::debugger::Client& client() const;

  enum DebuggerState
  {
    Finished = 0,
    Running = 1,
    Paused = 2,
  };

  int debuggerState() const;
  bool debuggerPaused() const;

  bool hasSource(const std::string& path) const;
  std::shared_ptr<gonk::debugger::SourceCode> getSource(const std::string& path) const;

  std::shared_ptr<gonk::debugger::Callstack> lastCallstackMessage() const;
  std::shared_ptr<gonk::debugger::BreakpointList> lastBreakpointListMessage() const;
  std::shared_ptr<gonk::debugger::VariableList> lastVariablesMessage() const;

public Q_SLOTS:
  void pause();
  void run();
  void stepInto();
  void stepOver();
  void stepOut();

Q_SIGNALS:
  void debuggerStateChanged();
  void callstackUpdated();
  void breakpointsUpdated();
  void variablesUpdated();
  void sourceCodeReceived(std::shared_ptr<gonk::debugger::SourceCode> src);

protected Q_SLOTS:
  void onSocketConnected();
  void onDebuggerRunning();
  void onDebuggerPaused();
  void onDebuggerFinished();
  void onMessageReceived(std::shared_ptr<gonk::debugger::DebuggerMessage> mssg);

protected:
  void setDebuggerState(int s);

private:
  gonk::debugger::Client* m_client;
  int m_debugger_state = 0;
  std::shared_ptr<gonk::debugger::Callstack> m_last_callstack_message;
  std::shared_ptr<gonk::debugger::BreakpointList> m_last_breakpoints_message;
  std::shared_ptr<gonk::debugger::VariableList> m_last_variables_message;
  std::map<std::string, std::shared_ptr<gonk::debugger::SourceCode>> m_source_codes;
};

#endif // GONKDBG_CONTROLLER_H
