// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "breakpoint-handler.h"

#include "ast-producer.h"
#include "value-serializer.h"

#include <debugger/server.h>

#include <script/interpreter/executioncontext.h>
#include <script/interpreter/workspace.h>
#include <script/program/statements.h>
#include <script/engine.h>
#include <script/script.h>

namespace gonk
{

GonkDebugHandler::GonkDebugHandler(debugger::Server& serv, State s)
  : comm(serv),
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
  m_sp = static_cast<int>(m_call->stackOffset());

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
    sendSource(req.data<debugger::GetSourceCode>().path);
    break;
  case debugger::RequestType::GetBreakpointList:
    sendBreakpointList();
    break;
  case debugger::RequestType::GetCallStack:
    sendCallstack();
    break;
  case debugger::RequestType::GetVariables:
    sendVariables(req.data<debugger::GetVariables>().depth);
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

void GonkDebugHandler::sendSource(const std::string& path)
{
  for (auto s : m_call->engine()->scripts())
  {
    if (s.path() == path)
    {
      debugger::SourceCode src;

      src.path = path;
      src.source = m_call->callee().script().source().content();

      GonkAstProducer astproducer;
      src.syntaxtree = astproducer.produce(m_call->callee().script().ast());

      comm.reply(src);

      return;
    }
  }

  debugger::SourceCode src;

  src.path = path;
  src.source = "could not find source code of " + path;

  comm.reply(src);
}

void GonkDebugHandler::sendBreakpointList()
{
  debugger::BreakpointList list;

  for (const auto& entry : m_breakpoints)
  {
    debugger::BreakpointData bp;
    bp.function = entry.second.front().first.name();

    script::Script s = entry.second.front().first.script();

    if (!s.isNull())
      bp.script_path = s.path();

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

    debugger::CallstackEntry entry;
    entry.function = f.engine()->toString(f);
    entry.path = f.script().isNull() ? std::string() : f.script().path();
    entry.line = cs[i]->last_breakpoint ? cs[i]->last_breakpoint->line : 1;

    result.entries.push_back(entry);
  }

  comm.reply(result);
}

void GonkDebugHandler::sendVariables(int d)
{
  script::interpreter::Callstack& cs = m_call->executionContext()->callstack;
  int cs_size = static_cast<int>(cs.size());

  d = (d < 0 || d >= cs_size) ? (cs_size-1) : d;

  script::interpreter::FunctionCall* fc = cs[d];
  script::Engine* e = fc->engine();
  script::interpreter::Workspace w{ fc };
  GonkValueSerializer serializer{ *e };

  debugger::VariableList result;
  result.callstack_depth = d;

  for (size_t i(0); i < w.size(); ++i)
  {
    debugger::Variable v;
    v.offset = static_cast<int>(w.stackOffsetAt(i));
    v.type = e->toString(w.varTypeAt(i));
    v.name = w.nameAt(i);
    v.value = serializer.serialize(w.valueAt(i));
    result.variables.push_back(v);
  }

  comm.reply(result);
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
