// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "server.h"

#include <json-toolkit/stringify.h>

#include <iostream>

namespace gonk
{

namespace debugger
{

Server::Server()
  : m_acceptor(m_io_context, tcp::endpoint(tcp::v4(), 24242))
{
  start_accept();
}

Server::~Server()
{

}

void Server::waitForConnection()
{
  std::cout << "waiting for connection" << std::endl;
  m_io_context.run();
  m_io_context.restart();
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
  m_io_context.poll_one();
  return hasPendingRequests();
}

bool Server::waitForRequest(int msecs)
{
  if (!m_connection)
    return false;

  m_io_context.run_for(std::chrono::milliseconds(msecs));

  return hasPendingRequests();
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
  if (m_connection)
  {
    std::string bytes = json::stringify(response);
    boost::asio::write(m_connection->socket(), boost::asio::buffer(bytes));
  }
}

void Server::start_accept()
{
  m_connection = TcpConnection::create(m_io_context);

  m_acceptor.async_accept(m_connection->socket(),
    [this](const boost::system::error_code& error) {
      handle_accept(error);
    });
}

void Server::handle_accept(const boost::system::error_code& error)
{
  if (!error)
  {
    m_io_context.stop();
    start_read();
  }

  //start_accept();
}

void Server::start_read()
{
  m_connection->buffer().clear();
  m_connection->buffer().resize(2048);

  boost::asio::async_read(m_connection->socket(), boost::asio::buffer(m_connection->buffer()),
    boost::asio::transfer_at_least(1),
    [this](const boost::system::error_code& error, size_t bytes_transferred) {
      handle_read(error, bytes_transferred);
    });
}

void Server::handle_read(const boost::system::error_code& error, size_t bytes_transferred)
{
  std::string& buffer = m_connection->buffer();
  buffer.resize(bytes_transferred);
  m_json_stream.write(buffer);

  for (json::Object obj : m_json_stream.objects)
  {
    m_requests.push_back(parseRequest(obj));
  }

  m_json_stream.objects.clear();

  start_read();
}

} // namespace debugger

} // namespace gonk
