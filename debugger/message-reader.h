// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_DEBUGGER_MESSAGEREADER_H
#define GONK_DEBUGGER_MESSAGEREADER_H

#include <QByteArray>
#include <QTcpSocket>

namespace gonk
{

namespace debugger
{

namespace priv
{

struct MessageReader
{
  QByteArray message;
  size_t current_read_size = 0;

  QByteArray read()
  {
    QByteArray result = message;
    message.clear();
    current_read_size = 0;
    return result;
  }

  bool operator()(QTcpSocket* socket)
  {
    if (current_read_size != 0)
    {
      QByteArray data = socket->read(current_read_size - message.size());
      message.append(data);
      return current_read_size == message.size();
    }
    else
    {
      if (socket->bytesAvailable() >= sizeof(size_t))
      {
        QByteArray len = socket->read(sizeof(size_t));
        current_read_size = *reinterpret_cast<const size_t*>(len.data());
        return (*this)(socket);
      }
      else
      {
        return false;
      }
    }
  }
};

} // namespace priv

} // namespace debugger

} // namespace gonk

#endif // GONK_DEBUGGER_MESSAGEREADER_H
