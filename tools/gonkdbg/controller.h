// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONKDBG_CONTROLLER_H
#define GONKDBG_CONTROLLER_H

#include <QObject>

#include <debugger/client.h>

class Controller : public QObject
{
  Q_OBJECT
public:
  explicit Controller(QObject* parent = nullptr);
  ~Controller() = default;

  gonk::debugger::Client& client() const;

  std::shared_ptr<gonk::debugger::Callstack> lastCallstackMessage() const;

Q_SIGNALS:
  void callstackUpdated();

protected Q_SLOTS:
  void onMessageReceived(std::shared_ptr<gonk::debugger::DebuggerMessage> mssg);

private:
  gonk::debugger::Client* m_client;
  std::shared_ptr<gonk::debugger::Callstack> m_last_callstack_message;
};

#endif // GONKDBG_CONTROLLER_H
