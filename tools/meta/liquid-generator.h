// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef METAGONK_LIQUIDGENERATOR_H
#define METAGONK_LIQUIDGENERATOR_H

#include "project.h"

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
  std::unordered_map<std::shared_ptr<json::details::Node>, std::shared_ptr<cxx::Entity>> backward;
  std::unordered_map<std::shared_ptr<cxx::Entity>, json::Json> forward;

  std::unordered_map<std::shared_ptr<json::details::Node>, MGModulePtr> module_backward;
  std::unordered_map<MGModulePtr, json::Json> module_forward;

  json::Json get(const std::shared_ptr<cxx::Entity>& n) const
  {
    return forward.at(n);
  }

  std::shared_ptr<cxx::Entity> get(const json::Json& obj) const
  {
    return backward.at(obj.impl());
  }

  void bind(const std::shared_ptr<cxx::Entity>& n, const json::Json& o)
  {
    backward[o.impl()] = n;
    forward[n] = o;
  }

  json::Json get(const MGModulePtr& n) const
  {
    return module_forward.at(n);
  }

  MGModulePtr getModule(const json::Json& obj) const
  {
    return module_backward.at(obj.impl());
  }


  void bind(const MGModulePtr& n, const json::Json& o)
  {
    module_backward[o.impl()] = n;
    module_forward[n] = o;
  }
};

class LiquidGenerator : protected liquid::Renderer
{
public:
  LiquidGenerator(const QString & dir);

  void generate(const MGProjectPtr & pro);

  const std::function<bool(const QString &)> & progressCallback() const { return mProgressCallback; }
  void setProgressCallback(std::function<bool(const QString &)> f) { mProgressCallback = f; }

  inline const MGProjectPtr& project() const { return mProject; }

  struct TypeInfo : MGType
  {
    TypeInfo() = default;
    TypeInfo(const MGType& t);
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

  QString operatorName(const cxx::Function& fn) const;

  static QString mangledName(const cxx::Function& fun);
  static QString computeWrapperName(const cxx::Function& fun);

  bool isExposable(const cxx::Function& fun);
  bool isExposed(QString t);

private:

  QString pluginDirectory() const;

private:
  struct UnsupportedType { QString name; };

  void buildTypeInfo();
  TypeInfo & typeinfo(const QString & t);

private:
  friend class LiquidGenStateGuard;
  MGProjectPtr mProject;
  std::map<std::string, TypeInfo> mTypeInfos;

  json::Json mSerializedProject;
  SerializationMaps m_serialization_map;

  QString mRootDirectory;

public:
  std::set<QString> unsupported_types;

  std::function<bool(const QString &)> mProgressCallback;
};

#endif // METAGONK_LIQUIDGENERATOR_H
