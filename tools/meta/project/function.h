// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_FUNCTION_H
#define METAGONK_FUNCTION_H

#include "project/node.h"

class Function : public Node
{
public:
  enum BindingMethod{
    FirstBindingMethod = 0,
    AutoBinding = FirstBindingMethod,
    SignalBinding,
    ChainableBinding,
    ReferenceBinding,
    StaticVoidBinding,
    ConstVoidBinding,
    VoidBinding,
    StaticBinding,
    SimpleBinding,
    ConstructorBinding,
    DestructorBinding,
    OperatorBinding,
    FreeFunctionBinding,
    FreeFunctionAsStaticBinding,
    NewFunctionBinding,
    LastBindingMethod = NewFunctionBinding,
  };

public:
  int function_id = -1;
  QString rename;
  QString returnType;
  QStringList parameters;
  bool isExplicit;
  bool isStatic;
  bool isConst;
  bool isDeleted;
  BindingMethod bindingMethod;
  QStringList defaultArguments;

public:
  Function(const QString & n, Qt::CheckState c = Qt::Checked);
  ~Function() = default;

  QString typeName() const override { return "function"; }
  static const NodeType staticTypeCode = NodeType::Function;
  NodeType typeCode() const override { return staticTypeCode; }

  QString display() const override;

  int compareTo(const Node & o) const;

  QStringList getSpecifiers() const;
  void setSpecifiers(const QStringList & specs);

  inline static QString serialize(BindingMethod bm)
  {
    switch (bm)
    {
    case Function::AutoBinding:
      break;
    case Function::SignalBinding:
      return "signals";
    case Function::ChainableBinding:
      return "chainable";
    case Function::ReferenceBinding:
      return "reference";
    case Function::StaticVoidBinding:
      return "static-void";
    case Function::ConstVoidBinding:
      return "const-void";
    case Function::VoidBinding:
      return "void";
    case Function::StaticBinding:
      return "static";
    case Function::SimpleBinding:
      return "simple";
    case Function::ConstructorBinding:
      return "ctor";
    case Function::DestructorBinding:
      return "dtor";
    case Function::OperatorBinding:
      return "op";
    case Function::FreeFunctionBinding:
      return "free";
    case Function::FreeFunctionAsStaticBinding:
      return "freestatic";
    case Function::NewFunctionBinding:
      return "newfunction";
    default:
      break;
    }

    return "auto";
  }

  template<typename T>
  static T deserialize(const QString & str);

  template<>
  static BindingMethod deserialize<BindingMethod>(const QString & str)
  {
    if (str == "signals")
      return SignalBinding;
    else if (str == "chainable")
      return ChainableBinding;
    else if (str == "reference")
      return ReferenceBinding;
    else if (str == "static-void")
      return StaticVoidBinding;
    else if (str == "const-void")
      return ConstVoidBinding;
    else if (str == "void")
      return VoidBinding;
    else if (str == "static")
      return StaticBinding;
    else if (str == "simple")
      return SimpleBinding;
    else if (str == "ctor")
      return ConstructorBinding;
    else if (str == "dtor")
      return DestructorBinding;
    else if (str == "op")
      return OperatorBinding;
    else if (str == "free")
      return FreeFunctionBinding;
    else if (str == "freestatic")
      return FreeFunctionAsStaticBinding;
    else if (str == "newfunction")
      return NewFunctionBinding;
    return AutoBinding;
  }
};
typedef std::shared_ptr<Function> FunctionRef;


struct Constructor : public Function
{
  Constructor(const QString & n, Qt::CheckState cs = Qt::Checked);
  ~Constructor() = default;

  QString typeName() const override { return "constructor"; }
  static const NodeType staticTypeCode = NodeType::Constructor;
  NodeType typeCode() const override { return staticTypeCode; }

  QString display() const override;

};
typedef std::shared_ptr<Constructor> ConstructorRef;

struct Destructor : public Function
{
  Destructor(const QString & n, Qt::CheckState cs = Qt::Checked);
  ~Destructor() = default;

  QString typeName() const override { return "destructor"; }
  static const NodeType staticTypeCode = NodeType::Destructor;
  NodeType typeCode() const override { return staticTypeCode; }

  QString display() const override;

};
typedef std::shared_ptr<Destructor> DestructorRef;

#endif // METAGONK_FUNCTION_H
