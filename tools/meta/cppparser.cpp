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
static QtVersion gCurrentVersion = QtVersion{};

CppParser::CppParser(const ProjectRef & pro)
  : mProject(pro)
{

}

void CppParser::addIncludeDirectory(const QString & str)
{
  mIncludeDirectories.append(str);
}

struct RAIINodeGuard
{
  NodeRef& target;
  NodeRef value;

  explicit RAIINodeGuard(NodeRef& node)
    : target(node),
    value(node)
  {

  }

  ~RAIINodeGuard()
  {
    target = value;
  }
};

struct FileVisitor
{
  ProjectRef project;
  ModuleRef curModule;
  NodeRef curNode;

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

    NodeRef inner;

    if (curNode->is<Class>())
    {
      Class& cla = static_cast<Class&>(*curNode);
      inner = cla.get<Class>(QString::fromStdString(decl.class_->name));
    }
    else if(curNode->is<Namespace>())
    {
      Namespace& ns = static_cast<Namespace&>(*curNode);
      inner = ns.get<Class>(QString::fromStdString(decl.class_->name));
    }

    if(inner)
    {
      RAIINodeGuard guard{ curNode };
      curNode = inner;

      visitAll(decl.declarations);
    }
  }

  void visit(const cxx::EnumDeclaration& decl)
  {
    EnumRef enm;

    if (curNode->is<Class>())
    {
      Class& cla = static_cast<Class&>(*curNode);
      enm = cla.get<Enum>(QString::fromStdString(decl.enum_->name));
    }
    else if (curNode->is<Namespace>())
    {
      Namespace& ns = static_cast<Namespace&>(*curNode);
      enm = ns.get<Enum>(QString::fromStdString(decl.enum_->name));
    }

    for (auto val : decl.enum_->values)
    {
      enm->enumerators.append(EnumeratorRef::create(QString::fromStdString(val->name)));
    }
  }

  void visit(const cxx::FunctionDeclaration& decl)
  {
    if (decl.isForwardDeclaration())
      return;

    cxx::Function& cxxfunc = *(decl.function);

    FunctionRef func = nullptr;


    if (curNode->is<Class>())
    {
      Class& cla = static_cast<Class&>(*curNode);

      if (cxxfunc.isConstructor())
      {
        func = cla.add<Constructor>(QString::fromStdString(cxxfunc.name));
      }
      else if (cxxfunc.isDestructor())
      {
        func = cla.add<Destructor>(QString::fromStdString(cxxfunc.name));
      }
      else
      {
        func = cla.add<Function>(QString::fromStdString(cxxfunc.name));
        func->isConst = cxxfunc.isConst();
        func->isStatic = cxxfunc.isStatic();
      }
    }
    else if (curNode->is<Namespace>())
    {
      Namespace& ns = static_cast<Namespace&>(*curNode);
      func = ns.add<Function>(QString::fromStdString(cxxfunc.name));
    }

    func->returnType = QString::fromStdString(cxxfunc.return_type.toString());

    for (auto p : cxxfunc.parameters)
    {
      func->parameters.push_back(QString::fromStdString(p->type.toString()));
    }
  }

  void visit(const cxx::File& file)
  {
    QFileInfo fileinfo{ QString::fromStdString(file.path()) };

    auto project_file = FileRef::create(fileinfo.baseName());
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

