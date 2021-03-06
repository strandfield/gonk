// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk-debugger.h"

#include "breakpoint-handler.h"
#include "server.h"

#include <gonk/gonk.h>

#include <script/interpreter/interpreter.h>

#include <script/engine.h>
#include <script/namespace.h>
#include <script/typesystem.h>

#include <iostream>

namespace gonk
{

} // namespace gonk

class GonkDebuggerPlugin : public gonk::Plugin
{
public:

  std::unique_ptr<gonk::debugger::Server> comm;
  std::shared_ptr<gonk::GonkDebugHandler> debug_handler;

  void load(script::Module m) override
  {
    script::Engine* e = m.engine();

    std::cout << "loading debugger" << std::endl;

    comm.reset(new gonk::debugger::Server);
    comm->waitForConnection();

    debug_handler = std::make_shared<gonk::GonkDebugHandler>(*comm, gonk::GonkDebugHandler::StepInto);
    e->interpreter()->setDebugHandler(debug_handler);

    std::cout << "debugger ready" << std::endl;
  }

  void unload(script::Module m) override
  {
    comm->notifyGoodbye();
    std::cout << "unloading debugger" << std::endl;
  }
};

gonk::Plugin* gonk_debugger_module()
{
  return new GonkDebuggerPlugin();
}
