// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk-debugger-defs.h"

#include "gonk/pretty-print.h"

#include <QJsonArray>
#include <QJsonObject>

#include <memory>

namespace script
{
class Class;
} // namespace script

namespace gonk
{

class GonkValueSerializer
{
private:
  script::Engine& m_engine;
  std::unique_ptr<gonk::PrettyPrinter> m_printer;

public:

  explicit GonkValueSerializer(script::Engine& e);

  QJsonValue serialize(const script::Value& val) const;

protected:
  size_t serialize_members(const script::Value& val, const script::Class& cla, QJsonArray& result);
};

} // namespace gonk