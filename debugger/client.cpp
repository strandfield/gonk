// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "client.h"

#include <json-toolkit/stringify.h>

#include <QHostAddress>

#include <QDebug>

namespace gonk
{

namespace debugger
{

DebuggerMessage::~DebuggerMessage()
{

}

Client::Client(QObject* parent)
  : QObject(parent)
{
  m_socket = new QTcpSocket(this);
  connect(m_socket, &QAbstractSocket::connected, this, &Client::onSocketConnected);
  connect(m_socket, &QAbstractSocket::disconnected, this, &Client::onSocketDisconnected);
  connect(m_socket, &QAbstractSocket::stateChanged, this, &Client::onSocketStateChanged);
}

Client::~Client()
{

}

QTcpSocket* Client::socket() const
{
  return m_socket;
}

void Client::connectToDebugger(int port)
{
  setState(State::Connecting);
  m_socket->connectToHost(QHostAddress::LocalHost, port);
}

Client::State Client::state() const
{
  return m_state;
}

void Client::setState(State s)
{
  if (m_state != s)
  {
    if (m_state == State::DebuggerFinished)
      return;

    int prev = m_state;
    m_state = s;

    if (s == State::Disconnected && prev == State::Connecting)
      Q_EMIT connectionFailed();
    else if ((s == State::DebuggerRunning || s == State::DebuggerPaused) && prev == State::Connecting)
      Q_EMIT connectionEstablished();
    else if (s == State::Disconnected && (prev == State::DebuggerRunning || prev == State::DebuggerPaused))
      Q_EMIT connectionLost();

    if (s == State::DebuggerRunning)
      Q_EMIT debuggerRunning();
    else if (s == State::DebuggerPaused)
      Q_EMIT debuggerPaused();
    else if (s == State::DebuggerFinished)
      Q_EMIT debuggerFinished();

    Q_EMIT stateChanged(s, prev);
  }
}

bool Client::isConnected() const
{
  return m_state == State::DebuggerPaused || m_state == State::DebuggerRunning;
}

bool Client::isConnecting() const
{
  return m_state == State::Connecting;
}

bool Client::isPaused() const
{
  return m_state == State::DebuggerPaused;
}

void Client::action(Action a)
{
  json::Object obj;

  switch (a)
  {
  case Action::Pause:
    obj["type"] = "pause";
    break;
  case Action::Run:
    obj["type"] = "run";
    break;
  case Action::StepInto:
    obj["type"] = "stepinto";
    break;
  case Action::StepOver:
    obj["type"] = "stepover";
    break;
  case Action::StepOut:
    obj["type"] = "stepout";
    break;
  }

  send(obj);
}

void Client::addBreakpoint(const std::string& script_path, int line)
{
  json::Object obj;
  obj["type"] = "addbreakpoint";
  obj["path"] = script_path;
  obj["line"] = line;
  send(obj);
}

void Client::removeBreakpoint(int id)
{
  json::Object obj;
  obj["type"] = "removebreakpoint";
  obj["id"] = id;
  send(obj);
}

void Client::removeBreakpoint(const std::string& script_path, int line)
{
  json::Object obj;
  obj["type"] = "removebreakpoint";
  obj["path"] = script_path;
  obj["line"] = line;
  obj["id"] = -1;
  send(obj);
}

void Client::getSource(const std::string& path)
{
  json::Object obj;
  obj["type"] = "getsource";
  obj["path"] = path;
  send(obj);
}

void Client::getBreakpoints()
{
  json::Object obj;
  obj["type"] = "getbreakpoints";
  send(obj);
}

void Client::getCallstack()
{
  json::Object obj;
  obj["type"] = "getcallstack";
  send(obj);
}

void Client::getVariables(int depth)
{
  json::Object obj;
  obj["type"] = "getvariables";
  obj["depth"] = depth;
  send(obj);
}

void Client::onSocketConnected()
{
  connect(m_socket, &QAbstractSocket::readyRead, this, &Client::onReadyRead);
  setState(State::DebuggerRunning);
}

void Client::onSocketDisconnected()
{
  setState(State::Disconnected);
}

void Client::onSocketStateChanged(QAbstractSocket::SocketState socketState)
{
  if (socketState == QAbstractSocket::UnconnectedState)
    setState(State::Disconnected);
}

void Client::onReadyRead()
{
  QByteArray bytes = m_socket->readAll();
  m_json_stream.write(bytes.constData());

  for (json::Object obj : m_json_stream.objects)
  {
    processMessage(obj);
  }

  m_json_stream.objects.clear();
}

static std::shared_ptr<debugger::Variable> deserializeVar(const json::Object& json)
{
  auto ret = std::make_shared<debugger::Variable>();

  ret->name = json["name"].toString();
  ret->offset = json["offset"].toInt();
  ret->type = json["type"].toString();
  ret->value = json["value"].toString();

  if (json["members"].isArray() && json["members"].toArray().length() > 0)
  {
    json::Array members = json["members"].toArray();

    for (int i(0); i < members.length(); ++i)
      ret->members.push_back(deserializeVar(members.at(i).toObject()));
  }

  return ret;
}

void Client::processMessage(json::Object message)
{
  std::string type = message["type"].toString();

  if (type == "run")
  {
    setState(State::DebuggerRunning);
  }
  else if (type == "break")
  {
    setState(State::DebuggerPaused);
  }
  else if (type == "goodbye")
  {
    setState(State::DebuggerFinished);
    m_socket->disconnectFromHost();
  }
  else if (type == "sourcecode")
  {
    auto mssg = std::make_shared<SourceCode>();
    mssg->source = message["text"].toString();
    mssg->path = message["path"].toString();
    mssg->syntaxtree = message["ast"].toObject();

    Q_EMIT messageReceived(mssg);
  }
  else if (type == "breakpoints")
  {
    auto mssg = std::make_shared<BreakpointList>();
  
    json::Array list = message["list"].toArray();

    for(int i(0); i < list.length(); ++i)
    {
      json::Object js = list.at(i).toObject();

      BreakpointData bp;
      bp.function = js["function"].toString();
      bp.id = js["id"].toInt();
      bp.line = js["line"].toInt();
      bp.script_path = js["path"].toString();

      mssg->list.push_back(bp);
    }

    Q_EMIT messageReceived(mssg);
  }
  else if (type == "callstack")
  {
    auto mssg = std::make_shared<Callstack>();

    json::Array list = message["stack"].toArray();

    for (int i(0); i < list.length(); ++i)
    {
      json::Object jsonentry = list.at(i).toObject();
      debugger::CallstackEntry entry;
      entry.function = jsonentry["function"].toString();
      entry.path = jsonentry["path"].toString();
      entry.line = jsonentry["line"].toInt();
      mssg->entries.push_back(entry);
    }

    Q_EMIT messageReceived(mssg);
  }
  else if (type == "variables")
  {
    auto mssg = std::make_shared<VariableList>();

    mssg->callstack_depth = message["depth"].toInt();

    json::Array list = message["variables"].toArray();

    for (int i(0); i < list.length(); ++i)
    {
      json::Object varjson = list.at(i).toObject();
      mssg->variables.push_back(deserializeVar(varjson));
    }

    Q_EMIT messageReceived(mssg);
  }
}

void Client::send(json::Object response)
{
  std::string bytes = json::stringify(response);
  size_t s = bytes.size();
  m_socket->write(bytes.c_str(), s);
}

} // namespace debugger

} // namespace gonk
