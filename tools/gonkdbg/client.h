// Copyright (C) 2020-2022 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_DEBUGGER_CLIENT_H
#define GONK_DEBUGGER_CLIENT_H

#include <QObject>

#include <debugger/json-stream-parser.h>
#include <debugger/message.h>

#include <json-toolkit/json.h>

#include <QTcpSocket>

namespace gonk
{

namespace debugger
{

class Client : public QObject
{
  Q_OBJECT
public:
  explicit Client(QObject* parent = nullptr);
  ~Client();

  QTcpSocket* socket() const;
  void connectToDebugger(int port = 24242);

  enum State
  {
    Disconnected = 0,
    Connecting = 1,
    DebuggerRunning = 2,
    DebuggerPaused = 3,
    DebuggerFinished = 4,
  };

  State state() const;
  bool isConnected() const;
  bool isConnecting() const;
  bool isPaused() const;

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
  void stateChanged(int cur, int prev);
  void connectionEstablished();
  void connectionFailed();
  void connectionLost();
  void debuggerRunning();
  void debuggerPaused();
  void debuggerFinished();
  void messageReceived(std::shared_ptr<DebuggerMessage> mssg);

protected Q_SLOTS:
  void onSocketConnected();
  void onSocketDisconnected();
  void onSocketStateChanged(QAbstractSocket::SocketState socketState);
  void onReadyRead();

protected:
  void setState(State s);

  void processMessage(json::Object message);

  void send(json::Object response);

private:
  QTcpSocket* m_socket = nullptr;
  State m_state = State::Disconnected;
  JsonStreamParser m_json_stream;
};

} // namespace debugger

} // namespace gonk

#endif // GONK_DEBUGGER_CLIENT_H
