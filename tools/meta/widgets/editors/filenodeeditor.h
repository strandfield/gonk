// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_FILE_NODE_EDITOR_H
#define METAGONK_FILE_NODE_EDITOR_H

#include "abstractnodeeditor.h"

#include "project/file.h"

class QLineEdit;

class FileNodeEditor : public AbstractNodeEditor
{
  Q_OBJECT
public:
  FileNodeEditor(const FileRef & f, QWidget *p = nullptr);
  ~FileNodeEditor() = default;

  void write() override;
  void read(FileRef f);

  inline FileRef getFile() const { return std::static_pointer_cast<File>(getNode()); }

private:
  QLineEdit *mName;
  QLineEdit *mHIncludes;
  QLineEdit *mCppIncludes;
};

#endif // METAGONK_FILE_NODE_EDITOR_H
