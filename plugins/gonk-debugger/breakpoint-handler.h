// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk-debugger-defs.h"

#include <script/interpreter/debug-handler.h>
#include <script/function.h>

#include <map>
#include <memory>
#include <vector>

namespace gonk
{

namespace debugger
{
class Server;
struct Request;
} // namespace debugger

class GonkDebugHandler : public script::interpreter::DebugHandler
{
public:

  enum State
  {
    Running = 0,
    StepInto = 1,
    StepOver = 2,
    StepOut = 3,
    Break,
  };

  GonkDebugHandler(debugger::Server& handler, State s = State::Running);
  ~GonkDebugHandler();

  void interrupt(script::interpreter::FunctionCall& call, script::program::Breakpoint& info) override;

protected:
  bool shouldBreak(script::interpreter::FunctionCall& call, script::program::Breakpoint& info);
  void doBreak();
  void process(debugger::Request& req);
  void sendSource();
  void sendBreakpointList();
  void sendCallstack();
  void sendVariables();
  void addBreakpoint(int line);
  void removeBreakpoint(int id);

private:
  State m_state;
  int m_sp = -1;
  debugger::Server& comm;
  script::interpreter::FunctionCall* m_call = nullptr;
  script::program::Breakpoint* m_breakpoint = nullptr;
  int m_breakpoint_counter = 0;
  using BreakpointEntry = std::pair<script::Function, std::shared_ptr<script::program::Breakpoint>>;
  std::map<int, std::vector<BreakpointEntry>> m_breakpoints;
};

} // namespace gonk