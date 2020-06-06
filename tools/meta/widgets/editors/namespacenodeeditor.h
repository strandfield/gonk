// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_NAMESPACE_NODE_EDITOR_H
#define METAGONK_NAMESPACE_NODE_EDITOR_H

#include "abstractnodeeditor.h"

#include "project/namespace.h"

class QCheckBox;
class QLineEdit;

class NamespaceNodeEditor : public AbstractNodeEditor
{
  Q_OBJECT
public:
  NamespaceNodeEditor(const NamespaceRef & ns, QWidget *p = nullptr);
  ~NamespaceNodeEditor() = default;

  void write() override;
  void read(NamespaceRef ns);

  inline NamespaceRef getNamespace() const { return std::static_pointer_cast<Namespace>(getNode()); }

private:
  QLineEdit *mName;
};

#endif // METAGONK_NAMESPACE_NODE_EDITOR_H
