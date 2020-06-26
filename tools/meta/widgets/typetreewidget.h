// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_TYPETREEWIDGET_H
#define METAGONK_TYPETREEWIDGET_H

#include <QTreeWidget>

#include "project.h"

class TypeTreeWidget : public QTreeWidget
{
  Q_OBJECT
public:
  TypeTreeWidget(const MGProjectPtr & pro);

  void fetchNewNodes();

  inline MGProjectPtr project() const { return mProject; }
  void setProject(const MGProjectPtr& pro);

protected:
  enum Columns {
    NameColumn = 0,
    IdColumn,
  };

  MGType& getType(QTreeWidgetItem *item);
  std::string & getField(MGType & t, int col);

protected:
  void keyPressEvent(QKeyEvent *e) override;

protected Q_SLOTS:
  void updateItem(QTreeWidgetItem *item, int col);

protected:
  void removeSelectedRows();

protected:
  void fillTreeWidget(const MGProjectPtr& pro);
  QTreeWidgetItem* createItem(const MGType& t);

private:
  MGProjectPtr mProject;
  QTreeWidgetItem *mFundamentalTypes;
  QTreeWidgetItem *mEnums;
  QTreeWidgetItem *mClasses;
};

#endif // METAGONK_TYPETREEWIDGET_H
