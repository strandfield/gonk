// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_PRETTYPRINT_H
#define GONK_PRETTYPRINT_H

#include "gonk/gonk-defs.h"

#include <script/function.h>
#include <script/value.h>

#include <unordered_map>

namespace gonk
{

class GONK_API PrettyPrinter
{
public:
  explicit PrettyPrinter(script::Engine& e);
 
  void print(const script::Value& val);

protected:
  script::Function reprFunction(const script::Type& t);
  script::Function findReprFunction(const script::Type& t) const;

private:
  script::Engine& m_engine;
  std::unordered_map<int, script::Function> m_repr_functions;
};

} // namespace gonk

#endif // GONK_PRETTYPRINT_H
