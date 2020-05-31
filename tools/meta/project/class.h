// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_CLASS_H
#define METAGONK_CLASS_H

#include "project/node.h"

class Class : public Node
{
public:
  int class_id = -1;
  int type_id = -1;
  QList<NodeRef> elements;
  QString base;
  bool isFinal;

public:
  Class(const QString & n, Qt::CheckState c = Qt::Checked);
  ~Class() = default;

  QString typeName() const override { return "class"; }
  static const NodeType staticTypeCode = NodeType::Class;
  NodeType typeCode() const override { return staticTypeCode; }

  QString display() const override;

  void accept(NodeVisitor& visitor) override;

  void appendChild(NodeRef n) override;
  size_t childCount() const override;
  std::shared_ptr<Node> childAt(size_t index) const override;
  void removeChild(size_t index) override;
  QList<std::shared_ptr<Node>> children() const override;

  template<typename T>
  std::shared_ptr<T> add(const QString & name)
  {
    auto ret = std::make_shared<T>(name);
    elements.append(ret);
    return ret;
  }

  template<typename T>
  std::shared_ptr<T> get(const QString & name)
  {
    for (const auto & e : elements)
    {
      if (e->is<T>() && e->name == name)
        return std::static_pointer_cast<T>(e);
    }

    auto ret = std::make_shared<T>(name);
    elements.append(ret);
    return ret;
  }

  template<typename T>
  void remove(const QString & name)
  {
    for (int i(0); i < elements.size(); ++i)
    {
      auto e = elements.at(i);
      if (e->is<T>() && e->name == name)
      {
        elements.removeAt(i);
        return;
      }
    }
  }
};
typedef std::shared_ptr<Class> ClassRef;


#endif // METAGONK_CLASS_H
