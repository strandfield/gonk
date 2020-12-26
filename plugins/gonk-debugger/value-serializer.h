// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk-debugger-defs.h"

#include <script/value.h>

#include <QJsonValue>

namespace gonk
{

// @TODO: try to merge/unify/refactor with gonk::PrettyPrinter
class GonkValueSerializer
{
private:
  script::Engine& m_engine;

public:

  explicit GonkValueSerializer(script::Engine& e);

  QJsonValue serialize(const script::Value& val) const;

};

} // namespace gonk