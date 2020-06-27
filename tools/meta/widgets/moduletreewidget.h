// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_MODULETREEWIDGET_H
#define METAGONK_MODULETREEWIDGET_H

#include <QTreeWidget>

#include "project.h"

class QAction;
class QMenu;

class ModuleTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
  ModuleTreeWidget(const MGProjectPtr& pro);

  void setShowCheckboxes(bool visible);

  inline MGProjectPtr project() const { return mProject; }
  void setProject(const MGProjectPtr& pro);

  void fetchNewNodes();

  std::shared_ptr<cxx::Entity> getEntity(QTreeWidgetItem* item) const;
  MGModulePtr getModule(QTreeWidgetItem* item) const;

  bool isModuleItem(QTreeWidgetItem* item) const;

  void removeUncheckedSymbols();

  static QString display(const cxx::Entity& e);
  static QString display(const MGModule& m);

protected:
  void keyPressEvent(QKeyEvent *e);

protected:
  void removeSelectedRows();
  void processCtrlE();
  void processCtrlN();

protected:
  void fillTreeWidget(const MGProjectPtr & pro);
  void fill(QTreeWidgetItem *parent, const std::shared_ptr<cxx::Entity>& node);
  void fill(QTreeWidgetItem* parent, const MGModulePtr& node);
  QTreeWidgetItem* createItem(const MGModulePtr& node);
  QTreeWidgetItem* createItem(const std::shared_ptr<cxx::Entity>& node);
  void refreshItem(QTreeWidgetItem* item);
  void fetchNewNodes(QTreeWidgetItem *item);

protected Q_SLOTS:
  void updateItem(QTreeWidgetItem *item, int column);
  void updateCheckState(QTreeWidgetItem *item);
  void resizeColumnsAuto();
  void displayContextMenu(const QPoint & p);

protected:
  void handle_checkboxes(QTreeWidgetItem* item, bool on);

private:
  void createContextMenus();
  void execAction(QTreeWidgetItem *item, std::shared_ptr<cxx::Entity> node, QAction *act);

private:
  MGProjectPtr mProject;
  bool mShowCheckboxes;
  QMenu *mMenu;
  QAction *mAddCopyCtorAction;
  QAction *mAddDestructorAction;
  QAction *mAddAssignmentAction;
  std::unordered_map<QTreeWidgetItem*, std::shared_ptr<cxx::Entity>> m_nodes_map;
};

#endif // METAGONK_MODULETREEWIDGET_H
