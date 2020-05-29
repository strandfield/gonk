// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_NAMESPACE_H
#define METAGONK_NAMESPACE_H

#include "project/node.h"

class Namespace : public Node
{
public:
  QList<NodeRef> elements;
  QString rename;

public:
  Namespace(const QString & n, Qt::CheckState c = Qt::Checked);
  ~Namespace() = default;

  QString typeName() const override { return "namespace"; }
  static const NodeType staticTypeCode = NodeType::Namespace;
  NodeType typeCode() const override { return staticTypeCode; }

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

  QSharedPointer<Namespace> getNamespace(const QString & name);


  void remove(const QString & name);

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
typedef QSharedPointer<Namespace> NamespaceRef;


#endif // METAGONK_NAMESPACE_H
