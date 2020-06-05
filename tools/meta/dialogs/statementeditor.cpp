// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "dialogs/statementeditor.h"

#include "controller.h"
#include "project-controller.h"

#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

StatementEditorDialog::StatementEditorDialog(StatementRef stmt, QWidget *parent)
  : QDialog(parent),
    m_statement(stmt)
{
  setWindowTitle("Statement Editor");

  m_texteditor = new QPlainTextEdit();
  m_texteditor->setFont(QFont("Courier"));

  QString text = stmt->name;
  text.replace("\\n", "\n");
  m_texteditor->setPlainText(text);

  m_ok_button = new QPushButton("Ok");
  m_cancel_button = new QPushButton("Cancel");

  {
    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(m_texteditor);

    QHBoxLayout* buttons_layout = new QHBoxLayout;
    {
      buttons_layout->addStretch();
      buttons_layout->addWidget(m_ok_button);
      buttons_layout->addWidget(m_cancel_button);
      buttons_layout->addStretch();

      layout->addLayout(buttons_layout);
    }
  }

  connect(m_ok_button, &QPushButton::clicked, this, &StatementEditorDialog::onOk);
  connect(m_cancel_button, &QPushButton::clicked, this, &StatementEditorDialog::onCancel);
}

void StatementEditorDialog::onOk()
{

  QString text = m_texteditor->toPlainText();
  text.replace("\r", "").replace("\n", "\\n");

  Controller::Instance().projectController().update(*m_statement, text);

  accept();
}

void StatementEditorDialog::onCancel()
{
  reject();
}