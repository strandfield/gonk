// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_DEBUGGER_CLIENT_H
#define GONK_DEBUGGER_CLIENT_H

#include <QObject>

class QTcpSocket;

namespace gonk
{

namespace debugger
{

namespace priv
{

struct MessageReader
{
  QByteArray message;
  size_t current_read_size = 0;

  QByteArray read();

  bool operator()(QTcpSocket* socket);
};

} // namespace priv

class DebuggerMessage
{
public:
  virtual ~DebuggerMessage();
};

struct Callstack : DebuggerMessage
{
  std::vector<std::string> functions;
};

struct BreakpointData
{
  int id;
  std::string function;
  int line;
};

struct BreakpointList : DebuggerMessage
{
  std::vector<BreakpointData> list;
};

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

  void addBreakpoint(int line);

  void removeBreakpoint(int id);

  void getSource();

  void getBreakpoints();

  void getCallstack();

Q_SIGNALS:
  void connectionEstablished();
  void debuggerRunning();
  void debuggerPaused();
  void debuggerFinished();
  void sourceCodeReceived(QString src);
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
