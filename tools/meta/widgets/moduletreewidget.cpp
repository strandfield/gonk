// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "moduletreewidget.h"

#include "controller.h"
#include "project-controller.h"

#include "dialogs/newfunctiondialog.h"
#include "dialogs/metadataeditordialog.h"

#include "widgets/editors/classnodeeditor.h"
#include "widgets/editors/enumnodeeditor.h"
#include "widgets/editors/functionnodeeditor.h"
#include "widgets/editors/namespacenodeeditor.h"
#include "widgets/editors/simplenodeeditor.h"

#include <cxx/class.h>
#include <cxx/enum.h>
#include <cxx/function.h>
#include <cxx/namespace.h>

#include <QAction>
#include <QKeyEvent>
#include <QMenu>
#include <QStyledItemDelegate>

#include <QDebug>

class ItemDelegate : public QStyledItemDelegate
{
public:
  ItemDelegate(QObject* parent = nullptr) : QStyledItemDelegate(parent) {}

  virtual QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
  {
    QTreeWidgetItem *item = static_cast<QTreeWidgetItem*>(index.internalPointer());
    std::shared_ptr<cxx::Entity> node = dynamic_cast<ModuleTreeWidget*>(item->treeWidget())->getEntity(item);

    if (node->is<cxx::Function>() && index.column() == 0)
      return new FunctionNodeEditor(std::static_pointer_cast<cxx::Function>(node), parent);
    else if (node->is<cxx::Namespace>() && index.column() == 0)
      return new NamespaceNodeEditor(std::static_pointer_cast<cxx::Namespace>(node), parent);
    else if (node->is<cxx::Class>() && index.column() == 0)
      return new ClassNodeEditor(std::static_pointer_cast<cxx::Class>(node), parent);
    else if (node->is<cxx::Enum>() && index.column() == 0)
      return new EnumNodeEditor(std::static_pointer_cast<cxx::Enum>(node), parent);

      return nullptr;
  }

  void setEditorData(QWidget *editor, const QModelIndex & index) const override
  {
    QTreeWidgetItem *item = static_cast<QTreeWidgetItem*>(index.internalPointer());
    std::shared_ptr<cxx::Entity> node = dynamic_cast<ModuleTreeWidget*>(item->treeWidget())->getEntity(item);

    EnumNodeEditor *enmedit = qobject_cast<EnumNodeEditor*>(editor);
    FunctionNodeEditor *funedit = qobject_cast<FunctionNodeEditor*>(editor);
    ClassNodeEditor *classedit = qobject_cast<ClassNodeEditor*>(editor);
    NamespaceNodeEditor* nsedit = qobject_cast<NamespaceNodeEditor*>(editor);
    SimpleNodeEditor* simple_edit = qobject_cast<SimpleNodeEditor*>(editor);
    if (funedit != nullptr)
    {
      funedit->read(std::static_pointer_cast<cxx::Function>(node));
      return;
    }
    else if (classedit != nullptr)
    {
      classedit->read(std::static_pointer_cast<cxx::Class>(node));
      return;
    }
    else if (enmedit != nullptr)
    {
      enmedit->read(std::static_pointer_cast<cxx::Enum>(node));
      return;
    }
    else if (nsedit != nullptr)
    {
      nsedit->read(std::static_pointer_cast<cxx::Namespace>(node));
      return;
    }
    else if (simple_edit != nullptr)
    {
      simple_edit->read(node);
      return;
    }
    else
    {
      return QStyledItemDelegate::setEditorData(editor, index);
    }
  }

  void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex & index) const override
  {
    QTreeWidgetItem *item = static_cast<QTreeWidgetItem*>(index.internalPointer());

    AbstractNodeEditor *nodeedit = qobject_cast<AbstractNodeEditor*>(editor);
    if (nodeedit != nullptr)
    {
      nodeedit->write();
      item->setText(0, ModuleTreeWidget::display(*nodeedit->getNode()));
      return;
    }
    else
    {
      return QStyledItemDelegate::setModelData(editor, model, index);
    }
  }

  void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &/* index */) const override
  {
    editor->setGeometry(option.rect);
  }
};


ModuleTreeWidget::ModuleTreeWidget(const MGProjectPtr& pro)
  : mProject(pro)
  , mShowCheckboxes(true)
{
  setColumnCount(1);
  setHeaderHidden(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::ExtendedSelection);
  setItemDelegate(new ItemDelegate(this));
  setContextMenuPolicy(Qt::CustomContextMenu);

  createContextMenus();

  if(pro != nullptr)
    fillTreeWidget(pro);

  setShowCheckboxes(true);

  connect(this, SIGNAL(itemChanged(QTreeWidgetItem*, int)), this, SLOT(updateItem(QTreeWidgetItem*, int)));
  connect(this, SIGNAL(itemDoubleClicked(QTreeWidgetItem*, int)), this, SLOT(onItemDoubleClicked(QTreeWidgetItem*, int)));
  connect(this, SIGNAL(collapsed(const QModelIndex&)), this, SLOT(resizeColumnsAuto()));
  connect(this, SIGNAL(expanded(const QModelIndex&)), this, SLOT(resizeColumnsAuto()));
  connect(this, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(displayContextMenu(const QPoint &)));
}

void ModuleTreeWidget::handle_checkboxes(QTreeWidgetItem *item, bool on)
{
  if (on)
  {
    item->setCheckState(0, Qt::Checked);
    item->setData(0, Qt::ForegroundRole, QVariant());
  }
  else
  {
    item->setData(0, Qt::CheckStateRole, QVariant());
  }

  for (int i(0); i < item->childCount(); ++i)
    handle_checkboxes(item->child(i), on);
}

void ModuleTreeWidget::setShowCheckboxes(bool visible)
{
  for (int i(0); i < topLevelItemCount(); ++i)
    handle_checkboxes(topLevelItem(i), visible);
}

void ModuleTreeWidget::setProject(const MGProjectPtr& pro)
{
  mProject = pro;
  fillTreeWidget(pro);
}

void ModuleTreeWidget::fetchNewNodes()
{
  for (int i(0); i < topLevelItemCount(); ++i)
    fetchNewNodes(topLevelItem(i));

  for (int i(topLevelItemCount()); i < mProject->modules.size(); ++i)
    addTopLevelItem(createItem(mProject->modules.at(i)));
}

bool ModuleTreeWidget::isModuleItem(QTreeWidgetItem* item) const
{
  return item->parent() == invisibleRootItem() || item->parent() == nullptr;
}

void ModuleTreeWidget::removeUncheckedSymbols()
{
  using Entry = std::pair<QTreeWidgetItem*, std::pair<MGModulePtr, std::shared_ptr<cxx::Entity>>>;

  auto get_child = [](std::pair<MGModulePtr, std::shared_ptr<cxx::Entity>> p, int index) ->std::shared_ptr<cxx::Entity> {
    if (p.first)
      return p.first->entities.at(static_cast<size_t>(index));
    
    if (p.second->is<cxx::Class>())
    {
      auto& c = static_cast<cxx::Class&>(*p.second);
      return c.members.at(static_cast<size_t>(index));
    }
    else if (p.second->is<cxx::Namespace>())
    {
      auto& e = static_cast<cxx::Namespace&>(*p.second);
      return e.entities.at(static_cast<size_t>(index));
    }
    else if (p.second->is<cxx::Enum>())
    {
      auto& e = static_cast<cxx::Enum&>(*p.second);
      return e.values.at(static_cast<size_t>(index));
    }
    else
    {
      return nullptr;
    }
  };

  std::list<Entry> entries;

  for (int i(0); i < topLevelItemCount(); ++i)
  {
    entries.push_back({ topLevelItem(i), {mProject->modules.at(i), nullptr} });
  }

  while (!entries.empty())
  {
    auto e = entries.front();
    entries.pop_front();

    if (e.first->checkState(0) == Qt::Checked)
      continue;

    if (e.first->checkState(0) == Qt::PartiallyChecked)
    {
      for (int i(0); i < e.first->childCount(); ++i)
      {
        entries.push_back({ e.first->child(i), {nullptr, get_child(e.second, i)} });
      }
    }
    else
    {
      if (e.second.first)
      {
        Controller::Instance().projectController().remove(e.second.first, mProject);
        invisibleRootItem()->removeChild(e.first);
      }
      else
      {
        Controller::Instance().projectController().remove(e.second.second, mProject);
        e.first->parent()->removeChild(e.first);
      }
    }
  }
}

QString ModuleTreeWidget::display(const cxx::Entity& e)
{
  if (e.is<cxx::Function>())
  {
    return QString::fromStdString(::signature(static_cast<const cxx::Function&>(e)));
  }
  else
  {
    return QString::fromStdString(e.name);
  }
}

QString ModuleTreeWidget::display(const MGModule& m)
{
  return QString::fromStdString(m.name);
}

void ModuleTreeWidget::keyPressEvent(QKeyEvent *e)
{
  if (e->key() == Qt::Key_Delete || e->key() == Qt::Key_Backspace)
    removeSelectedRows();
  else if (e->key() == Qt::Key_N && e->modifiers() == Qt::CTRL)
    processCtrlN();
  else if (e->key() == Qt::Key_E && e->modifiers() == Qt::CTRL)
    processCtrlE();
  else if (e->key() == Qt::Key_M && e->modifiers() == Qt::CTRL)
    processCtrlM();
  else
    QTreeWidget::keyPressEvent(e);
}

void ModuleTreeWidget::removeSelectedRows()
{
  const QList<QTreeWidgetItem*> selecteds = selectedItems();
  for (auto item : selecteds)
  {
    Controller::Instance().projectController().remove(getEntity(item), mProject);

    QTreeWidgetItem *parent = item->parent();
    if (parent == nullptr)
      parent = invisibleRootItem();
    const int item_index = parent->indexOfChild(item);
    parent->removeChild(item);
  }
}

void ModuleTreeWidget::processCtrlE()
{
  const QList<QTreeWidgetItem*> selecteds = selectedItems();
  if (selecteds.size() != 1)
    return;

  QTreeWidgetItem *item = selecteds.first();

  std::shared_ptr<cxx::Entity> node = m_nodes_map.at(item);
  if (node == nullptr)
    return;

  if (!node->is<cxx::Function>())
    return;

  auto *dialog = new NewFunctionDialog(std::static_pointer_cast<cxx::Function>(node), this);
  int result = dialog->exec();
  dialog->deleteLater();
  if (result != QDialog::Accepted)
    return;

  dialog->sync();

  refreshItem(item);
}

void ModuleTreeWidget::processCtrlM()
{
  const QList<QTreeWidgetItem*> selecteds = selectedItems();
  if (selecteds.size() != 1)
    return;

  QTreeWidgetItem* item = selecteds.first();

  std::shared_ptr<cxx::Entity> node = m_nodes_map.at(item);
  if (node == nullptr)
    return;

  auto* dialog = new MetadataEditorDialog(mProject, node, this);
  int result = dialog->exec();
  dialog->deleteLater();
  if (result != QDialog::Accepted)
    return;

  dialog->sync();

  refreshItem(item);
}

void ModuleTreeWidget::processCtrlN()
{
  const QList<QTreeWidgetItem*> selecteds = selectedItems();
  if (selecteds.size() != 1)
    return;

  QTreeWidgetItem *item = selecteds.first();

  std::shared_ptr<cxx::Entity> node = m_nodes_map.at(item);
  if (node == nullptr)
    return;

  if (!node->is<cxx::Class>() && !node->is<cxx::Namespace>())
    return;

  auto *dialog = new NewFunctionDialog(this);
  int result = dialog->exec();
  dialog->deleteLater();
  if (result != QDialog::Accepted)
    return;

  dialog->sync();

  if(node->is<cxx::Class>())
    std::static_pointer_cast<cxx::Class>(node)->members.push_back(dialog->function());
  else if (node->is<cxx::Namespace>())
    std::static_pointer_cast<cxx::Namespace>(node)->entities.push_back(dialog->function());

  fetchNewNodes();
}

void ModuleTreeWidget::refreshItem(QTreeWidgetItem* item)
{
  auto node = m_nodes_map.at(item);
  if (node == nullptr)
    return;

  blockSignals(true);

  item->setText(0, display(*node));

  blockSignals(false);
}

void ModuleTreeWidget::fetchNewNodes(QTreeWidgetItem *item)
{
  auto m = getModule(item);

  if (m)
  {
    for (int i(item->childCount()); i < m->entities.size(); ++i)
      item->addChild(createItem(m->entities.at(i)));
    return;
  }

  auto node = m_nodes_map.at(item);
  if (node == nullptr)
    return;

   if (node->is<cxx::Namespace>())
  {
     auto& ns = static_cast<cxx::Namespace&>(*node);

    for (int i(item->childCount()); i < ns.entities.size(); ++i)
      item->addChild(createItem(ns.entities.at(i)));
  }
  else if (node->is<cxx::Class>())
  {
    auto& c = static_cast<cxx::Class&>(*node);

    for (int i(item->childCount()); i < c.members.size(); ++i)
      item->addChild(createItem(c.members.at(i)));
  }
  else if (node->is<cxx::Enum>())
  {
    auto& e = static_cast<cxx::Enum&>(*node);

    for (int i(item->childCount()); i < e.values.size(); ++i)
      item->addChild(createItem(e.values.at(i)));
  }

  for (int i(0); i < item->childCount(); ++i)
    fetchNewNodes(item->child(i));
}

void ModuleTreeWidget::fillTreeWidget(const MGProjectPtr & pro)
{
  clear();

  for (const auto & item : pro->modules)
    fill(invisibleRootItem(), item);
}

QTreeWidgetItem* ModuleTreeWidget::createItem(const std::shared_ptr<cxx::Entity> & node)
{
  QTreeWidgetItem *item = new QTreeWidgetItem(QStringList{ display(*node) });
  item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEditable);
  m_nodes_map[item] = node;

  if (node->is<cxx::Namespace>())
  {
    item->setIcon(0, QIcon(":/assets/namespace.png"));
    auto& ns = static_cast<cxx::Namespace&>(*node);
    for (const auto & n : ns.entities)
      item->addChild(createItem(n));
  }
  else if (node->is<cxx::Class>())
  {
    item->setIcon(0, QIcon(":/assets/class.png"));
    auto& c = static_cast<cxx::Class&>(*node);
    for (const auto & n : c.members)
      item->addChild(createItem(n));
  }
  else if (node->is<cxx::Enum>())
  {
    item->setIcon(0, QIcon(":/assets/enum.png"));
    auto& enm = static_cast<cxx::Enum&>(*node);
    for (const auto & n : enm.values)
      item->addChild(createItem(n));
  }
  else if (node->is<cxx::EnumValue>())
  {
    item->setIcon(0, QIcon(":/assets/enumerator.png"));
  }
  else if (node->is<cxx::Function>())
  {
    item->setIcon(0, QIcon(":/assets/func.png"));
  }

  handle_checkboxes(item, mShowCheckboxes);
  return item;
}

QTreeWidgetItem* ModuleTreeWidget::createItem(const MGModulePtr& node)
{
  QTreeWidgetItem* item = new QTreeWidgetItem(QStringList{ display(*node) });
  item->setFlags(item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsSelectable | Qt::ItemIsEditable);

  item->setIcon(0, QIcon(":/assets/namespace.png"));

  for (const auto& n : node->entities)
    item->addChild(createItem(n));

  handle_checkboxes(item, mShowCheckboxes);
  return item;
}

void ModuleTreeWidget::fill(QTreeWidgetItem *parent, const std::shared_ptr<cxx::Entity> & node)
{
  QTreeWidgetItem *item = createItem(node);
  parent->addChild(item);
}

void ModuleTreeWidget::fill(QTreeWidgetItem* parent, const MGModulePtr& node)
{
  QTreeWidgetItem* item = createItem(node);
  parent->addChild(item);
}

void ModuleTreeWidget::updateItem(QTreeWidgetItem *item, int column)
{
  updateCheckState(item);
}

void ModuleTreeWidget::updateCheckState(QTreeWidgetItem *item)
{
  auto root = invisibleRootItem();
  if (item->parent() != root && item->parent() != nullptr)
  {
    if (item->parent()->childCount() == 1)
      item->parent()->setCheckState(0, item->checkState(0));
    else
    {
      bool one_checked = false;
      bool one_unchecked = false;
      for (int i(0); i < item->parent()->childCount(); ++i)
      {
        if (item->parent()->child(i)->checkState(0) == Qt::Checked)
          one_checked = true;
        else if (item->parent()->child(i)->checkState(0) == Qt::Unchecked)
          one_unchecked = true;
        else if (item->parent()->child(i)->checkState(0) == Qt::PartiallyChecked)
        {
          one_checked = true, one_unchecked = true;
          break;
        }
      }

      if (one_checked && one_unchecked)
        item->parent()->setCheckState(0, Qt::PartiallyChecked);
      else if (one_checked)
        item->parent()->setCheckState(0, Qt::Checked);
      else
        item->parent()->setCheckState(0, Qt::Unchecked);

    }
  }

  if (item->checkState(0) == Qt::Unchecked)
  {
    for (int i(0); i < item->childCount(); ++i)
      item->child(i)->setCheckState(0, Qt::Unchecked);
  }
  else if (item->checkState(0) == Qt::Checked)
  {
    for (int i(0); i < item->childCount(); ++i)
      item->child(i)->setCheckState(0, Qt::Checked);
  }
}

void ModuleTreeWidget::resizeColumnsAuto()
{
  //for (int i(0); i < columnCount(); ++i)
  //  resizeColumnToContents(i);
}

void ModuleTreeWidget::displayContextMenu(const QPoint & p)
{
  QTreeWidgetItem *item = itemAt(p);
  if (item == nullptr)
    return;

  auto node = getEntity(item);
  if (node == nullptr)
    return;

  bool is_class = node->is<cxx::Class>();
  bool is_function = node->is<cxx::Function>();

  mAddCopyCtorAction->setVisible(is_class);
  mAddDestructorAction->setVisible(is_class);
  mAddAssignmentAction->setVisible(is_class);

  QAction *act = mMenu->exec(this->mapToGlobal(p));
  execAction(item, node, act);
}

std::shared_ptr<cxx::Entity> ModuleTreeWidget::getEntity(QTreeWidgetItem *item) const
{
  auto it = m_nodes_map.find(item);
  return it != m_nodes_map.end() ? it->second : nullptr;
}

MGModulePtr ModuleTreeWidget::getModule(QTreeWidgetItem* item) const
{
  if (!isModuleItem(item))
    return nullptr;
  return mProject->modules.at(invisibleRootItem()->indexOfChild(item));
}

void ModuleTreeWidget::createContextMenus()
{
  mMenu = new QMenu(this);

  mAddCopyCtorAction = mMenu->addAction("Add copy constructor");
  mAddDestructorAction = mMenu->addAction("Add destructor");
  mAddAssignmentAction = mMenu->addAction("Add assignment");
}

void ModuleTreeWidget::execAction(QTreeWidgetItem *item, std::shared_ptr<cxx::Entity> node, QAction *act)
{
  if (node->is<cxx::Class>())
  {
    auto& cla = static_cast<cxx::Class&>(*node);
    
    if (act == mAddCopyCtorAction)
    {
      auto ctor = std::make_shared<cxx::Function>(cla.name);
      ctor->kind = cxx::FunctionKind::Constructor;
      auto p = std::make_shared<cxx::FunctionParameter>(cxx::Type("const " + cla.name + " &"), "other");
      ctor->parameters.push_back(p);
      Controller::Instance().projectController().insert(ctor, node);
    }
    else if (act == mAddDestructorAction)
    {
      auto dtor = std::make_shared<cxx::Function>("~" + cla.name);
      dtor->kind = cxx::FunctionKind::Destructor;
      Controller::Instance().projectController().insert(dtor, node);
    }
    else if (act == mAddAssignmentAction)
    {
      auto assign = std::make_shared<cxx::Function>("operator=");
      assign->return_type = cxx::Type(cla.name + " &");
      auto p = std::make_shared<cxx::FunctionParameter>(cxx::Type("const " + cla.name + " &"), "other");
      assign->parameters.push_back(p);
      Controller::Instance().projectController().insert(assign, node);
    }
  }

  fetchNewNodes(item);
}