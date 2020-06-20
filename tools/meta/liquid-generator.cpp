// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "liquid-generator.h"

#include "gen/cppfile.h"
#include "gen/format.h"

#include "project/statement.h"
#include "project/node-visitor.h"

#include <json-toolkit/stringify.h>

#include <QDir>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonObject>
#include <QTextStream>

#include <QDebug>

#include <stdexcept>

class RecursiveTypeCollector : public NodeVisitor
{
public:

  ProjectRef project;
  json::Array result;

  RecursiveTypeCollector(ProjectRef p)
    : project(p)
  {

  }

  json::Object serialize(const Type& t)
  {
    json::Object obj;
    obj["name"] = t.name.toStdString();
    obj["id"] = t.id.toStdString();
    obj["enum"] = t.is_enum;
    obj["class"] = t.is_class;
    return obj;
  }

  json::Array operator()(Module& m)
  {
    for (auto e : m.elements)
      e->accept(*this);
    return result;
  }

  void visit(Class& c) override
  {
    for (auto e : c.elements)
      e->accept(*this);

    if (c.type_id != -1)
    {
      auto t = project->getType(c.type_id);
      result.push(serialize(*t));
    }
  }

  void visit(Module& m) override
  {

  }

  void visit(Enum& e) override
  {
    if (e.type_id != -1)
    {
      auto t = project->getType(e.type_id);
      result.push(serialize(*t));
    }
  }

  void visit(Enumerator& e) override
  {

  }

  void visit(File& f) override
  {
    for (auto e : f.elements)
      e->accept(*this);
  }

  void visit(Function& f) override
  {

  }

  void visit(Namespace& n) override
  {
    for (auto e : n.elements)
      e->accept(*this);
  }

  void visit(Statement& s)
  {

  }
};


class ProjectSerializer : public NodeVisitor
{
public:

  ProjectRef project;
  SerializationMaps& map;
  json::Json result;

  ProjectSerializer(ProjectRef pro, SerializationMaps& m)
    : project(pro), map(m)
  {

  }

  json::Json serialize(Node& n)
  {
    ProjectSerializer s{ project, map };
    n.accept(s);
    return s.result;
  }

  json::Array serialize(const QList<NodeRef>& nodes)
  {
    json::Array ret;

    json::Json default_constructed;

    for (auto n : nodes)
    {
      json::Json obj = serialize(*n);
      if (obj != default_constructed)
        ret.push(obj);
    }

    return ret;
  }

  json::Json serialize()
  {
    json::Json result;

    json::Array modules;

    for (auto m : project->modules)
      modules.push(serialize(*m));

    result["modules"] = modules;

    return result;
  }

  void visit(Class& c) override
  {
    map.bind(c.shared_from_this(), result);

    result["type"] = "class";
    result["name"] = c.name.toStdString();
    
    if (!c.base.isEmpty())
      result["base"] = c.base.toStdString();

    if (c.isFinal)
      result["final"] = true;

    if (!c.elements.isEmpty())
      result["members"] = serialize(c.elements);
  }

  void visit(Module& m) override
  {
    map.bind(m.shared_from_this(), result);

    result["type"] = "module";
    result["name"] = m.name.toStdString();

    if (!m.elements.isEmpty())
    {
      json::Array elements;

      for (auto f : m.elements)
      {
        for (auto e : static_cast<File&>(*f).elements)
          elements.push(serialize(*e));
      }

      result["elements"] = elements;
    }

    RecursiveTypeCollector collector{ project };
    result["types"] = collector(m);
  }

  void visit(Enum& e) override
  {
    map.bind(e.shared_from_this(), result);

    result["type"] = "enum";
    result["name"] = e.name.toStdString();

    json::Array values;

    for (auto ev : e.enumerators)
      values.push(serialize(*ev));

    result["values"] = values;
  }

  void visit(Enumerator& e) override
  {
    map.bind(e.shared_from_this(), result);

    result["type"] = "enumerator";
    result["name"] = e.name.toStdString();
  }

  void visit(File& f) override
  {

  }

  void visit(Function& f) override
  {
    map.bind(f.shared_from_this(), result);

    result["type"] = "function";
    result["name"] = f.name.toStdString();
    
    if (!f.isConstructor && !f.isDestructor)
      result["return_type"] = f.returnType.toStdString();

    if (f.isConstructor)
      result["constructor"] = true;
    else if (f.isDestructor)
      result["destructor"] = true;
    else if (f.isOperator())
      result["operator"] = true;

    json::Array parameters;
    for (auto p : f.parameters)
    {
      parameters.push(p.toStdString());
    }
    result["parameters"] = parameters;

    if (f.isConst)
      result["const"] = true;

    if (f.isExplicit)
      result["explicit"] = true;

    if (f.isStatic)
      result["static"] = true;

    if (f.bindingMethod != Function::BindingMethod::AutoBinding)
    {
      result["binding"] = Function::serialize(f.bindingMethod).toStdString();

      if (f.bindingMethod == Function::GenWrapperBinding)
        result["genwrapper"] = true;
    }

    if (!f.implementation.isEmpty())
      result["implementation"] = f.implementation.toStdString();
  }

  void visit(Namespace& n) override
  {
    map.bind(n.shared_from_this(), result);

    result["type"] = "namespace";
    result["name"] = n.name.toStdString();

    if (!n.elements.isEmpty())
      result["elements"] = serialize(n.elements);
  }

  void visit(Statement& s)
  {

  }
};

LiquidGenerator::TypeInfo::TypeInfo(const Type & t)
  : Type(t)
{

}


LiquidGenerator::LiquidGenerator(const QString & dir)
  : mRootDirectory(dir)
{

}

void LiquidGenerator::generate(const ProjectRef & p)
{
  QElapsedTimer timer;
  timer.start();

  mProject = p;

  ProjectSerializer serializer{ mProject, m_serialization_map };
  mSerializedProject = serializer.serialize();

  {
    QFile pro_json{ "project.json" };
    pro_json.open(QIODevice::WriteOnly | QIODevice::Truncate);
    pro_json.write(json::stringify(mSerializedProject).c_str());
  }

  buildTypeInfo();

  {
    QDir dir{ mRootDirectory };
    if (!dir.exists("plugins"))
      dir.mkdir("plugins");
  }

  parseTemplates();

  QDirIterator diriterator{ mRootDirectory + "/plugins", QDir::Files, QDirIterator::Subdirectories };

  while (diriterator.hasNext())
  {
    diriterator.next();

    QFileInfo item_info = diriterator.fileInfo();

    if (item_info.suffix() == "cpp" || item_info.suffix() == "h")
    {
      processFile(item_info);

      if (mProgressCallback)
      {
        if (!mProgressCallback(item_info.absoluteFilePath()))
          return;
      }
    }
  }

  auto elapsed = timer.elapsed();

  for (const QString& t : unsupported_types)
  {
    qDebug() << t;
  }

  qDebug() << "Generation done in" << elapsed;
}

int LiquidGenerator::numberOfFiles() const
{
  int result = 0;

  QDirIterator diriterator{ mRootDirectory + "/plugins", QDir::Files, QDirIterator::Subdirectories };

  while (diriterator.hasNext())
  {
    diriterator.next();

    QFileInfo item_info = diriterator.fileInfo();

    if (item_info.suffix() == "cpp" || item_info.suffix() == "h")
    {
      ++result;
    }
  }

  return result;
}

QString LiquidGenerator::to_snake_case(const QString & camelCaseName)
{
  QString result;

  for (const auto & c : camelCaseName)
  {
    if (c.isUpper())
    {
      result += "_";
      result += c.toLower();
    }
    else
    {
      result += c;
    }
  }

  if (result.startsWith("_"))
    result.remove(0, 1);
  if (result.startsWith("q_"))
    result.remove(0, 2);
  if (result == "class" || result == "char")
    result += '_';

  return result;
}

LiquidGenerator::OperatorSymbol LiquidGenerator::getOperatorSymbol(const QString & str)
{
  static const QMap<QString, OperatorSymbol> symbols = {
    {"operator=", Assign},
  { "operator==", Eq },
  { "operator!=", Neq },
  { "operator<", Less },
  { "operator<=", Leq },
  { "operator>", Greater },
  { "operator>=", Geq },
  { "operator+", Plus },
  { "operator-", Minus },
  { "operator*", Mul },
  { "operator/", Div },
  { "operator%", Modulo },
  { "operator+=", AddAssign },
  { "operator-=", SubAssign },
  { "operator*=", MulAssign },
  { "operator/=", DivAssign },
  { "operator&", And },
  { "operator|", Or },
  { "operator&=", AndAssign },
  { "operator|=", OrAssign },
  { "operator[]", Subscript },
  { "operator()", FunctionCall },
  { "operator++", PlusPlus },
  { "operator--", MinusMinus },
  { "operator<<", LeftShift },
  { "operator>>", RightShift },
  { "operator<<=", LeftShiftAssign },
  { "operator>>=", RightShiftAssign },
  { "operator^", Xor },
  { "operator^=", XorAssign },
  { "operator~", BitwiseNot },
  };

  return symbols.value(str, Invalid);
}

struct OperatorInfo {
  QString short_name;
  bool print_return_type;
  int nargs;
};

static const OperatorInfo static_operator_infos[]{
  { "invalid", false, 0 },
  { "assign", false, 2 },
  { "eq", false,  2},
  { "neq", false, 2 },
  { "less", false, 2 },
  { "leq", false, 2 },
  { "greater", false, 2 },
  { "geq", false, 2 },
  { "invalid", false, 0 }, // plus
  { "unary_plus", true, 1 },
  { "add", true, 2 },
  { "invalid", false, 0 }, // minus
  { "unary_minus", true, 1 },
  { "sub", true, 2 },
  { "mul", true, 2 },
  { "div", true, 2 },
  { "mod", true, 2 },
  { "add_assign", false, 2 },
  { "sub_assign", false, 2 },
  { "mul_assign", false, 2 },
  { "div_assign", false, 2 },
  { "bitand", true, 2 },
  { "bitor", true, 2 },
  { "and_assign", false, 2 },
  { "or_assign", false, 2 },
  { "subscript", true, 2 },
  { "call", true, -1 },
  { "invalid", false, 0 }, // plusplus
  { "pre_increment", true, 1 },
  { "post_increment", true, 1 },
  { "invalid", false, 0 }, // minusminus
  { "pre_decrement", true, 1 },
  { "post_decrement", true, 1 },
  { "left_shift", true, 2 },
  { "right_shift", true, 2 },
  { "left_shift_assign", false, 2 },
  { "right_shift_assign", false, 2 },
  { "bitxor", true, 2 },
  { "xor_assign", false, 2 },
  { "bitwise_not", true, 1 },
};

void LiquidGenerator::parseTemplates()
{
  QDirIterator diriterator{ mRootDirectory + "/templates", QDir::Files };

  while (diriterator.hasNext())
  {
    diriterator.next();

    QFileInfo item_info = diriterator.fileInfo();

    if (item_info.suffix() == "liquid")
    {
      liquid::Template tmplt = liquid::parseFile(item_info.absoluteFilePath().toStdString());
      tmplt.skipWhitespacesAfterTag();
      liquid::Renderer::templates()[item_info.baseName().toStdString()] = tmplt;

      qDebug() << "Parsed template: " << item_info.baseName();
    }
  }
}

void LiquidGenerator::processFile(const QFileInfo& fileinfo)
{
  QFile file{ fileinfo.absoluteFilePath() };

  if (!file.open(QIODevice::ReadOnly))
    return;

  std::string content = file.readAll().toStdString();

  file.close();

  std::string processed_content = renderSource(content);

  if (processed_content != content)
  {
    file.open(QIODevice::WriteOnly | QIODevice::Truncate);
    file.write(processed_content.c_str());
    file.close();
  }
}

std::string LiquidGenerator::renderSource(std::string src)
{
  size_t offset = src.find("#if METAGONK_SOURCE");

  while (offset != std::string::npos)
  {
    size_t src_begin = src.find('\n', offset) + 1;
    size_t src_end = src.find("#else", src_begin);

    std::string liquid_src{ src.begin() + src_begin, src.begin() + src_end };

    liquid::Template tmplt = liquid::parse(liquid_src);
    json::Object data;
    data["project"] = mSerializedProject;
    std::string liquid_rendered = liquid::Renderer::render(tmplt, data);

    size_t replacement_begin = src.find('\n', src_end) + 1;
    size_t replacement_end = src.find("#endif // METAGONK_SOURCE", replacement_begin);

    src.replace(src.begin() + replacement_begin, src.begin() + replacement_end, liquid_rendered);

    offset = src.find("#if METAGONK_SOURCE", src_begin);
  }

  return src;
}

json::Json LiquidGenerator::applyFilter(const std::string& name, const json::Json& object, const std::vector<json::Json>& args)
{
  if (name == "snake_case")
  {
    return to_snake_case(QString::fromStdString(object.toString())).toStdString();
  }
  else if (name == "replace")
  {
    QString str = QString::fromStdString(object.toString());
    str.replace(QString::fromStdString(args.at(0).toString()), QString::fromStdString(args.at(1).toString()));
    return str.toStdString();
  }
  else if (name == "append_if_not_empty")
  {
    QString str = QString::fromStdString(object.toString());

    if (!str.isEmpty())
    {
      QString other = QString::fromStdString(args.front().toString());
      str.append(other);
    }

    return str.toStdString();
  }
  else if (name == "join")
  {
    QString sep = QString::fromStdString(args.front().toString());

    json::Array list = object.toArray();
    QStringList strings;
    for (int i(0); i < list.length(); ++i)
    {
      strings.push_back(QString::fromStdString(list.at(i).toString()));
    }

    return strings.join(sep).toStdString();
  }
  else if (name == "qualified_name")
  {
    NodeRef node = m_serialization_map.get(object);
    return node->qualifiedName().toStdString();
  }
  else if (name == "typeid")
  {
    NodeRef node = m_serialization_map.get(object);
    if (node->is<Class>())
    {
      auto t = project()->getType(node->as<Class>().type_id);
      return t->id.toStdString();
    }
    else if (node->is<Enum>())
    {
      auto t = project()->getType(node->as<Enum>().type_id);
      return t->id.toStdString();
    }
    else
    {
      return "";
    }
  }
  else if (name == "get_symbol")
  {
    NodeRef node = mProject->getSymbol(QString::fromStdString(args.at(0).toString()), QString::fromStdString(args.at(1).toString()));
    return m_serialization_map.get(node);
  }
  else if (name == "get_module")
  {
    ModuleRef m = mProject->get<Module>(QString::fromStdString(args.at(0).toString()));
    return m_serialization_map.get(m);
  }
  else if (name == "parent")
  {
    NodeRef node = m_serialization_map.get(object);
    node = node->parent.lock();
    return node && ! node->is<File>() ? m_serialization_map.get(node) : json::null;
  }
  else if (name == "signature")
  {
    NodeRef node = m_serialization_map.get(object);
    FunctionRef fn = std::static_pointer_cast<Function>(node);
    return fn->signature().toStdString();
  }
  else if (name == "funaddr")
  {
    NodeRef node = m_serialization_map.get(object);
    FunctionRef fn = std::static_pointer_cast<Function>(node);
    return ("&" + (fn->implementation.isEmpty() ? (fn->qualifiedName()) : fn->implementation)).toStdString();
  }
  else if (name == "operator_name")
  {
    NodeRef node = m_serialization_map.get(object);
    FunctionRef fn = std::static_pointer_cast<Function>(node);
    return operatorName(*fn).toStdString();
  }
  else if (name == "operator_need_return_type")
  {
    NodeRef node = m_serialization_map.get(object);
    FunctionRef fn = std::static_pointer_cast<Function>(node);
    OperatorSymbol opsym = getOperatorSymbol(fn->name);

    static std::set<OperatorSymbol> assignment_ops = {
      Greater,
      Less,
      Leq, 
      Geq,
      Eq,
      Neq,
      Assign,
      AddAssign,
      SubAssign,
      MulAssign,
      DivAssign,
      AndAssign,
      OrAssign,
      PlusPlus,
      PreIncrement,
      PostIncrement,
      MinusMinus,
      PreDecrement,
      PostDecrement,
      LeftShiftAssign,
      RightShiftAssign,
      XorAssign,
    };

    return assignment_ops.find(opsym) == assignment_ops.end();
  }
  else if (name == "mangled_name")
  {
    NodeRef node = m_serialization_map.get(object);
    FunctionRef fn = std::static_pointer_cast<Function>(node);
    return mangledName(*fn).toStdString();
  }
  else if (name == "check_params")
  {
    NodeRef node = m_serialization_map.get(object);
    FunctionRef fn = std::static_pointer_cast<Function>(node);
    return isExposable(*fn);
  }
  else
  {
    return liquid::Renderer::applyFilter(name, object, args);
  }
}

QString LiquidGenerator::operatorName(const Function& fn) const
{
  OperatorSymbol op = getOperatorSymbol(fn.name);

  QString result = fn.parent.lock()->is<Class>() ? "memop_" : "op_";

  const bool is_member = fn.parent.lock()->is<Class>();
  const int implicit_param_count = is_member ? 1 : 0;

  if (op == Plus)
    op = fn.parameters.count() + implicit_param_count == 1 ? UnaryPlus : Add;
  else if (op == Minus)
    op = fn.parameters.count() + implicit_param_count == 1 ? UnaryMinus : Sub;
  else if (op == PlusPlus)
    op = fn.parameters.count() + implicit_param_count == 1 ? PreIncrement : PostIncrement;
  else if (op == MinusMinus)
    op = fn.parameters.count() + implicit_param_count == 1 ? PreDecrement : PostDecrement;

  if (op == FunctionCall)
  {
    return result + static_operator_infos[op].short_name;
  }
  else if (op == Subscript)
  {
    // operator[] cannot be non-member
    if (fn.isConst)
      return result + "const_subscript";
    else
      return result + "subscript";
  }
  else
  {
    if (op == LeftShift || op == RightShift)
    {
      if (!is_member)
      {
        if (fn.returnType == fn.parameters.at(0))
        {
          result += (op == LeftShift ? "put_to" : "read_from");
          return result;
        }
      }
      else
      {
        // TODO: check if not l- or r-shift
        result += (op == LeftShift ? "put_to" : "read_from");
        return result;
      }
    }

    return result + static_operator_infos[op].short_name;
  }
}

QString LiquidGenerator::mangledName(const Function& fun)
{
  QString ret = fun.name;

  if (fun.parent.lock() != nullptr && fun.parent.lock()->is<Class>())
    ret = fun.parent.lock()->name + "_" + ret;

  if (fun.isStatic)
    ret += "_static";

  for (QString p : fun.parameters)
  {
    p.remove("const");
    p.remove("&");
    p.remove("<");
    p.remove(">");
    p.remove(",");
    p.remove(":");
    p.remove(" ");

    ret += "_" + p;
  }

  return ret;
}

QString LiquidGenerator::computeWrapperName(const Function& fun)
{
  return fun.implementation.isEmpty() ? mangledName(fun) : fun.implementation;
}

bool LiquidGenerator::isExposable(const Function& fun)
{
  try
  {
    if (!fun.returnType.isEmpty() && !isExposed(fun.returnType))
      return false;
      

    for (const QString p : fun.parameters)
    {
      if (!isExposed(p))
        return false;
    }
    return true;
  }
  catch (...)
  {
    return false;
  }
}

bool LiquidGenerator::isExposed(QString t)
{
  bool is_const = false;
  bool is_ref = false;
  bool is_refref = false;

  if (t.startsWith("const "))
  {
    is_const = true;
    t.remove(0, 6);
  }
  if (t.endsWith("&&"))
  {
    is_refref = true;
    t.chop(2);
  }
  if (t.endsWith("&"))
  {
    is_ref = true;
    t.chop(1);
  }

  if (t.endsWith(" *"))
  {
    t.chop(2);
    t.append(QChar('*'));
  }

  t = t.simplified();

  if (t.isEmpty())
    return false;

  auto it = mTypeInfos.find(t);
  if (it == mTypeInfos.end())
  {
    unsupported_types.insert(t);
    return false;
  }
  return true;
}



QString LiquidGenerator::pluginDirectory() const
{
  return mRootDirectory + "/plugins";
}

void LiquidGenerator::buildTypeInfo()
{
  mTypeInfos.clear();

  for (const auto & t : mProject->types.fundamentals)
    mTypeInfos[t->name] = *t;

  for (const auto & t : mProject->types.enums)
    mTypeInfos[t->name] = *t;

  for (const auto & t : mProject->types.classes)
    mTypeInfos[t->name] = *t;
}

LiquidGenerator::TypeInfo & LiquidGenerator::typeinfo(const QString & t)
{
  if (t.isEmpty())
    throw UnsupportedType{ t };

  auto it = mTypeInfos.find(t);
  if (it == mTypeInfos.end())
  {
    unsupported_types.insert(t);
    throw UnsupportedType{ t };
  }
  return it.value();
}