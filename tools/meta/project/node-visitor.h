// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_NODEVISITOR_H
#define METAGONK_NODEVISITOR_H

#include "module.h"
#include "file.h"
#include "class.h"
#include "enum.h"
#include "function.h"
#include "statement.h"

class NodeVisitor
{
public:

  virtual void visit(Class& c) = 0;
  virtual void visit(Module& m) = 0;
  virtual void visit(Enum& e) = 0;
  virtual void visit(Enumerator& e) = 0;
  virtual void visit(File& f) = 0;
  virtual void visit(Function& f) = 0;
  virtual void visit(Namespace& n) = 0;
  virtual void visit(Statement& s) = 0;
};

#endif // METAGONK_NODEVISITOR_H
