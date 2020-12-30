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

struct SourceCode : DebuggerMessage
{
  std::string path;
  std::string source;
  QJsonObject syntaxtree;
};

struct CallstackEntry
{
  std::string function;
  std::string path;
  int line;
};

struct Callstack : DebuggerMessage
{
  std::vector<CallstackEntry> entries;
};

struct BreakpointData
{
  int id;
  std::string function;
  std::string script_path;
  int line;
};

struct BreakpointList : DebuggerMessage
{
  std::vector<BreakpointData> list;
};

struct Variable
{
  int offset = -1;
  std::string type;
  std::string name;
  std::string value;
  std::vector<std::shared_ptr<Variable>> members;
};

struct VariableList : DebuggerMessage
{
  int callstack_depth = -1;
  std::vector<std::shared_ptr<Variable>> variables;
};

} // namespace debugger

} // namespace gonk

#endif // GONK_DEBUGGER_MESSAGE_H
