// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_TYPETREEWIDGET_H
#define METAGONK_TYPETREEWIDGET_H

#include <QTreeWidget>

#include "project.h"

class Type;

class TypeTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
  TypeTreeWidget(const ProjectRef & pro);

  void fetchNewNodes();

  inline ProjectRef project() const { return mProject; }
  void setProject(const ProjectRef & pro);

protected:
  enum Columns {
    NameColumn = 0,
    IdColumn,
    HeaderColumn,
  };

  QList<std::shared_ptr<Type>> & getTypeList(QTreeWidgetItem *item);
  Type & getType(QTreeWidgetItem *item);
  QString & getField(Type & t, int col);

protected:
  void keyPressEvent(QKeyEvent *e) override;

protected Q_SLOTS:
  void updateItem(QTreeWidgetItem *item, int col);

protected:
  void removeSelectedRows();
  void moveSelectedRow(int k);

protected:
  void fillTreeWidget(const ProjectRef & pro);
  void fill(QTreeWidgetItem *parent, const QList<std::shared_ptr<Type>> & types);
  QTreeWidgetItem* createItem(const Type & t);

private:
  ProjectRef mProject;
  QTreeWidgetItem *mFundamentalTypes;
  QTreeWidgetItem *mEnums;
  QTreeWidgetItem *mClasses;
};

#endif // METAGONK_TYPETREEWIDGET_H
