// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_DEBUGGER_MESSAGE_H
#define GONK_DEBUGGER_MESSAGE_H

#include <QObject>

#include <QJsonObject>

#include <variant>
#include <vector>

namespace gonk
{

namespace debugger
{

class DebuggerMessage
{
public:
  virtual ~DebuggerMessage();
};

struct Callstack : DebuggerMessage
{
  std::vector<std::string> functions;
};

struct BreakpointData
{
  int id;
  std::string function;
  int line;
};

struct BreakpointList : DebuggerMessage
{
  std::vector<BreakpointData> list;
};

struct Variable
{
  int offset;
  std::string type;
  std::string name;
  QJsonValue value;
};

struct VariableList : DebuggerMessage
{
  int callstack_depth = -1;
  std::vector<Variable> variables;
};

} // namespace debugger

} // namespace gonk

#endif // GONK_DEBUGGER_MESSAGE_H
