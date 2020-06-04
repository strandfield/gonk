// Copyright (C) 2018 Vincent Chambrin
// This file is part of the Yasl project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "typetreewidget.h"

#include "project/type.h"

#include <QKeyEvent>

TypeTreeWidget::TypeTreeWidget(const ProjectRef & pro)
  : mProject(pro)
{
  setColumnCount(3);
  setHeaderLabels(QStringList() << "C++ type" << "Id" << "Header");

  fillTreeWidget(pro);

  connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateItem(QTreeWidgetItem*, int)));
}

void TypeTreeWidget::setProject(const ProjectRef & pro)
{
  mProject = pro;
  fillTreeWidget(pro);
}

void TypeTreeWidget::fetchNewNodes()
{
  for (int i(mFundamentalTypes->childCount()); i < mProject->types.fundamentals.size(); ++i)
    mFundamentalTypes->addChild(createItem(*mProject->types.fundamentals.at(i)));

  for (int i(mEnums->childCount()); i < mProject->types.enums.size(); ++i)
    mEnums->addChild(createItem(*mProject->types.enums.at(i)));

  for (int i(mClasses->childCount()); i < mProject->types.classes.size(); ++i)
    mClasses->addChild(createItem(*mProject->types.classes.at(i)));
}

QList<std::shared_ptr<Type>> & TypeTreeWidget::getTypeList(QTreeWidgetItem *item)
{
  if (item->parent() == mFundamentalTypes)
    return mProject->types.fundamentals;
  else if (item->parent() == mEnums)
    return mProject->types.enums;
  else
    return mProject->types.classes;
}

Type & TypeTreeWidget::getType(QTreeWidgetItem *item)
{
  const int item_index = item->parent()->indexOfChild(item);
  if (item->parent() == mFundamentalTypes)
    return *mProject->types.fundamentals[item_index];
  else if (item->parent() == mEnums)
    return *mProject->types.enums[item_index];
  else
    return *mProject->types.classes[item_index];
}

QString & TypeTreeWidget::getField(Type & t, int col)
{
  switch (col)
  {
  case NameColumn:
    return t.name;
  case IdColumn:
    return t.id;
  case HeaderColumn:
    return t.header;
  }

  throw std::runtime_error{ "TypeTreeWidget::getField() : invalid column" };
}

void TypeTreeWidget::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace)
    removeSelectedRows();
  else if ((e->key() == Qt::Key_Up || e->key() == Qt::Key_Down) && e->modifiers() == Qt::CTRL)
    moveSelectedRow(e->key());
  else
    QTreeWidget::keyPressEvent(e);
}

void TypeTreeWidget::removeSelectedRows()
{
  const QList<QTreeWidgetItem*> selecteds = selectedItems();
  for (auto item : selecteds)
  {
    const int item_index = item->parent()->indexOfChild(item);
    if (item->parent() == mFundamentalTypes)
      mProject->types.fundamentals.removeAt(item_index);
    else if (item->parent() == mEnums)
      mProject->types.enums.removeAt(item_index);
    else
      mProject->types.classes.removeAt(item_index);

    item->parent()->removeChild(item);
  }
}

void TypeTreeWidget::moveSelectedRow(int k)
{
  const QList<QTreeWidgetItem*> selecteds = selectedItems();
  if (selecteds.size() != 1)
    return;

  QTreeWidgetItem *item = selecteds.first();
  QTreeWidgetItem *parent = item->parent();
  const int item_index = parent->indexOfChild(item);

  if (item_index == 0 && k == Qt::Key_Up)
    return;
  else if (item_index == parent->childCount() - 1 && k == Qt::Key_Down)
    return;

  QList<std::shared_ptr<Type>> & types = getTypeList(item);

  if (k == Qt::Key_Up)
  {
    types.swap(item_index, item_index - 1);
    QTreeWidgetItem *sibling = parent->takeChild(item_index - 1);
    parent->insertChild(item_index, sibling);
  }
  else if (k == Qt::Key_Down)
  {
    types.swap(item_index, item_index + 1);

    QTreeWidgetItem *sibling = parent->takeChild(item_index + 1);
    parent->insertChild(item_index, sibling);
  }
}

void TypeTreeWidget::updateItem(QTreeWidgetItem *item, int col)
{
  Type & t = getType(item);
  getField(t, col) = item->text(col);
}

void TypeTreeWidget::fillTreeWidget(const ProjectRef & pro)
{
  clear();

  mFundamentalTypes = new QTreeWidgetItem();
  mFundamentalTypes->setText(0, "Fundamental types");
  fill(mFundamentalTypes, pro->types.fundamentals);

  mEnums = new QTreeWidgetItem();
  mEnums->setText(0, "Enumerations");
  fill(mEnums, pro->types.enums);

  mClasses = new QTreeWidgetItem();
  mClasses->setText(0, "Classes");
  fill(mClasses, pro->types.classes);

  invisibleRootItem()->addChild(mFundamentalTypes);
  invisibleRootItem()->addChild(mEnums);
  invisibleRootItem()->addChild(mClasses);
}

void TypeTreeWidget::fill(QTreeWidgetItem *parent, const QList<std::shared_ptr<Type>> & types)
{
  for (const auto & t : types)
  {
    QTreeWidgetItem *item = createItem(*t);
    parent->addChild(item);
  }
}

QTreeWidgetItem* TypeTreeWidget::createItem(const Type & t)
{
  QTreeWidgetItem *item = new QTreeWidgetItem();
  item->setFlags(item->flags() | Qt::ItemIsEditable);
  item->setText(NameColumn, t.name);
  item->setText(IdColumn, t.id);
  item->setText(HeaderColumn, t.header);
  return item;
}
