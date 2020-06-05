// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_NODE_H
#define METAGONK_NODE_H

#include <QJsonObject>
#include <QMap>
#include <QSharedPointer>
#include <QStack>

#include <memory>
#include <vector>

enum class NodeType
{
  Module,
  File,
  Namespace,
  Enum,
  Enumerator,
  Class,
  Function,
  Statement,
};

class NodeVisitor;

class Node : public std::enable_shared_from_this<Node>
{
public:
  Node(const QString & n, Qt::CheckState c = Qt::Checked);
  virtual ~Node() = default;

  template<typename T>
  bool is() const
  {
    return dynamic_cast<const T*>(this) != nullptr;
  }

  template<typename T>
  T & as()
  {
    return *dynamic_cast<T*>(this);
  }

  virtual QString display() const { return name; }
  virtual QString typeName() const = 0;
  virtual NodeType typeCode() const = 0;

  virtual void accept(NodeVisitor& visitor) = 0;

  virtual void appendChild(std::shared_ptr<Node> n);
  virtual size_t childCount() const;
  virtual std::shared_ptr<Node> childAt(size_t index) const;
  virtual void removeChild(size_t index);
  virtual QList<std::shared_ptr<Node>> children() const;

  static QString nameQualification(const QStack<std::shared_ptr<Node>> & nodes);
  static QString nameQualification(const std::vector<std::shared_ptr<Node>>& nodes);

  static int compare(const Node & a, const Node & b);

  int entity_id = -1;
  std::weak_ptr<Node> parent;
  int order = -1;
  QString name;
  Qt::CheckState checkState;
  QString condition;

protected:
  virtual int compareTo(const Node & other) const;
};
typedef std::shared_ptr<Node> NodeRef;

/// TODO: this seems to be dead code, remove!
class NodeValue
{
public:
  NodeRef value;

  NodeValue(const NodeRef & val) : value(val) { }
  NodeValue(const NodeValue & ) = default;

  NodeValue & operator=(const NodeValue &) = default;
};

bool eq(const NodeRef & lhs, const NodeRef & rhs);
inline bool neq(const NodeRef & lhs, const NodeRef & rhs)
{
  return !eq(lhs, rhs);
}

void sort(QList<NodeRef> & list);

struct RAIINodeGuard
{
  NodeRef& target;
  NodeRef value;

  explicit RAIINodeGuard(NodeRef& node)
    : target(node),
    value(node)
  {

  }

  ~RAIINodeGuard()
  {
    target = value;
  }
};

#endif // METAGONK_NODE_H
