// Copyright (C) 2018 Vincent Chambrin
// This file is part of the Yasl project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef YASL_META_MODULE_H
#define YASL_META_MODULE_H

#include "project/node.h"

class Module : public Node
{
public:
  int module_id = -1;
  QList<NodeRef> elements;

public:
  Module(const QString & n, Qt::CheckState c = Qt::Checked);
  ~Module() = default;

  QString typeName() const override { return "module"; }
  static const NodeType staticTypeCode = NodeType::Module;
  NodeType typeCode() const override { return staticTypeCode; }

  void accept(NodeVisitor& visitor) override;

  void appendChild(NodeRef child) override;
  size_t childCount() const override;
  std::shared_ptr<Node> childAt(size_t index) const override;
  void removeChild(size_t index) override;
  QList<std::shared_ptr<Node>> children() const override;


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

  QString module_snake_name() const;
  QString module_camel_case() const;
  QString module_dir_name() const;
};

typedef std::shared_ptr<Module> ModuleRef;

#endif // YASL_META_MODULE_H
