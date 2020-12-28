// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "client.h"

#include <QHostAddress>
#include <QTcpSocket>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

#include <QDebug>

namespace gonk
{

namespace debugger
{

DebuggerMessage::~DebuggerMessage()
{

}

Client::Client(int port)
{
  m_socket = new QTcpSocket(this);
  connect(m_socket, &QAbstractSocket::connected, this, &Client::onSocketConnected);
  m_socket->connectToHost(QHostAddress::LocalHost, port);
}

Client::~Client()
{

}

void Client::action(Action a)
{
  QJsonObject obj;

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
  QJsonObject obj;
  obj["type"] = "addbreakpoint";
  obj["path"] = QString::fromStdString(script_path);
  obj["line"] = line;
  send(obj);
}

void Client::removeBreakpoint(int id)
{
  QJsonObject obj;
  obj["type"] = "removebreakpoint";
  obj["id"] = id;
  send(obj);
}

void Client::removeBreakpoint(const std::string& script_path, int line)
{
  QJsonObject obj;
  obj["type"] = "removebreakpoint";
  obj["path"] = QString::fromStdString(script_path);
  obj["line"] = line;
  obj["id"] = -1;
  send(obj);
}

void Client::getSource(const std::string& path)
{
  QJsonObject obj;
  obj["type"] = "getsource";
  obj["path"] = QString::fromStdString(path);
  send(obj);
}

void Client::getBreakpoints()
{
  QJsonObject obj;
  obj["type"] = "getbreakpoints";
  send(obj);
}

void Client::getCallstack()
{
  QJsonObject obj;
  obj["type"] = "getcallstack";
  send(obj);
}

void Client::getVariables(int depth)
{
  QJsonObject obj;
  obj["type"] = "getvariables";
  obj["depth"] = depth;
  send(obj);
}

void Client::onSocketConnected()
{
  connect(m_socket, &QAbstractSocket::readyRead, this, &Client::onReadyRead);
  Q_EMIT connectionEstablished();
}

void Client::onReadyRead()
{
  while (m_reader(m_socket))
  {
    QByteArray data = m_reader.read();
    QJsonObject message = QJsonDocument::fromJson(data).object();
    processMessage(message);
  }
}

void Client::processMessage(QJsonObject message)
{
  QString type = message.value("type").toString();

  if (type == "run")
  {
    Q_EMIT debuggerRunning();
  }
  else if (type == "break")
  {
    Q_EMIT debuggerPaused();
  }
  else if (type == "goodbye")
  {
    Q_EMIT debuggerFinished();
  }
  else if (type == "sourcecode")
  {
    auto mssg = std::make_shared<SourceCode>();
    mssg->source = message.value("text").toString().toStdString();
    mssg->path = message.value("path").toString().toStdString();
    mssg->syntaxtree = message.value("ast").toObject();

    Q_EMIT messageReceived(mssg);
  }
  else if (type == "breakpoints")
  {
    auto mssg = std::make_shared<BreakpointList>();
  
    QJsonArray list = message.value("list").toArray();

    for(int i(0); i < list.size(); ++i)
    {
      QJsonObject js = list.at(i).toObject();

      BreakpointData bp;
      bp.function = js.value("function").toString().toStdString();
      bp.id = js.value("id").toInt();
      bp.line = js.value("line").toInt();
      bp.script_path = js.value("path").toString().toStdString();

      mssg->list.push_back(bp);
    }

    Q_EMIT messageReceived(mssg);
  }
  else if (type == "callstack")
  {
    auto mssg = std::make_shared<Callstack>();

    QJsonArray list = message.value("stack").toArray();

    for (int i(0); i < list.size(); ++i)
    {
      QJsonObject jsonentry = list.at(i).toObject();
      debugger::CallstackEntry entry;
      entry.function = jsonentry.value("function").toString().toStdString();
      entry.path = jsonentry.value("path").toString().toStdString();
      entry.line = jsonentry.value("line").toInt();
      mssg->entries.push_back(entry);
    }

    Q_EMIT messageReceived(mssg);
  }
  else if (type == "variables")
  {
    auto mssg = std::make_shared<VariableList>();

    mssg->callstack_depth = message.value("depth").toInt();

    QJsonArray list = message.value("variables").toArray();

    for (int i(0); i < list.size(); ++i)
    {
      QJsonObject varjson = list.at(i).toObject();
      Variable var;
      var.name = varjson.value("name").toString().toStdString();
      var.offset = varjson.value("offset").toInt();
      var.type = varjson.value("type").toString().toStdString();
      var.value = varjson.value("value");

      mssg->variables.push_back(var);
    }

    Q_EMIT messageReceived(mssg);
  }
}

void Client::send(QJsonObject response)
{
  QByteArray bytes = QJsonDocument(response).toJson(QJsonDocument::Compact);
  size_t s = bytes.size();
  m_socket->write(reinterpret_cast<const char*>(&s), sizeof(size_t));
  m_socket->write(bytes);
}

} // namespace debugger

} // namespace gonk
