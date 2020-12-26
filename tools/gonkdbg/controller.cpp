// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "controller.h"

#include <QDebug>

Controller::Controller(QObject* parent)
  : QObject(parent)
{
  m_client = new gonk::debugger::Client;
  m_client->setParent(this);

  connect(m_client, &gonk::debugger::Client::messageReceived, this, &Controller::onMessageReceived);
}

gonk::debugger::Client& Controller::client() const
{
  return *m_client;
}

std::shared_ptr<gonk::debugger::Callstack> Controller::lastCallstackMessage() const
{
  return m_last_callstack_message;
}

void Controller::onMessageReceived(std::shared_ptr<gonk::debugger::DebuggerMessage> mssg)
{
  if (!mssg)
    return;

  if (dynamic_cast<gonk::debugger::Callstack*>(mssg.get()))
  {
    m_last_callstack_message = std::static_pointer_cast<gonk::debugger::Callstack>(mssg);
    auto& callstack = *m_last_callstack_message;
    Q_EMIT callstackUpdated();
  }
}
