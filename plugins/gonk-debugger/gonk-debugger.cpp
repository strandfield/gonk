// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk-debugger.h"

#include "breakpoint-handler.h"
#include "communication-handler.h"

#include <gonk/gonk.h>

#include <script/interpreter/interpreter.h>

#include <script/engine.h>
#include <script/namespace.h>
#include <script/typesystem.h>

#include <QDebug>

namespace gonk
{

} // namespace gonk

class GonkDebuggerPlugin : public gonk::Plugin
{
public:

  std::unique_ptr<gonk::debugger::CommunicationHandler> comm;
  std::shared_ptr<gonk::GonkDebugHandler> debug_handler;

  void load(script::Module m) override
  {
    script::Engine* e = m.engine();

    qDebug() << "loading debugger";

    // creates the QCoreApplication if it doesn't exist
    Gonk::Instance().qCoreApplication();

    comm.reset(new gonk::debugger::CommunicationHandler);
    comm->waitForConnection();

    debug_handler = std::make_shared<gonk::GonkDebugHandler>(*comm, gonk::GonkDebugHandler::StepInto);
    e->interpreter()->setDebugHandler(debug_handler);

    qDebug() << "debugger ready";
  }

  void unload(script::Module m) override
  {
    comm->notifyGoodbye();
    qDebug() << "unloading debugger";
  }
};

gonk::Plugin* gonk_debugger_module()
{
  return new GonkDebuggerPlugin();
}
