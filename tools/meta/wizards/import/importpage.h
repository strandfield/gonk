// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_IMPORTPAGE_H
#define METAGONK_IMPORTPAGE_H

#include <QWizardPage>

#include "project.h"

class SymbolsTreeWidget;

class ImportPage : public QWizardPage
{
  Q_OBJECT
public:
  ImportPage();

protected:
  void cleanupPage() override;
  void initializePage() override;
  bool validatePage() override;

private:
  SymbolsTreeWidget *selectionTreeWidget;
};

#endif // METAGONK_IMPORTPAGE_H
