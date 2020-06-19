// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "project/function.h"

#include "project/node-visitor.h"

#include <QJsonArray>

Function::Function(const QString & n, Qt::CheckState c)
  : Node(n, c)
  , isExplicit(false)
  , isStatic(false)
  , isConst(false)
  , isDeleted(false)
  , isConstructor(false)
  , isDestructor(false)
  , bindingMethod(Function::AutoBinding)
{

}

void Function::accept(NodeVisitor& visitor)
{
  visitor.visit(*this);
}

QString Function::display() const
{
  QString result = signature();

  if (bindingMethod != Function::AutoBinding)
    result += " [" + serialize(bindingMethod) + "]";

  if (!implementation.isEmpty())
    result += " [&" + implementation + "]";

  if (!condition.isEmpty())
    result += " [?" + condition + "]";

  return result;
}

QString Function::signature() const
{
  if (isConstructor)
  {
    QString result;
    if (isExplicit)
      result += "explicit ";
    result += name;
    result += "(";
    {
      QStringList params = parameters;
      for (int i(0); i < defaultArguments.size(); ++i)
        params[params.size() - i - 1] += " = " + defaultArguments.at(i);
      result += params.join(", ");
    }
    result += ")";

    if (isDeleted)
      result += " = delete";

    result += ";";

    return result;
  }
  else if (isDestructor)
  {
    QString result;

    result += name;
    result += "()";

    if (isDeleted)
      result += " = delete";

    result += ";";
    return result;
  }

  QString result;
  if (isExplicit)
    result += "explicit ";
  if (isStatic)
    result += "static ";

  result += returnType;
  result += " " + name;
  result += "(";
  {
    QStringList params = parameters;
    for (int i(0); i < defaultArguments.size(); ++i)
      params[params.size() - i - 1] += " = " + defaultArguments.at(i);
    result += params.join(", ");
  }
  result += ")";

  if (isConst)
    result += " const";

  if (isDeleted)
    result += " = delete";

  result += ";";

  return result;
}

bool Function::isOperator() const
{
  return this->name.startsWith("operator");
}

int Function::compareTo(const Node & o) const
{
  static const QString operator_str = "operator";

  const Function & other = dynamic_cast<const Function &>(o);
  
  {
    const bool self_is_operator = this->name.startsWith(operator_str);
    const bool other_is_operator = other.name.startsWith(operator_str);
    if (self_is_operator != other_is_operator)
      return self_is_operator ? 1 : -1;
  }

  int c = this->name.compare(other.name);
  if (c != 0)
    return c;

  if (this->parameters.size() < other.parameters.size())
    return -1;
  else if (this->parameters.size() > other.parameters.size())
    return 1;

  for (int i(0); i < this->parameters.size(); ++i)
  {
    c = this->parameters.at(i).compare(other.parameters.at(i));
    if (c != 0)
      return c;
  }

  c = this->returnType.compare(other.returnType);
  if (c != 0)
    return c;

  c = this->getSpecifiers().join(QChar()).compare(other.getSpecifiers().join(QChar()));
  return c;
}

QStringList Function::getSpecifiers() const
{
  QStringList specifiers;
  if (isConstructor)
    specifiers << "ctor";
  if (isDestructor)
    specifiers << "dtor";
  if (isConst)
    specifiers << "const";
  if (isDeleted)
    specifiers << "delete";
  if (isExplicit)
    specifiers << "explicit";
  if (isStatic)
    specifiers << "static";
  return specifiers;
}

void Function::setSpecifiers(const QStringList & specs)
{
  isConst = specs.contains("const");
  isDeleted = specs.contains("delete");
  isExplicit = specs.contains("explicit");
  isStatic = specs.contains("static");
  isConstructor = specs.contains("ctor");
  isDestructor = specs.contains("dtor");
}
