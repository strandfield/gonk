// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_PLUGIN_H
#define GONK_PLUGIN_H

#include "gonk/gonk-defs.h"

#include <script/module.h>

#include <dynlib/dynlib.h>

#include <memory>

namespace gonk
{

class GONK_API Plugin
{
public:
  std::shared_ptr<dynlib::Library> library;

public:
  Plugin();
  virtual ~Plugin();

  virtual void load(script::Module m) = 0;
  virtual void unload(script::Module m);

};

} // namespace gonk

#endif // GONK_PLUGIN_H
