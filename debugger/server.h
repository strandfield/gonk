// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "message.h"
#include "json-stream-parser.h"

#include <boost/asio.hpp>

#include <memory>
#include <variant>
#include <vector>

namespace gonk
{

namespace debugger
{

/* Requests */

enum class RequestType
{
  Run,
  Pause,
  StepInto,
  StepOver,
  StepOut,
  GetSourceCode,
  GetBreakpointList,
  GetCallStack,
  GetVariables,
  AddBreakpoint,
  RemoveBreakpoint
};

template<RequestType RT>
struct EmptyData { };

struct GetSourceCode
{
  std::string path;
};

struct GetVariables
{
  int depth;
};

struct AddBreakpoint
{
  std::string script_path;
  int line;
};

struct RemoveBreakpoint
{
  int id = -1;
  std::string script_path;
  int line = -1;
};

struct Request
{
  using Data = std::variant<
    EmptyData<RequestType::Run>,
    EmptyData<RequestType::Pause>,
    EmptyData<RequestType::StepInto>,
    EmptyData<RequestType::StepOver>,
    EmptyData<RequestType::StepOut>,
    GetSourceCode,
    EmptyData<RequestType::GetBreakpointList>,
    EmptyData<RequestType::GetCallStack>,
    GetVariables,
    AddBreakpoint,
    RemoveBreakpoint
  >;
  
  Data data_;

  Request(const Request&) = default;
  Request(Request&&) = default;

  Request(Data d)
    : data_(std::move(d))
  {

  }

  template<RequestType RT>
  static Request make()
  {
    return Request(EmptyData<RT>());
  }

  RequestType type() const { return static_cast<RequestType>(data_.index()); }

  template<typename T>
  T data()
  {
    return std::get<T>(data_);
  }
};

/* Server */

class TcpConnection : public std::enable_shared_from_this<TcpConnection>
{
public:
  typedef std::shared_ptr<TcpConnection> pointer;

  using tcp = boost::asio::ip::tcp;

  static pointer create(boost::asio::io_context& io_context)
  {
    return pointer(new TcpConnection(io_context));
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  std::string& buffer()
  {
    return buffer_;
  }

private:
  TcpConnection(boost::asio::io_context& io_context)
    : socket_(io_context)
  {
    buffer_.resize(2048);
  }

  tcp::socket socket_;
  std::string buffer_;
};

class Server
{
public:
  Server();
  ~Server();

  using tcp = boost::asio::ip::tcp;

  void waitForConnection();

  bool hasPendingRequests() const;
  std::vector<Request>& pendingRequests();
  bool receiveRequest();
  bool waitForRequest(int msecs = 100);

  void notifyRun();
  void notifyBreak();
  void notifyGoodbye();

  template<typename T>
  void reply(T response_data);

protected:
  Request parseRequest(json::Object reqjson);

  json::Object serialize(const SourceCode& src);
  json::Object serialize(const BreakpointList& list);
  json::Object serialize(const Callstack& cs);
  json::Object serialize(const Variable& v);
  json::Object serialize(const VariableList& vlist);
  void send(json::Object response);

private:
  void start_accept();
  void handle_accept(const boost::system::error_code& error);
  void start_read();
  void handle_read(const boost::system::error_code& error, size_t bytes_transferred);

private:
  boost::asio::io_context m_io_context;
  tcp::acceptor m_acceptor;
  TcpConnection::pointer m_connection;
  std::vector<Request> m_requests;
  JsonStreamParser m_json_stream;
};

template<typename T>
inline void Server::reply(T response_data)
{
  json::Object obj = serialize(response_data);
  send(obj);
}

} // namespace debugger

} // namespace gonk