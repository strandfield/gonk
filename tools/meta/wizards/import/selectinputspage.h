// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_SELECTINPUTSPAGE_H
#define METAGONK_SELECTINPUTSPAGE_H

#include <QWizardPage>

class QPushButton;
class QTreeWidget;
class QTreeWidgetItem;

class SelectInputsPage : public QWizardPage
{
  Q_OBJECT
public:
  SelectInputsPage();


protected:
  void initializePage() override;
  bool validatePage() override;

protected Q_SLOTS:
  void unselectAll();

protected:
  int fetchFiles(QTreeWidgetItem *parent, const QString & dir);
  void getSelectedFiles(QStringList & result, QTreeWidgetItem *item);
  void unselectAll(QTreeWidgetItem *item);

private:
  QPushButton *unselectAllButton;
  QTreeWidget *sourceTreeWidget;
};

#endif // METAGONK_SELECTINPUTSPAGE_H
