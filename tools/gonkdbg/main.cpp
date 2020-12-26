// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "mainwindow.h"

#include <QApplication>

#include <QIcon>

int main(int argc, char *argv[])
{
  QApplication app(argc, argv);

  QApplication::setApplicationName("gonkdbg");
  QApplication::setApplicationDisplayName("gonkdbg");
  // Qt does not support clipping in the Svg, will need to do a png out of it, sad...
  QApplication::setWindowIcon(QIcon(":/icon.svg"));

  MainWindow w;
  w.show();

  return app.exec();
}