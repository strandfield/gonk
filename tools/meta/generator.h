// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_GENERATOR_H
#define METAGONK_GENERATOR_H

#include "project.h"
#include "project/class.h"
#include "project/enum.h"
#include "project/file.h"
#include "project/function.h"

#include <QSet>
#include <QStack>

#include <functional>
#include <map>

class HeaderFile;
class SourceFile;

class QTextStream;

class Generator
{
public:
  Generator(const QString & dir);

  void generate(const ProjectRef & pro);

  const std::function<bool(const QString &)> & progressCallback() const { return mProgressCallback; }
  void setProgressCallback(std::function<bool(const QString &)> f) { mProgressCallback = f; }

  inline const ProjectRef & project() const { return mProject; }

  struct TypeInfo : Type
  {
    TypeInfo() = default;
    TypeInfo(const Type & t);
  };

private:
  static const QString endl;

  static const QString ClassBinderInclude;
  static const QString EnumBinderInclude;
  static const QString NamespaceBinderInclude;
  static const QString ClassBuilderInclude;
  static const QString EnumBuilderInclude;

private:
  static QString to_snake_case(const QString & camelCaseName);

  enum OperatorSymbol {
    Invalid = 0,
    Assign,
    Eq,
    Neq,
    Less,
    Leq,
    Greater,
    Geq,
    Plus,
    UnaryPlus,
    Add,
    Minus,
    UnaryMinus,
    Sub,
    Mul,
    Div,
    Modulo,
    AddAssign,
    SubAssign,
    MulAssign,
    DivAssign,
    And,
    Or,
    AndAssign,
    OrAssign,
    Subscript,
    FunctionCall,
    PlusPlus,
    PreIncrement,
    PostIncrement,
    MinusMinus,
    PreDecrement,
    PostDecrement,
    LeftShift,
    RightShift,
    LeftShiftAssign,
    RightShiftAssign,
    Xor,
    XorAssign,
    BitwiseNot,
  };

  static OperatorSymbol getOperatorSymbol(const QString & str);

private:

  void fetchTypesHeaders();
  void fetchTypesHeaders(NodeRef node);

  void generate(ModuleRef mod);
  void generateModuleDefsFile();
  void generateModuleFile();
  void generate(FileRef file);
  QString generate(FunctionRef fun);
  QString generate(FunctionRef fun, Function::BindingMethod bm);
  QString generateSignal(FunctionRef fun);
  QString generateOperator(FunctionRef fun, OperatorSymbol op);
  QString generateNewFunction(FunctionRef fn);
  void generate(ClassRef cla);
  void generate(EnumRef enm);
  void generate(NamespaceRef ns);
  void generate(NodeRef n);

  QString fparam(FunctionRef fun, int n);
  QString fparam(const QString & p);
  QString fparams(const QStringList & params, const QString & prefix = QString(""));
  QString fparams(const FunctionRef fun);
  QString fparamscomma(const FunctionRef fun);
  const QStringList & checkParams(const QStringList & ps);
  Function::BindingMethod getBindingMethod(FunctionRef fun) const;
  Function::BindingMethod guessBindingMethod(FunctionRef fun) const;

  typedef QPair<QString, QString> Link;
  typedef QList<Link> Links;
  static Links extractLinks(const QString & str);

  static QString fundisplay(FunctionRef fun);

private:
  QString prefix() const;
  QString enclosingEntity() const; // returns Namespace or Class
  QString enclosingName() const;
  QString enclosing_snake_name() const; // returns 'ns' if no enclosing, or the enclosing namespace/class name
  QString nameQualification() const;
  QString removeQualification(QString name) const;
  bool isMember() const;

  HeaderFile & currentHeader();
  SourceFile & currentSource();
  QString pluginDirectory() const;
  QString currentHeaderDirectory();
  QString currentSourceDirectory();

private:
  struct UnsupportedType { QString name; };

  void buildTypeInfo();
  TypeInfo & typeinfo(const QString & t);

private:
  friend class StateGuard;
  ProjectRef mProject;
  QMap<QString, TypeInfo> mTypeInfos;

  QString mRootDirectory;
  QStack<NodeRef> mProcessingStack;
  QString mCurrentModuleName;
  ModuleRef m_current_module;
  FileRef m_current_file;
  HeaderFile *mCurrentHeader;
  SourceFile *mCurrentSource;

  std::map<QString, QString> m_types_headers; // QByteArray -> bytearray.h
  std::vector<std::shared_ptr<Type>> m_generated_types;
  std::vector<QString> m_generated_files;

  std::function<bool(const QString &)> mProgressCallback;
};

#endif // METAGONK_GENERATOR_H
