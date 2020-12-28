// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_DEBUGGER_CLIENT_H
#define GONK_DEBUGGER_CLIENT_H

#include <QObject>

#include "message.h"
#include "message-reader.h"

class QTcpSocket;

namespace gonk
{

namespace debugger
{

class Client : public QObject
{
  Q_OBJECT
public:
  explicit Client(int port = 24242);
  ~Client();

  enum Action
  {
    Pause,
    Run,
    StepInto,
    StepOver,
    StepOut,
  };

  void action(Action a);

  void addBreakpoint(const std::string& script_path, int line);

  void removeBreakpoint(int id);
  void removeBreakpoint(const std::string& script_path, int line);

  void getSource(const std::string& path);

  void getBreakpoints();

  void getCallstack();

  void getVariables(int depth = -1);

Q_SIGNALS:
  void connectionEstablished();
  void debuggerRunning();
  void debuggerPaused();
  void debuggerFinished();
  void messageReceived(std::shared_ptr<DebuggerMessage> mssg);

protected Q_SLOTS:
  void onSocketConnected();
  void onReadyRead();

protected:
  void processMessage(QJsonObject message);

  void send(QJsonObject response);

private:
  QTcpSocket* m_socket = nullptr;
  priv::MessageReader m_reader;
};

} // namespace debugger

} // namespace gonk

#endif // GONK_DEBUGGER_CLIENT_H
