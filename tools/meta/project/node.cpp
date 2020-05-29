// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project/node.h"

#include "project/file.h"
#include "project/function.h"
#include "project/module.h"

namespace yaml
{

QString extractField(QString str, const QString & fieldName)
{
  const int index = str.indexOf("[" + fieldName + ":");
  if (index == -1)
    return QString();

  const int end = str.indexOf(']', index + 2 + fieldName.size());
  Q_ASSERT(end != -1);
  const int begin = index + 2 + fieldName.size();
  return str.mid(begin, end - begin).trimmed();
}

QString createField(const QString &fieldName, const QString & content)
{
  static const QString fmt = "[%1:%2]";
  return fmt.arg(fieldName, content);
}

QString checkStateField(Qt::CheckState cs)
{
  static const QString unchecked = "[unchecked]";
  static const QString partiallychecked = "[partially-checked]";

  if (cs == Qt::Unchecked)
    return unchecked;
  else if (cs == Qt::PartiallyChecked)
    return partiallychecked;
  return QString();
}

Qt::CheckState checkstate(const QString & str)
{
  static const QString unchecked = "[unchecked]";
  static const QString partiallychecked = "[partially-checked]";

  if (str.contains(unchecked))
    return Qt::Unchecked;
  else if (str.contains(partiallychecked))
    return Qt::PartiallyChecked;
  return Qt::Checked;
}

int firstFieldIndex(const QString & str)
{
  int ret = str.indexOf('[');
  while (ret != -1 && str.at(ret + 1) == ']')
    ret = str.indexOf('[', ret + 1);
  return ret;
}

QString extractName(QString str)
{
  return str.mid(0, yaml::firstFieldIndex(str));
}

} // namespace yaml

QMap<QString, Node::JsonDeserializer> Node::staticFactory = QMap<QString, Node::JsonDeserializer>{};

namespace json
{

Qt::CheckState readCheckState(const QJsonObject & obj)
{
  if(obj.contains("checked"))
    return static_cast<Qt::CheckState>(obj.value("checked").toInt());
  return Qt::Checked;
}

void writeCheckState(QJsonObject & obj, Qt::CheckState cs)
{
  if(cs != Qt::Checked)
    obj["checked"] = static_cast<int>(cs);
}

} // namespace json

Node::Node(const QString & n, Qt::CheckState c)
  : checkState(c)
  , name(n)
{

}

void Node::fillJson(QJsonObject & obj) const
{
  obj["name"] = this->name;
  json::writeCheckState(obj, this->checkState);
  obj["type"] = typeName();
}

QJsonObject Node::toJson() const
{
  QJsonObject result;
  fillJson(result);
  return result;
}

QSharedPointer<Node> Node::fromJson(const QJsonObject & obj)
{
  return staticFactory.value(obj.value("type").toString(), nullptr)(obj);
}

void Node::appendChild(QSharedPointer<Node> n)
{
  throw std::runtime_error{ "Node does not support child insertion" };
}

void Node::registerDeserializer(const QString & name, JsonDeserializer func)
{
  staticFactory[name] = func;
}

QString Node::nameQualification(const QStack<QSharedPointer<Node>> & nodes)
{
  if (nodes.isEmpty())
    return "";

  QStringList names;
  for (const auto & node : nodes)
  {
    if (node->is<File>() || node->is<Module>())
      continue;

    names << node->name;
  }

  if (names.empty())
    return "";
  return names.join("::") + "::";
}

int Node::compare(const Node & a, const Node & b)
{
  if (a.typeCode() < b.typeCode())
    return -1;
  else if (a.typeCode() > b.typeCode())
    return 1;

  return a.compareTo(b);
}

int Node::compareTo(const Node & other) const
{
  return this->name.compare(other.name);
}

bool eq(const NodeRef & lhs, const NodeRef & rhs)
{
  return Node::compare(*lhs, *rhs) == 0;
}

void sort(QList<NodeRef> & list)
{
  qSort(list.begin(), list.end(), [](const NodeRef & a, const NodeRef & b) -> bool {
    return Node::compare(*a, *b) < 0;
  });
}
