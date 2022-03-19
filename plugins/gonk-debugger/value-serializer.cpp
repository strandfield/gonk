// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "value-serializer.h"

#include <script/class.h>
#include <script/datamember.h>
#include <script/engine.h>
#include <script/object.h>
#include <script/typesystem.h>

#include <sstream>

namespace gonk
{

GonkValueSerializer::GonkValueSerializer(script::Engine& e)
  : m_engine(e),
    m_printer(new gonk::PrettyPrinter(e))
{

}

std::shared_ptr<debugger::Variable> GonkValueSerializer::serialize(const script::Value& val) const
{
  auto ret = std::make_shared<debugger::Variable>();
  ret->value = m_printer->repr(val);

  if (!val.type().isObjectType())
    return ret;

  script::Class cla = m_engine.typeSystem()->getClass(val.type());
  serialize_members(val, cla, ret->members);

  return ret;
}

void GonkValueSerializer::serialize_members(const script::Value& val, const script::Class& cla, std::vector<std::shared_ptr<debugger::Variable>>& result) const
{
  if (!cla.parent().isNull())
    serialize_members(val, cla.parent(), result);

  for (const script::DataMember& dm : cla.dataMembers())
  {
    std::shared_ptr<debugger::Variable> mvar = serialize(val.toObject().at(result.size()));
    mvar->name = dm.name;
    mvar->type = m_engine.typeSystem()->typeName(dm.type);
    result.push_back(mvar);
  }
}

} // namespace gonk
