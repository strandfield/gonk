// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "wizards/import/importpage.h"

#include "controller.h"
#include "cppparser.h"

#include "widgets/moduletreewidget.h"
#include "wizards/importwizard.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

#include <QDebug>

ImportPage::ImportPage()
{
  setTitle("Select symbols to import");

  QVBoxLayout *l = new QVBoxLayout(this);

  QHBoxLayout *hl = new QHBoxLayout();
  l->addLayout(hl);
  hl->addWidget(new QLabel("Select the elements you want to process:"));
  hl->addItem(new QSpacerItem(20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));

  selectionTreeWidget = new ModuleTreeWidget(nullptr);
  selectionTreeWidget->setShowCheckboxes(true);
  l->addWidget(selectionTreeWidget);
}


void ImportPage::cleanupPage()
{
  auto & fields = ImportWizard::get(wizard())->fields();
  fields.importedSymbols->types.clear();

  selectionTreeWidget->clear();

  QWizardPage::cleanupPage();
}

void ImportPage::initializePage()
{
  auto & fields = ImportWizard::get(wizard())->fields(); 

  CppParser parser{ fields.importedSymbols };
  parser.setIncludeDirectories(fields.includeDirectories);

  for (const auto & f : fields.fileList)
    parser.parse(f);

  selectionTreeWidget->setProject(fields.importedSymbols);
  selectionTreeWidget->setShowCheckboxes(true);
}

bool ImportPage::validatePage()
{
  auto & fields = ImportWizard::get(wizard())->fields();

  selectionTreeWidget->removeUncheckedSymbols();

  MGProjectPtr pro = ImportWizard::get(wizard())->project();
  Controller::Instance().importSymbols(fields.importedSymbols);

  return true;
}
