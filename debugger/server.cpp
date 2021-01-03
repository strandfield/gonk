// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "server.h"

#include <QEventLoop>

#include <QJsonArray>
#include <QJsonDocument>

#include <QTcpServer>
#include <QTcpSocket>

namespace gonk
{

namespace debugger
{

Server::Server()
{
  m_server = new QTcpServer(this);
  m_server->listen(QHostAddress::LocalHost, 24242);

  connect(m_server, &QTcpServer::newConnection, this, &Server::onNewConnection);
}

Server::~Server()
{

}

void Server::waitForConnection()
{
  qDebug() << "waiting for connection";

  if (m_socket)
    return;

  QEventLoop ev;
  QObject::connect(this, &Server::connectionEstablished, &ev, &QEventLoop::quit);
  ev.exec();
}

void Server::notifyRun()
{
  QJsonObject resp;
  resp["type"] = "run";
  send(resp);
}

void Server::notifyBreak()
{
  QJsonObject resp;
  resp["type"] = "break";
  send(resp);
}

void Server::notifyGoodbye()
{
  QJsonObject resp;
  resp["type"] = "goodbye";
  send(resp);
  m_socket->waitForBytesWritten(10);
}

bool Server::hasPendingRequests() const
{
  return !m_requests.empty();
}

std::vector<Request>& Server::pendingRequests()
{
  return m_requests;
}

bool Server::receiveRequest()
{
  size_t reqcount = m_requests.size();
  read();
  return m_requests.size() != reqcount;
}

bool Server::waitForRequest(int msecs)
{
  if (!m_socket)
    return false;

  if (!m_socket->waitForReadyRead(msecs))
    return false;

  return receiveRequest();
}

void Server::onNewConnection()
{
  m_socket = m_server->nextPendingConnection();

  if (m_socket)
  {
    m_server->deleteLater();
    m_server = nullptr;

    qDebug() << "connection established";

    Q_EMIT connectionEstablished();
  }
}

void Server::read()
{
  while (m_reader(m_socket))
  {
    QByteArray data = m_reader.read();
    parseRequest(data);
  }
}

void Server::parseRequest(QByteArray reqdata)
{
  QJsonObject reqjson = QJsonDocument::fromJson(reqdata).object();
  m_requests.push_back(parseRequest(reqjson));
}

Request Server::parseRequest(QJsonObject reqjson)
{
  QString reqtype = reqjson.value("type").toString();

  if (reqtype == "pause")
  {
    return Request::make<RequestType::Pause>();
  }
  else if (reqtype == "run")
  {
    return Request::make<RequestType::Run>();
  }
  else if (reqtype == "stepinto")
  {
    return Request::make<RequestType::StepInto>();
  }
  else if (reqtype == "stepover")
  {
    return Request::make<RequestType::StepOver>();
  }
  else if (reqtype == "stepout")
  {
    return Request::make<RequestType::StepOut>();
  }
  else if (reqtype == "getsource")
  {
    GetSourceCode data;
    data.path = reqjson["path"].toString().toStdString();
    return Request(data);
  }
  else if (reqtype == "getbreakpoints")
  {
    return Request::make<RequestType::GetBreakpointList>();
  }
  else if (reqtype == "getcallstack")
  {
    return Request::make<RequestType::GetCallStack>();
  }
  else if (reqtype == "getvariables")
  {
    GetVariables data;
    data.depth = reqjson["depth"].toInt();
    return Request(data);
  }
  else if (reqtype == "addbreakpoint")
  {
    AddBreakpoint data;
    data.script_path = reqjson["path"].toString().toStdString();
    data.line = reqjson["line"].toInt();
    return Request(data);
  }
  else if (reqtype == "removebreakpoint")
  {
    RemoveBreakpoint data;
    data.id = reqjson["id"].toInt();

    if (data.id == -1)
    {
      data.script_path = reqjson["path"].toString().toStdString();
      data.line = reqjson["line"].toInt();
    }

    return Request(data);
  }

  return Request::make<RequestType::Run>();
}

QJsonObject Server::serialize(const SourceCode& src)
{
  QJsonObject obj;
  obj["type"] = "sourcecode";
  obj["path"] = QString::fromStdString(src.path);
  obj["text"] = QString::fromStdString(src.source);
  obj["ast"] = src.syntaxtree;
  return obj;
}

QJsonObject Server::serialize(const BreakpointList& list)
{
  QJsonObject obj;
  obj["type"] = "breakpoints";
  
  {
    QJsonArray jsonlist;

    for (const BreakpointData& bpd : list.list)
    {
      QJsonObject jsonbp;
      jsonbp["id"] = bpd.id;
      jsonbp["line"] = bpd.line;
      jsonbp["function"] = QString::fromStdString(bpd.function);
      jsonbp["path"] = QString::fromStdString(bpd.script_path);
      jsonlist.push_back(jsonbp);
    }

    obj["list"] = jsonlist;
  }

  return obj;
}

QJsonObject Server::serialize(const Callstack& cs)
{
  QJsonObject obj;
  obj["type"] = "callstack";

  {
    QJsonArray stack;

    for (const auto& e : cs.entries)
    {
      QJsonObject entry;
      entry["function"] = QString::fromStdString(e.function);
      entry["path"] = QString::fromStdString(e.path);
      entry["line"] = e.line;

      stack.append(entry);
    }

    obj["stack"] = stack;
  }

  return obj;
}

QJsonObject Server::serialize(const Variable& v)
{
  QJsonObject ret;
  ret["offset"] = v.offset;
  ret["type"] = QString::fromStdString(v.type);
  ret["name"] = QString::fromStdString(v.name);
  ret["value"] = QString::fromStdString(v.value);

  if (!v.members.empty())
  {
    QJsonArray members;
    for (std::shared_ptr<Variable> memvar : v.members)
      members.append(serialize(*memvar));
    ret["members"] = members;
  }

  return ret;
}

QJsonObject Server::serialize(const VariableList& vlist)
{
  QJsonObject obj;
  obj["type"] = "variables";
  obj["depth"] = vlist.callstack_depth;

  {
    QJsonArray vars;

    for (const auto& v : vlist.variables)
    {
      QJsonObject e = serialize(*v);
      vars.push_back(e);
    }

    obj["variables"] = vars;
  }

  return obj;
}

void Server::send(QJsonObject response)
{
  if (m_socket)
  {
    QByteArray bytes = QJsonDocument(response).toJson(QJsonDocument::Compact);
    size_t s = bytes.size();
    m_socket->write(reinterpret_cast<const char*>(&s), sizeof(size_t));
    m_socket->write(bytes);
  }
}

} // namespace debugger

} // namespace gonk
