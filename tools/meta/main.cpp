// Copyright (C) 2018 Vincent Chambrin
// This file is part of the Yasl project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include "project/class.h"
#include "project/enum.h"
#include "project/file.h"
#include "project/function.h"
#include "project/module.h"
#include "project/namespace.h"
#include "project/statement.h"

#include <QApplication>

#include <QDebug>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  MainWindow w;
  w.show();

  return app.exec();
}