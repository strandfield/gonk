// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "cppparser.h"

#include "project/class.h"
#include "project/enum.h"
#include "project/file.h"
#include "project/function.h"
#include "project/module.h"
#include "project/namespace.h"

#include <cxx/class-declaration.h>
#include <cxx/namespace-declaration.h>
#include <cxx/function-declaration.h>
#include <cxx/enum-declaration.h>
#include <cxx/filesystem.h>


#include <QDebug>
#include <QFileInfo>
#include <QQueue>

static QString gCurrentFile = QString{};

CppParser::CppParser(const ProjectRef & pro)
  : mProject(pro)
{

}

void CppParser::addIncludeDirectory(const QString & str)
{
  mIncludeDirectories.append(str);
}

struct FileVisitor
{
  ProjectRef project;
  ModuleRef curModule;
  NodeRef curNode;

  void fill(Function& f, const cxx::Function& cxxfunc)
  {
    f.returnType = QString::fromStdString(cxxfunc.return_type.toString());

    for (auto p : cxxfunc.parameters)
    {
      f.parameters.push_back(QString::fromStdString(p->type.toString()));
    }
  }

  void visit(const cxx::NamespaceDeclaration& decl)
  {
    Namespace& ns = dynamic_cast<Namespace&>(*curNode);

    auto inner = ns.getNamespace(QString::fromStdString(decl.namespace_->name));

    {
      RAIINodeGuard guard{ curNode };
      curNode = inner;

      visitAll(decl.declarations);
    }
  }

  void visit(const cxx::ClassDeclaration& decl)
  {
    if (decl.isForwardDeclaration())
      return;

    visit(*decl.class_);
  }

  void visit(const cxx::Class& cxxclass)
  {
    NodeRef inner;

    if (curNode->is<Class>())
    {
      Class& cla = static_cast<Class&>(*curNode);
      inner = cla.get<Class>(QString::fromStdString(cxxclass.name));
    }
    else if (curNode->is<Namespace>())
    {
      Namespace& ns = static_cast<Namespace&>(*curNode);
      inner = ns.get<Class>(QString::fromStdString(cxxclass.name));
    }

    if (inner)
    {
      RAIINodeGuard guard{ curNode };
      curNode = inner;

      for (const auto m : cxxclass.members)
        visitNode(*m);
    }
  }

  void visit(const cxx::EnumDeclaration& decl)
  {
    visit(*decl.enum_);
  }

  void visit(const cxx::Enum& cxxenum)
  {
    EnumRef enm;

    if (curNode->is<Class>())
    {
      Class& cla = static_cast<Class&>(*curNode);
      enm = cla.get<Enum>(QString::fromStdString(cxxenum.name));
    }
    else if (curNode->is<Namespace>())
    {
      Namespace& ns = static_cast<Namespace&>(*curNode);
      enm = ns.get<Enum>(QString::fromStdString(cxxenum.name));
    }

    for (auto val : cxxenum.values)
    {
      enm->enumerators.append(std::make_shared<Enumerator>(QString::fromStdString(val->name)));
    }
  }

  void visit(const cxx::FunctionDeclaration& decl)
  {
    if (decl.function->parent()->is<cxx::Class>())
      return;

    visit(*decl.function);
  }

  void visit(const cxx::Function& cxxfunc)
  {
    FunctionRef func = nullptr;

    if (curNode->is<Class>())
    {
      Class& cla = static_cast<Class&>(*curNode);

      func = cla.add<Function>(QString::fromStdString(cxxfunc.name));
      func->isConst = cxxfunc.isConst();
      func->isStatic = cxxfunc.isStatic();
      func->isConstructor = cxxfunc.isConstructor();
      func->isDestructor = cxxfunc.isDestructor();
    }
    else if (curNode->is<Namespace>())
    {
      Namespace& ns = static_cast<Namespace&>(*curNode);
      func = ns.add<Function>(QString::fromStdString(cxxfunc.name));
    }

    fill(*func, cxxfunc);
  }

  void visit(const cxx::File& file)
  {
    QFileInfo fileinfo{ QString::fromStdString(file.path()) };

    auto project_file = std::make_shared<File>(fileinfo.baseName());
    RAIINodeGuard guard{ curNode };
    curNode = project_file;

    for (auto n : file.nodes)
    {
      visitNode(*n);
    }

    curModule->elements.append(project_file);
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
    else if (n.is<cxx::Function>())
    {
      visit(static_cast<const cxx::Function&>(n));
    }
    else if (n.is<cxx::Class>())
    {
      visit(static_cast<const cxx::Class&>(n));
    }
    else if (n.is<cxx::Enum>())
    {
      visit(static_cast<const cxx::Enum&>(n));
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

void CppParser::parse(const QString & file, const ModuleRef & mo)
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

