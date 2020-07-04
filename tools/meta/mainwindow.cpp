// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include "controller.h"
#include "liquid-generator.h"

#include "dialogs/newtypedialog.h"

#include "widgets/moduletreewidget.h"
#include "widgets/typetreewidget.h"

#include "wizards/importwizard.h"

#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressDialog>
#include <QSettings>
#include <QTabWidget>

#include <QSqlDatabase>
#include <QSqlError>

#include <QDebug>

MainWindow::MainWindow()
{
  m_controller = new Controller(this);

  mSettings = new QSettings("settings.ini", QSettings::IniFormat, this);

  //if (mSettings->contains("lastproject"))
  //{
  //  mProject = Project::load(mSettings->value("lastproject").toString());
  //  if (mProject == nullptr)
  //    mSettings->remove("lastproject");
  //}
  
  if (mProject == nullptr)
    mProject = std::make_shared<MGProject>();

  mTabWidget = new QTabWidget();

  mTypeTreeWidget = new TypeTreeWidget(mProject);
  mModuleTreeWidget = new ModuleTreeWidget(mProject);

  mTabWidget->addTab(mTypeTreeWidget, "Types");
  mTabWidget->addTab(mModuleTreeWidget, "Modules");

  setCentralWidget(mTabWidget);

  menuBar()->addAction("Open", this, SLOT(openProject()));
  menuBar()->addAction("Build DB", this, SLOT(buildDB()));
  menuBar()->addAction("Export DB", this, SLOT(exportDB()));
  menuBar()->addAction("New type", this, SLOT(createNewType()));
  menuBar()->addAction("Import", this, SLOT(importCpp()));
  menuBar()->addAction("Generate", this, SLOT(generateBinding()));
}

void MainWindow::openProject()
{
  QString path = QFileDialog::getOpenFileName(this, "Open project", QString(), QString("metagonk database (*.db; *.sql)"));
  if (path.isEmpty())
    return;

  QFileInfo fileinfo{ path };

  if (fileinfo.suffix() == "db")
  {
    if (!m_controller->loadDatabase(fileinfo))
    {
      QMessageBox::warning(nullptr, QObject::tr("Cannot open database"),
        m_controller->database().lastError().text(), QMessageBox::Ok);

      return;
    }

    m_controller->loadProject();
    mProject = m_controller->project();
  }
  else
  {
    QMessageBox::information(this, "Open project", "Failed to open project", QMessageBox::Ok);
    return;
  }

  if (mProject)
  {
    mSettings->setValue("lastproject", path);

    mTypeTreeWidget->setProject(mProject);
    mModuleTreeWidget->setProject(mProject);
  }
  else
  {
    QMessageBox::information(this, "Open project", "Failed to open project", QMessageBox::Ok); return;
  }
}

void MainWindow::buildDB()
{
  QString path = QFileDialog::getExistingDirectory(this, "Selected database directory");
  if (path.isEmpty())
    return;

  QFileInfo fileinfo{ path };

  if (fileinfo.exists())
  {
    QString savepath = QFileDialog::getSaveFileName(this, "Save database", QString(), QString("metagonk database (*.db)"));

    if (savepath.isEmpty())
      return;

    if (!m_controller->createSqlDatabase(fileinfo, savepath))
    {
      QMessageBox::warning(nullptr, QObject::tr("Cannot create database"),
        m_controller->database().lastError().text(), QMessageBox::Ok);

      return;
    }

    m_controller->loadProject();
    mProject = m_controller->project();
  }

  if (mProject)
  {
    mSettings->setValue("lastproject", path);

    mTypeTreeWidget->setProject(mProject);
    mModuleTreeWidget->setProject(mProject);
  }
  else
  {
    QMessageBox::information(this, "Open project", "Failed to open project", QMessageBox::Ok); return;
  }
}

void MainWindow::exportDB()
{
  if (m_controller->project() == nullptr)
    return;

  m_controller->exportDatabase();
}

void MainWindow::createNewType()
{
  auto *dialog = new NewTypeDialog(this);
  int result = dialog->exec();
  dialog->deleteLater();
  if (result != QDialog::Accepted)
    return;

  auto t = dialog->getType();

  mProject->types.push_back(t);

  mTypeTreeWidget->fetchNewNodes();
}

void MainWindow::importCpp()
{
  auto *wizard = new ImportWizard(mProject);
  wizard->exec();
  wizard->deleteLater();

  mTypeTreeWidget->fetchNewNodes();
  mModuleTreeWidget->fetchNewNodes();
}

void MainWindow::generateBinding()
{
  QString path = mSettings->value("generatedir").toString();
  if (path.isEmpty())
    path = QFileDialog::getExistingDirectory(this, "Save directory");

  if (path.isEmpty())
    return;

  mSettings->setValue("generatedir", path);

  LiquidGenerator gen{ path };

  QProgressDialog progress("Generating files...", "Abort", 0, gen.numberOfFiles(), this);
  progress.setWindowTitle("Binding generation");
  progress.setWindowModality(Qt::WindowModal);
  progress.setMinimumDuration(2000);

  gen.setProgressCallback([&progress](const QString& filename) -> bool {
    progress.setValue(progress.value() + 1);
    return !progress.wasCanceled();
    });
  gen.generate(mProject);
}

void MainWindow::showEvent(QShowEvent *e)
{
  if (mSettings->contains("geometry"))
    restoreGeometry(mSettings->value("geometry").toByteArray());

  if (mSettings->contains("windowState"))
    restoreState(mSettings->value("windowState").toByteArray());

  QMainWindow::showEvent(e);
}

void MainWindow::closeEvent(QCloseEvent *e)
{
  mSettings->setValue("geometry", saveGeometry());
  mSettings->setValue("windowState", saveState());
  QMainWindow::closeEvent(e);
}
