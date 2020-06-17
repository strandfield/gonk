// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_MAINWINDOW_H
#define METAGONK_MAINWINDOW_H

#include <QMainWindow>

#include "project.h"

class Controller;

class ModuleTreeWidget;
class TypeTreeWidget;

class QFileInfo;
class QSettings;
class QTabWidget;

class MainWindow : public QMainWindow
{
  Q_OBJECT
public:
  explicit MainWindow();
  ~MainWindow() = default;

protected Q_SLOTS:
  void openProject();
  void buildDB();
  void createNewType();
  void importCpp();
  void generateBinding();
  void generateBinding2();

protected:
  void showEvent(QShowEvent *e);
  void closeEvent(QCloseEvent *e);

private:
  Controller *m_controller;
  ProjectRef mProject;
  QSettings *mSettings;

  QTabWidget *mTabWidget;
  TypeTreeWidget *mTypeTreeWidget;
  ModuleTreeWidget *mModuleTreeWidget;
};

#endif // METAGONK_MAINWINDOW_H
