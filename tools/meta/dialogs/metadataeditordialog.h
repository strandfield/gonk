// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_METADATA_EDITOR_DIALOG_H
#define METAGONK_METADATA_EDITOR_DIALOG_H

#include <QDialog>

#include "project.h"

class QPlainTextEdit;

class MetadataEditorDialog : public QDialog
{
  Q_OBJECT
public:
  MetadataEditorDialog(MGProjectPtr pro, const std::shared_ptr<cxx::Entity> & e, QWidget *p = nullptr);
  ~MetadataEditorDialog() = default;

  void sync();

private:
  MGProjectPtr m_project;
  std::shared_ptr<cxx::Entity> m_entity;
  QPlainTextEdit* m_textedit;
};

#endif // METAGONK_METADATA_EDITOR_DIALOG_H
