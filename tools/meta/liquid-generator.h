// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_LIQUIDGENERATOR_H
#define METAGONK_LIQUIDGENERATOR_H

#include "project.h"
#include "project/class.h"
#include "project/enum.h"
#include "project/file.h"
#include "project/function.h"

#include <liquid/renderer.h>

#include <QMap>
#include <QSet>
#include <QStack>

#include <functional>
#include <map>
#include <set>

class CppFile;

class QFileInfo;
class QTextStream;

struct SerializationMaps
{
  std::unordered_map<std::shared_ptr<json::details::Node>, NodeRef> backward;
  std::unordered_map<NodeRef, json::Json> forward;

  json::Json get(const NodeRef& n) const
  {
    return forward.at(n);
  }

  NodeRef get(const json::Json& obj) const
  {
    return backward.at(obj.impl());
  }

  void bind(const NodeRef& n, const json::Json& o)
  {
    backward[o.impl()] = n;
    forward[n] = o;
  }
};

class LiquidGenerator : protected liquid::Renderer
{
public:
  LiquidGenerator(const QString & dir);

  void generate(const ProjectRef & pro);

  const std::function<bool(const QString &)> & progressCallback() const { return mProgressCallback; }
  void setProgressCallback(std::function<bool(const QString &)> f) { mProgressCallback = f; }

  inline const ProjectRef & project() const { return mProject; }

  struct TypeInfo : Type
  {
    TypeInfo() = default;
    TypeInfo(const Type & t);
  };

  int numberOfFiles() const;

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

  void parseTemplates();

  void processFile(const QFileInfo& fileinfo);

  std::string renderSource(std::string src);

  json::Json applyFilter(const std::string& name, const json::Json& object, const std::vector<json::Json>& args) override;

  QString operatorName(const Function& fn) const;

  static QString mangledName(const Function& fun);
  static QString computeWrapperName(const Function& fun);

  bool isExposable(const Function& fun);
  bool isExposed(QString t);

private:

  QString pluginDirectory() const;

private:
  struct UnsupportedType { QString name; };

  void buildTypeInfo();
  TypeInfo & typeinfo(const QString & t);

private:
  friend class LiquidGenStateGuard;
  ProjectRef mProject;
  QMap<QString, TypeInfo> mTypeInfos;

  json::Json mSerializedProject;
  SerializationMaps m_serialization_map;

  QString mRootDirectory;

public:
  std::set<QString> unsupported_types;

  std::function<bool(const QString &)> mProgressCallback;
};

#endif // METAGONK_LIQUIDGENERATOR_H
