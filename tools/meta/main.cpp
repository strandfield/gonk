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

  Node::registerDeserializer("class", Class::fromJson);
  Node::registerDeserializer("enum", Enum::fromJson);
  Node::registerDeserializer("file", File::fromJson);
  Node::registerDeserializer("function", Function::fromJson);
  Node::registerDeserializer("constructor", Constructor::fromJson);
  Node::registerDeserializer("destructor", Destructor::fromJson);
  Node::registerDeserializer("module", Module::fromJson);
  Node::registerDeserializer("namespace", Namespace::fromJson);
  Node::registerDeserializer("statement", Statement::fromJson);

  MainWindow w;
  w.show();

  return app.exec();
}