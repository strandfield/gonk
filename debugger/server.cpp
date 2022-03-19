// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "server.h"

#include <json-toolkit/stringify.h>

#include <QEventLoop>

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
  json::Object resp;
  resp["type"] = "run";
  send(resp);
}

void Server::notifyBreak()
{
  json::Object resp;
  resp["type"] = "break";
  send(resp);
}

void Server::notifyGoodbye()
{
  json::Object resp;
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
  QByteArray bytes = m_socket->readAll();
  m_json_stream.write(bytes.constData());

  for (json::Object obj : m_json_stream.objects)
  {
    m_requests.push_back(parseRequest(obj));
  }

  m_json_stream.objects.clear();
}

Request Server::parseRequest(json::Object reqjson)
{
  std::string reqtype = reqjson["type"].toString();

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
    data.path = reqjson["path"].toString();
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
    data.script_path = reqjson["path"].toString();
    data.line = reqjson["line"].toInt();
    return Request(data);
  }
  else if (reqtype == "removebreakpoint")
  {
    RemoveBreakpoint data;
    data.id = reqjson["id"].toInt();

    if (data.id == -1)
    {
      data.script_path = reqjson["path"].toString();
      data.line = reqjson["line"].toInt();
    }

    return Request(data);
  }

  return Request::make<RequestType::Run>();
}

json::Object Server::serialize(const SourceCode& src)
{
  json::Object obj;
  obj["type"] = "sourcecode";
  obj["path"] = src.path;
  obj["text"] = src.source;
  obj["ast"] = src.syntaxtree;
  return obj;
}

json::Object Server::serialize(const BreakpointList& list)
{
  json::Object obj;
  obj["type"] = "breakpoints";
  
  {
    json::Array jsonlist;

    for (const BreakpointData& bpd : list.list)
    {
      json::Object jsonbp;
      jsonbp["id"] = bpd.id;
      jsonbp["line"] = bpd.line;
      jsonbp["function"] = bpd.function;
      jsonbp["path"] = bpd.script_path;
      jsonlist.push(jsonbp);
    }

    obj["list"] = jsonlist;
  }

  return obj;
}

json::Object Server::serialize(const Callstack& cs)
{
  json::Object obj;
  obj["type"] = "callstack";

  {
    json::Array stack;

    for (const auto& e : cs.entries)
    {
      json::Object entry;
      entry["function"] = e.function;
      entry["path"] = e.path;
      entry["line"] = e.line;

      stack.push(entry);
    }

    obj["stack"] = stack;
  }

  return obj;
}

json::Object Server::serialize(const Variable& v)
{
  json::Object ret;
  ret["offset"] = v.offset;
  ret["type"] = v.type;
  ret["name"] = v.name;
  ret["value"] = v.value;

  if (!v.members.empty())
  {
    json::Array members;
    for (std::shared_ptr<Variable> memvar : v.members)
      members.push(serialize(*memvar));
    ret["members"] = members;
  }

  return ret;
}

json::Object Server::serialize(const VariableList& vlist)
{
  json::Object obj;
  obj["type"] = "variables";
  obj["depth"] = vlist.callstack_depth;

  {
    json::Array vars;

    for (const auto& v : vlist.variables)
    {
      json::Object e = serialize(*v);
      vars.push(e);
    }

    obj["variables"] = vars;
  }

  return obj;
}

void Server::send(json::Object response)
{
  if (m_socket)
  {
    std::string bytes = json::stringify(response);
    size_t s = bytes.size();
    m_socket->write(bytes.c_str(), s);
  }
}

} // namespace debugger

} // namespace gonk
