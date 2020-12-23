// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <QObject>

#include "message.h"

#include <QJsonObject>

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

struct AddBreakpoint
{
  int line;
};

struct RemoveBreakpoint
{
  int id;
};

struct Request
{
  std::variant<
    EmptyData<RequestType::Run>,
    EmptyData<RequestType::Pause>,
    EmptyData<RequestType::StepInto>,
    EmptyData<RequestType::StepOver>,
    EmptyData<RequestType::StepOut>,
    EmptyData<RequestType::GetSourceCode>,
    EmptyData<RequestType::GetBreakpointList>,
    EmptyData<RequestType::GetCallStack>,
    EmptyData<RequestType::GetVariables>,
    AddBreakpoint,
    RemoveBreakpoint
  > data_;

  template<typename T>
  Request(T d)
    : data_(d)
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

/* Responses */

struct SourceCode
{
  std::string src;
};

/* Server */

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
  void parseRequest(QByteArray reqdata);
  Request parseRequest(QJsonObject reqjson);

  QJsonObject serialize(const SourceCode& src);
  QJsonObject serialize(const BreakpointList& list);
  QJsonObject serialize(const Callstack& cs);
  void send(QJsonObject response);

private:
  QTcpServer* m_server = nullptr;
  QTcpSocket* m_socket = nullptr;
  std::vector<Request> m_requests;
  QByteArray m_current_read;
  size_t m_current_read_size = 0;
};

template<typename T>
inline void Server::reply(T response_data)
{
  QJsonObject obj = serialize(response_data);
  send(obj);
}

} // namespace debugger

} // namespace gonk