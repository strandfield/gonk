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

void Client::addBreakpoint(int line)
{
  QJsonObject obj;
  obj["type"] = "addbreakpoint";
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

void Client::getSource()
{
  QJsonObject obj;
  obj["type"] = "getsource";
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
    Q_EMIT sourceCodeReceived(message.value("text").toString());
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
      mssg->functions.push_back(list.at(i).toString().toStdString());
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
