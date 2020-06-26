// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "cppparser.h"

#include <cxx/class-declaration.h>
#include <cxx/namespace-declaration.h>
#include <cxx/function-declaration.h>
#include <cxx/enum-declaration.h>
#include <cxx/filesystem.h>


#include <QDebug>
#include <QFileInfo>
#include <QQueue>

static QString gCurrentFile = QString{};

CppParser::CppParser(const MGProjectPtr& pro)
  : mProject(pro)
{

}

void CppParser::addIncludeDirectory(const QString & str)
{
  mIncludeDirectories.append(str);
}

struct FileVisitor
{
  MGProjectPtr project;
  MGModulePtr curModule;
  std::shared_ptr<cxx::Entity> curNode;

  void visit(const cxx::NamespaceDeclaration& decl)
  {
    if (!curNode)
    {
      curModule->entities.push_back(decl.namespace_);
    }

    {
      RAIICxxElemGuard guard{ curNode };
      curNode = decl.namespace_;

      visitAll(decl.declarations);
    }
  }

  void visit(const cxx::ClassDeclaration& decl)
  {
    if (decl.isForwardDeclaration())
      return;

    if (!curNode)
    {
      curModule->entities.push_back(decl.class_);
    }
  }

  void visit(const cxx::EnumDeclaration& decl)
  {
    if (!curNode)
    {
      curModule->entities.push_back(decl.enum_);
    }
  }

  void visit(const cxx::FunctionDeclaration& decl)
  {
    if (!curNode)
    {
      curModule->entities.push_back(decl.function);
    }
  }

  void visit(const cxx::File& file)
  {
    QFileInfo fileinfo{ QString::fromStdString(file.path()) };

    for (auto n : file.nodes)
    {
      visitNode(*n);
    }
  }

  void visitNode(const cxx::Node& n)
  {
    if (n.is<cxx::NamespaceDeclaration>())
    {
      visit(static_cast<const cxx::NamespaceDeclaration&>(n));
    }
    else if (n.is<cxx::ClassDeclaration>())
    {
      visit(static_cast<const cxx::ClassDeclaration&>(n));
    }
    else if (n.is<cxx::EnumDeclaration>())
    {
      visit(static_cast<const cxx::EnumDeclaration&>(n));
    }
    else if (n.is<cxx::FunctionDeclaration>())
    {
      visit(static_cast<const cxx::FunctionDeclaration&>(n));
    }
  }

  template<typename T>
  void visitAll(const std::vector<std::shared_ptr<T>>& nodes)
  {
    for (auto n : nodes)
    {
      visitNode(*n);
    }
  }
};

void CppParser::parse(const QString & file, const MGModulePtr & mo)
{
  mActiveModule = mo;

  m_parser.includedirs.clear();

  for (const QString inc : mIncludeDirectories)
    m_parser.includedirs.insert(inc.toStdString());

  std::string filepath = file.toStdString();
  m_parser.parse(filepath);

  auto cxxfile = cxx::FileSystem::GlobalInstance().get(filepath);

  FileVisitor visitor;
  visitor.project = project();
  visitor.curModule = mActiveModule;
  visitor.visit(*cxxfile);

  mActiveModule = nullptr;
}

