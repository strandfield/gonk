// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_IMPORTWIZARD_H
#define METAGONK_IMPORTWIZARD_H

#include <QWizard>

#include "project.h"

class ImportPage;
class SelectInputsPage;
class StartImportPage;

class ImportWizard : public QWizard
{
  Q_OBJECT
public:
  ImportWizard(MGProjectPtr pro);
  ~ImportWizard();

  enum PageIds {
    StartImportPageId = 0,
    SelectInputsPageId,
    FinalizeImportPageId,
  };

  struct Fields
  {
    QStringList includeDirectories;
    QString moduleName;
    QStringList fileList;
    MGProjectPtr importedSymbols;
  };

  inline static ImportWizard * get(QWizard *w) { return static_cast<ImportWizard*>(w); }

  Fields & fields() { return mFields; }
  MGProjectPtr& project() { return mProject; }

private:
  struct
  {
    StartImportPage *startProject;
    SelectInputsPage *selectInputs;
    ImportPage *generate;
  } pages;

  Fields mFields;
  MGProjectPtr mProject;
};

#endif // METAGONK_IMPORTWIZARD_H
