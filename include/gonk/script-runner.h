// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_SCRIPTRUNNER_H
#define GONK_SCRIPTRUNNER_H

#include "gonk/gonk-defs.h"

#include <script/compilemode.h>
#include <script/function.h>
#include <script/value.h>

#include <unordered_map>

class Gonk;

namespace gonk
{

class GONK_API ScriptRunner
{
public:
  explicit ScriptRunner(Gonk& gnk);
 
  int run();

protected:
  int runScript();
  script::Type findVectorStringType() const;
  bool validateMain(const script::Function& f) const;
  script::Function findMain(const script::Script& s) const;
  script::Function findPushBackFunction(const script::Type& t) const;
  int invokeMain(const script::Script& s);
  script::Value invokeMain(const script::Function& f);

private:
  Gonk& m_gonk;
  script::CompileMode m_mode = script::CompileMode::Release;
  size_t m_argv_offset = 1;
};

} // namespace gonk

#endif // GONK_SCRIPTRUNNER_H
