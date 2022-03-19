// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <QObject>

#include "message.h"
#include "json-stream-parser.h"

#include <boost/asio.hpp>

#include <memory>
#include <variant>
#include <vector>

class QTcpSocket;
class QTcpServer;

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

  void start()
  {
    //message_ = make_daytime_string();

    boost::asio::async_write(socket_, boost::asio::buffer(message_),
      [this](const boost::system::error_code& error, size_t bytes_transferred) {
        handle_write(error, bytes_transferred);
      });
  }

private:
  TcpConnection(boost::asio::io_context& io_context)
    : socket_(io_context)
  {
  }

  void handle_write(const boost::system::error_code& /*error*/,
    size_t /*bytes_transferred*/)
  {
  }

  tcp::socket socket_;
  std::string message_;
};

class tcp_server
{
public:

  using tcp = boost::asio::ip::tcp;

  tcp_server(boost::asio::io_context& io_context)
    : io_context_(io_context),
    acceptor_(io_context, tcp::endpoint(tcp::v4(), 24242))
  {
    start_accept();
  }

private:
  void start_accept()
  {
    TcpConnection::pointer new_connection =
      TcpConnection::create(io_context_);

    acceptor_.async_accept(new_connection->socket(),
      [this, new_connection](const boost::system::error_code& error) {
        handle_accept(new_connection, error);
      });
  }

  void handle_accept(TcpConnection::pointer new_connection,
    const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
    }

    start_accept();
  }

  boost::asio::io_context& io_context_;
  tcp::acceptor acceptor_;
};


class Server : public QObject
{
  Q_OBJECT
public:
  Server();
  ~Server();

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

Q_SIGNALS:
  void connectionEstablished();

protected Q_SLOTS:
  void onNewConnection();

protected:
  void read();
  Request parseRequest(json::Object reqjson);

  json::Object serialize(const SourceCode& src);
  json::Object serialize(const BreakpointList& list);
  json::Object serialize(const Callstack& cs);
  json::Object serialize(const Variable& v);
  json::Object serialize(const VariableList& vlist);
  void send(json::Object response);

private:
  boost::asio::io_context m_io_context;
  QTcpServer* m_server = nullptr;
  QTcpSocket* m_socket = nullptr;
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