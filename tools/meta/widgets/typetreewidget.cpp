// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "typetreewidget.h"

#include "controller.h"
#include "project-controller.h"

#include "project.h"

#include <QKeyEvent>

#include <stdexcept>

TypeTreeWidget::TypeTreeWidget(const MGProjectPtr& pro)
  : mProject(pro)
{
  setColumnCount(2);
  setHeaderLabels(QStringList() << "C++ type" << "Id");

  fillTreeWidget(pro);

  connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateItem(QTreeWidgetItem*, int)));
}

void TypeTreeWidget::setProject(const MGProjectPtr& pro)
{
  mProject = pro;
  fillTreeWidget(pro);
}

void TypeTreeWidget::fetchNewNodes()
{
  int offset = mFundamentalTypes->childCount() + mEnums->childCount() + mClasses->childCount();

  for (auto it = mProject->types.begin() + static_cast<size_t>(offset); it != mProject->types.end(); ++it)
  {
    MGTypePtr t = *it;

    QTreeWidgetItem* item = createItem(*t);

    if (t->category == MGType::ClassType)
      mClasses->addChild(item);
    else if (t->category == MGType::EnumType)
      mEnums->addChild(item);
    else if (t->category == MGType::FundamentalType)
      mFundamentalTypes->addChild(item);
  }
}

MGType& TypeTreeWidget::getType(QTreeWidgetItem *item)
{
  return *mProject->getTypeById(item->text(IdColumn).toStdString());
}

std::string & TypeTreeWidget::getField(MGType & t, int col)
{
  switch (col)
  {
  case NameColumn:
    return t.name;
  case IdColumn:
    return t.id;
  }

  throw std::runtime_error{ "TypeTreeWidget::getField() : invalid column" };
}

void TypeTreeWidget::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace)
    removeSelectedRows();
  else
    QTreeWidget::keyPressEvent(e);
}

void TypeTreeWidget::removeSelectedRows()
{
  const QList<QTreeWidgetItem*> selecteds = selectedItems();

  for (auto item : selecteds)
  {
    try
    {
      auto t = mProject->getTypeById(item->text(IdColumn).toStdString());
      Controller::Instance().projectController().remove(t, mProject);
    }
    catch (...)
    {
      return;
    }

    item->parent()->removeChild(item);
  }
}

void TypeTreeWidget::updateItem(QTreeWidgetItem *item, int col)
{
  MGType& t = getType(item);
  getField(t, col) = item->text(col).toStdString();
}

void TypeTreeWidget::fillTreeWidget(const MGProjectPtr & pro)
{
  clear();

  mFundamentalTypes = new QTreeWidgetItem();
  mFundamentalTypes->setText(0, "Fundamental types");

  mEnums = new QTreeWidgetItem();
  mEnums->setText(0, "Enumerations");

  mClasses = new QTreeWidgetItem();
  mClasses->setText(0, "Classes");

  fetchNewNodes();

  invisibleRootItem()->addChild(mFundamentalTypes);
  invisibleRootItem()->addChild(mEnums);
  invisibleRootItem()->addChild(mClasses);
}

QTreeWidgetItem* TypeTreeWidget::createItem(const MGType & t)
{
  QTreeWidgetItem *item = new QTreeWidgetItem();
  item->setFlags(item->flags() | Qt::ItemIsEditable);
  item->setText(NameColumn, QString::fromStdString(t.name));
  item->setText(IdColumn, QString::fromStdString(t.id));
  return item;
}
