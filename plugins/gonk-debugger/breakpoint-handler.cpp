// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "breakpoint-handler.h"

#include <debugger/server.h>

#include <script/interpreter/executioncontext.h>
#include <script/program/statements.h>
#include <script/engine.h>
#include <script/script.h>

namespace gonk
{

GonkDebugHandler::GonkDebugHandler(debugger::Server& handler, State s)
  : comm(handler),
    m_state(s)
{

}

GonkDebugHandler::~GonkDebugHandler()
{

}

void GonkDebugHandler::interrupt(script::interpreter::FunctionCall& call, script::program::Breakpoint& info)
{
  m_call = &call;
  m_breakpoint = &info;

  if (shouldBreak(call, info))
  {
    doBreak();
  }
  else if (comm.receiveRequest())
  {
    std::vector<debugger::Request> reqs{ std::move(comm.pendingRequests()) };

    for (auto& r : reqs)
      process(r);
  }
}

bool GonkDebugHandler::shouldBreak(script::interpreter::FunctionCall& call, script::program::Breakpoint& info)
{
  return (m_state == State::StepInto) ||
    (info.status == 1) ||
    (m_state == State::StepOver && call.stackOffset() == m_sp) ||
    (m_state == State::StepOut && call.stackOffset() < m_sp);
}

void GonkDebugHandler::doBreak()
{
  m_state = State::Break;
  m_sp = m_call->stackOffset();

  comm.notifyBreak();

  for (;;)
  {
    while (!comm.hasPendingRequests())
      comm.waitForRequest();

    std::vector<debugger::Request> reqs{ std::move(comm.pendingRequests()) };

    for (auto& r : reqs)
      process(r);

    if (m_state != State::Break)
      break;
  }

  comm.notifyRun();
}

void GonkDebugHandler::process(debugger::Request& req)
{
  switch (req.type())
  {
  case debugger::RequestType::Run:
    m_state = State::Running;
    break;
  case debugger::RequestType::Pause:
  case debugger::RequestType::StepInto:
    m_state = State::StepInto;
    break;
  case debugger::RequestType::StepOver:
    m_state = State::StepOver;
    break;
  case debugger::RequestType::StepOut:
    m_state = State::StepOut;
    break;
  case debugger::RequestType::GetSourceCode:
    sendSource();
    break;
  case debugger::RequestType::GetBreakpointList:
    sendBreakpointList();
    break;
  case debugger::RequestType::GetCallStack:
    sendCallstack();
    break;
  case debugger::RequestType::GetVariables:
    sendVariables();
    break;
  case debugger::RequestType::AddBreakpoint:
    addBreakpoint(req.data<debugger::AddBreakpoint>().line);
    break;
  case debugger::RequestType::RemoveBreakpoint:
    removeBreakpoint(req.data<debugger::RemoveBreakpoint>().id);
    break;
  default:
    break;
  }
}

void GonkDebugHandler::sendSource()
{
  debugger::SourceCode src;
  src.src = m_call->callee().script().source().content();
  comm.reply(src);
}

void GonkDebugHandler::sendBreakpointList()
{
  debugger::BreakpointList list;

  for (const auto& entry : m_breakpoints)
  {
    debugger::BreakpointData bp;
    bp.function = entry.second.front().first.name();
    bp.id = entry.first;
    bp.line = entry.second.front().second->line;
    list.list.push_back(bp);
  }

  comm.reply(list);
}

void GonkDebugHandler::sendCallstack()
{
  debugger::Callstack result;

  script::interpreter::Callstack& cs = m_call->executionContext()->callstack;

  for (size_t i(0); i < cs.size(); ++i)
  {
    script::Function f = cs[i]->callee();
    result.functions.push_back(f.engine()->toString(f));
  }

  comm.reply(result);
}

void GonkDebugHandler::sendVariables()
{

}

void GonkDebugHandler::addBreakpoint(int line)
{
  std::vector<BreakpointEntry> bps = m_call->callee().script().breakpoints(line);

  if (bps.empty())
    return;

  for (auto p : bps)
  {
    p.second->status = 1;
  }

  m_breakpoints[m_breakpoint_counter++] = bps;
}

void GonkDebugHandler::removeBreakpoint(int id)
{
  std::vector<BreakpointEntry>& list = m_breakpoints[id];

  for (auto p : list)
  {
    p.second->status = 0;
  }
}

} // namespace gonk
