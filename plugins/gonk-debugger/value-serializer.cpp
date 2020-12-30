// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "value-serializer.h"

#include <script/class.h>
#include <script/datamember.h>
#include <script/engine.h>
#include <script/object.h>
#include <script/typesystem.h>

#include <QJsonArray>
#include <QJsonObject>

#include <sstream>

namespace gonk
{

GonkValueSerializer::GonkValueSerializer(script::Engine& e)
  : m_engine(e),
    m_printer(new gonk::PrettyPrinter(e))
{

}

QJsonValue GonkValueSerializer::serialize(const script::Value& val) const
{
  std::string repr = m_printer->repr(val);
  return QString::fromStdString(repr);

  /*if (!val.type().isObjectType())
    return QString::fromStdString(repr);

  QJsonObject result;
  result["__type"] = "object";
  result["__repr"] = QString::fromStdString(repr);

  script::Class cla = m_engine.typeSystem()->getClass(val.type());

  {
    QJsonArray members;

    std::vector<script::DataMember> mbs = cla.dataMembers();
  }

  return result;*/
}

size_t GonkValueSerializer::serialize_members(const script::Value& val, const script::Class& cla, QJsonArray& result)
{
  size_t off = 0;

  if (!cla.parent().isNull())
    off = serialize_members(val, cla.parent(), result);

  for (const script::DataMember& dm : cla.dataMembers())
  {
    QJsonValue jsonval = serialize(val.toObject().at(off));
    QJsonObject jsonmember;
    jsonmember["name"] = QString::fromStdString(dm.name);
    result.append(jsonmember);
    ++off;
  }

  return off;
}

} // namespace gonk
