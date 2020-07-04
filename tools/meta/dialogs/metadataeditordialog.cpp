// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "metadataeditordialog.h"

#include "controller.h"
#include "project-controller.h"

#include <json-toolkit/parsing.h>
#include <json-toolkit/stringify.h>

#include <QFormLayout>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QVBoxLayout>

MetadataEditorDialog::MetadataEditorDialog(MGProjectPtr pro, const std::shared_ptr<cxx::Entity>& e, QWidget* p)
  : QDialog(p),
    m_project(pro),
    m_entity(e), 
    m_textedit(nullptr)
{
  m_textedit = new QPlainTextEdit();

  auto* okButton = new QPushButton("OK");
  connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

  auto* layout = new QVBoxLayout(this);
  layout->addWidget(m_textedit);
  layout->addWidget(okButton, 0, Qt::AlignCenter);

  std::string metadata = json::stringify(pro->getMetadata(e));
  m_textedit->setPlainText(QString::fromStdString(metadata));
}

void MetadataEditorDialog::sync()
{
  std::string metadata_content = m_textedit->toPlainText().toStdString();

  json::Json metadata = json::parse(metadata_content);

  Controller::Instance().projectController().update(*m_entity,
    metadata.toObject());
}

