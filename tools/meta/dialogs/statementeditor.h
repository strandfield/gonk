// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_STATEMENTEDITORDIALOG_H
#define METAGONK_STATEMENTEDITORDIALOG_H

#include <QDialog>

#include "project/statement.h"

class QPlainTextEdit;
class QPushButton;

class StatementEditorDialog : public QDialog
{
  Q_OBJECT
public:
  explicit StatementEditorDialog(StatementRef stmt, QWidget *parent = nullptr);
  ~StatementEditorDialog() = default;

protected Q_SLOTS:
  void onOk();
  void onCancel();

private:
  StatementRef m_statement;
  QPlainTextEdit* m_texteditor;
  QPushButton* m_ok_button;
  QPushButton* m_cancel_button;
};

#endif // METAGONK_STATEMENTEDITORDIALOG_H
