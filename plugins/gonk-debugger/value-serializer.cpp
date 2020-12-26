// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "value-serializer.h"

#include <script/engine.h>
#include <script/enumerator.h>
#include <script/typesystem.h>

#include <QJsonArray>
#include <QJsonObject>

#include <sstream>

namespace gonk
{

GonkValueSerializer::GonkValueSerializer(script::Engine& e)
  : m_engine(e)
{

}

QJsonValue GonkValueSerializer::serialize(const script::Value& val) const
{
  switch (val.type().baseType().data())
  {
  case script::Type::Void:
    return "void";
  case script::Type::Boolean:
    return val.toBool() ? true : false;
  case script::Type::Char:
    return QString(QChar(val.toChar()));
  case script::Type::Int:
    return val.toInt();
  case script::Type::Float:
    return val.toFloat();
  case script::Type::Double:
    return val.toDouble();
  case script::Type::String:
    return QString::fromStdString(val.toString());
  default:
    break;
  }

  if (val.type().isEnumType())
  {
    script::Enumerator enm = val.toEnumerator();
    return QString::fromStdString(enm.enumeration().name() + "::" + enm.name());
  }

  //if (val.type().isObjectType())
  //{
  //  script::Function repr_func = reprFunction(val.type());

  //  if (!repr_func.isNull())
  //  {
  //    std::string repr = repr_func.invoke({ val }).toString();
  //    std::cout << repr << std::endl;
  //    return;
  //  }
  //}

  std::stringstream ss;
  ss << (void*)(val.impl());

  std::string repr_str = m_engine.typeSystem()->typeName(val.type()) + "@" + ss.str();
  return QString::fromStdString(repr_str);
}

} // namespace gonk
