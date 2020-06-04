// Copyright (C) 2018 Vincent Chambrin
// This file is part of the Yasl project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef YASL_META_MODULETREEWIDGET_H
#define YASL_META_MODULETREEWIDGET_H

#include <QTreeWidget>

#include "project.h"

class QAction;
class QMenu;

class ModuleTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
  ModuleTreeWidget(const ProjectRef & pro);

  void setShowCheckboxes(bool visible);

  inline ProjectRef project() const { return mProject; }
  void setProject(const ProjectRef & pro);

  void fetchNewNodes();

  NodeRef getNode(QTreeWidgetItem* item) const;

protected:
  void keyPressEvent(QKeyEvent *e);

protected:
  void removeSelectedRows();
  void moveSelectedRow(int k);
  void processCtrlE();
  void processCtrlN();

protected:
  void fillTreeWidget(const ProjectRef & pro);
  void fill(QTreeWidgetItem *parent, const NodeRef & node);
  QTreeWidgetItem* createItem(const NodeRef & node);
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
  void execAction(QTreeWidgetItem *item, NodeRef node, QAction *act);

private:
  ProjectRef mProject;
  bool mShowCheckboxes;
  QMenu *mClassMenu;
  QAction *mAddCopyCtorAction;
  QAction *mAddDestructorAction;
  QAction *mAddAssignmentAction;
  QAction *mSortClassMembersAction;
  QAction* mAddClassStatementAction;
  QMenu *mFileNodeMenu;
  QAction *mAddStatementAction;
  std::unordered_map<QTreeWidgetItem*, std::shared_ptr<Node>> m_nodes_map;
};

#endif // YASL_META_MODULETREEWIDGET_H
