// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONKDBG_CONTROLLER_H
#define GONKDBG_CONTROLLER_H

#include <QObject>

#include "client.h"

#include <QProcess>

#include <map>
#include <optional>
#include <string>

class QProcess;

class GonkdbgCLI
{
public:
  GonkdbgCLI(int argc_, char** argv_);

public:
  int argc;
  char** argv;

public:
  std::optional<std::string> script;
  std::vector<std::string> extras;

public:
  bool empty() const;
};

class Controller : public QObject
{
  Q_OBJECT

  Q_PROPERTY(int currentFrame READ currentFrame NOTIFY currentFrameChanged)

public:
  explicit Controller(int& argc, char** argv, QObject* parent = nullptr);
  ~Controller() = default;

  const GonkdbgCLI& cli() const;
  gonk::debugger::Client& client() const;
  QProcess* process() const;

  int currentFrame() const;
  void setCurrentFrame(int n);

  bool hasSource(const std::string& path) const;
  std::shared_ptr<gonk::debugger::SourceCode> getSource(const std::string& path) const;

  bool hasBreakpoint(const std::string& script_path, int line) const;

  std::shared_ptr<gonk::debugger::Callstack> lastCallstackMessage() const;
  std::shared_ptr<gonk::debugger::BreakpointList> lastBreakpointListMessage() const;
  std::shared_ptr<gonk::debugger::VariableList> lastVariablesMessage() const;

  const std::vector<std::shared_ptr<gonk::debugger::VariableList>>& variables() const;

public Q_SLOTS:
  void pause();
  void run();
  void stepInto();
  void stepOver();
  void stepOut();

Q_SIGNALS:
  void processStarted();
  void processFinished();
  void currentFrameChanged(int n);
  void callstackUpdated();
  void breakpointsUpdated();
  void variablesUpdated();
  void sourceCodeReceived(std::shared_ptr<gonk::debugger::SourceCode> src);

protected Q_SLOTS:
  void onSocketConnected();
  void onDebuggerRunning();
  void onDebuggerPaused();
  void onMessageReceived(std::shared_ptr<gonk::debugger::DebuggerMessage> mssg);
  void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
  GonkdbgCLI m_cli;
  gonk::debugger::Client* m_client;
  QProcess* m_process = nullptr;
  int m_current_frame = -1;
  std::shared_ptr<gonk::debugger::Callstack> m_last_callstack_message;
  std::shared_ptr<gonk::debugger::BreakpointList> m_last_breakpoints_message;
  std::shared_ptr<gonk::debugger::VariableList> m_last_variables_message;
  std::vector<std::shared_ptr<gonk::debugger::VariableList>> m_variables;
  std::map<std::string, std::shared_ptr<gonk::debugger::SourceCode>> m_source_codes;
};

#endif // GONKDBG_CONTROLLER_H
