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

  void appendChild(NodeRef n) override;

  void fillJson(QJsonObject & obj) const override;
  static QSharedPointer<Node> fromJson(const QJsonObject & val);

  template<typename T>
  QSharedPointer<T> add(const QString & name)
  {
    auto ret = QSharedPointer<T>::create(name);
    elements.append(ret);
    return ret;
  }

  template<typename T>
  QSharedPointer<T> get(const QString & name)
  {
    for (const auto & e : elements)
    {
      if (e->is<T>() && e->name == name)
        return qSharedPointerCast<T>(e);
    }

    auto ret = QSharedPointer<T>::create(name);
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
typedef QSharedPointer<Class> ClassRef;


#endif // METAGONK_CLASS_H
