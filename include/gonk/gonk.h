// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_GONK_H
#define GONK_GONK_H

#include "gonk/gonk-defs.h"

#include <script/classtemplate.h>
#include <script/engine.h>

class GONK_API Gonk
{
public:
  Gonk(int & argc, char **argv);
  ~Gonk();

  int argc() const;
  char** argv() const;
  std::string argv(size_t index) const;

  int exec();

  static Gonk& Instance();

  script::Engine * scriptEngine() { return &m_engine; }

protected:
  int interactiveSession();
  void eval(std::string cmd);
  int runScript();
  int invokeMain(const script::Script& s);
  
protected:
  void display(const script::Value & val);

private:
  static Gonk* m_instance;
  int m_argc;
  char** m_argv;
  script::Engine m_engine;
};

#endif // GONK_GONK_H
