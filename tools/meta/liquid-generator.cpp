// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "liquid-generator.h"

#include <cxx/class.h>
#include <cxx/enum.h>
#include <cxx/function.h>
#include <cxx/namespace.h>

#include <json-toolkit/stringify.h>

#include <QDir>
#include <QDirIterator>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonObject>
#include <QTextStream>

#include <QDebug>

#include <stdexcept>

static void crlf2lf(std::string& str)
{
  size_t w = 0, r = 0;

  while (r < str.size())
  {
    if (str.at(r) == '\r')
      ++r;
    else
      str[w++] = str.at(r++);
  }

  str.resize(w);
}

class NodeVisitor
{
public:
  virtual void visit(cxx::Class& c) = 0;
  virtual void visit(cxx::Enum& e) = 0;
  virtual void visit(cxx::EnumValue& e) = 0;
  virtual void visit(cxx::Function& f) = 0;
  virtual void visit(cxx::Namespace& n) = 0;


  void dispatch(cxx::Entity& e)
  {
    if (e.is<cxx::Class>())
      visit(static_cast<cxx::Class&>(e));
    else if (e.is<cxx::Function>())
      visit(static_cast<cxx::Function&>(e));
    else if (e.is<cxx::Namespace>())
      visit(static_cast<cxx::Namespace&>(e));
    if (e.is<cxx::Enum>())
      visit(static_cast<cxx::Enum&>(e));
    if (e.is<cxx::EnumValue>())
      visit(static_cast<cxx::EnumValue&>(e));
  }

};

class RecursiveTypeCollector : public NodeVisitor
{
public:

  MGProjectPtr project;
  json::Array result;

  RecursiveTypeCollector(MGProjectPtr p)
    : project(p)
  {

  }

  json::Object serialize(const MGType& t)
  {
    json::Object obj;
    obj["name"] = t.name;
    obj["id"] = t.id;
    obj["enum"] = t.category == MGType::EnumType;
    obj["class"] = t.category == MGType::ClassType;
    return obj;
  }

  json::Array operator()(MGModule& m)
  {
    for (auto e : m.entities)
      dispatch(*e);
    return result;
  }

  void visit(cxx::Class& c) override
  {
    for (auto e : c.members)
      dispatch(*e);

    auto it = project->entity_type_map.find(c.shared_from_this());

    if (it != project->entity_type_map.end())
    {
      result.push(serialize(*(it->second)));
    }
  }

  void visit(cxx::Enum& e) override
  {
    auto it = project->entity_type_map.find(e.shared_from_this());

    if (it != project->entity_type_map.end())
    {
      result.push(serialize(*(it->second)));
    }
  }

  void visit(cxx::EnumValue& e) override
  {

  }

  void visit(cxx::Function& f) override
  {

  }

  void visit(cxx::Namespace& n) override
  {
    for (auto e : n.entities)
      dispatch(*e);
  }
};


class ProjectSerializer : public NodeVisitor
{
public:

  MGProjectPtr project;
  SerializationMaps& map;
  json::Object metadata;
  json::Json result;

  ProjectSerializer(MGProjectPtr pro, SerializationMaps& m)
    : project(pro), map(m)
  {

  }

  json::Json serialize(cxx::Entity& n)
  {
    ProjectSerializer s{ project, map };
    s.dispatch(n);

    if (project->getMetadata(n.shared_from_this(), metadata))
    {
      for (const auto& key_value : metadata.data())
        s.result[key_value.first] = key_value.second;
    }

    return s.result;
  }

  json::Json serialize(MGModule& n)
  {
    ProjectSerializer s{ project, map };
    s.visit(n);

    if (project->getMetadata(n.shared_from_this(), metadata))
    {
      for (const auto& key_value : metadata.data())
        s.result[key_value.first] = key_value.second;
    }

    return s.result;
  }


  json::Array serialize(const std::vector<std::shared_ptr<cxx::Entity>>& nodes)
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

  void visit(cxx::Class& c) override
  {
    map.bind(c.shared_from_this(), result);

    result["type"] = "class";
    result["name"] = c.name;
    
    if (!c.bases.empty())
      result["base"] = c.bases.front().base->name;

    if (c.is_final)
      result["final"] = true;

    result["members"] = serialize(c.members);
  }

  void visit(MGModule& m)
  {
    map.bind(m.shared_from_this(), result);

    result["type"] = "module";
    result["name"] = m.name;

    if (!m.entities.empty())
    {
      json::Array elements;

      for (auto e : m.entities)
      {
        elements.push(serialize(*e));
      }

      result["elements"] = elements;
    }

    RecursiveTypeCollector collector{ project };
    result["types"] = collector(m);
  }

  void visit(cxx::Enum& e) override
  {
    map.bind(e.shared_from_this(), result);

    result["type"] = "enum";
    result["name"] = e.name;

    if (e.enum_class)
      result["enum_class"] = true;

    json::Array values;

    for (auto ev : e.values)
      values.push(serialize(*ev));

    result["values"] = values;
  }

  void visit(cxx::EnumValue& e) override
  {
    map.bind(e.shared_from_this(), result);

    result["type"] = "enumerator";
    result["name"] = e.name;
  }

  void visit(cxx::Function& f) override
  {
    map.bind(f.shared_from_this(), result);

    result["type"] = "function";
    result["name"] = f.name;
    
    if (!f.isConstructor() && !f.isDestructor())
      result["return_type"] = f.return_type.toString();

    if (f.isConstructor())
      result["constructor"] = true;
    else if (f.isDestructor())
      result["destructor"] = true;
    else if (f.name.find("operator", 0) == 0)
      result["operator"] = true;

    json::Array parameters;
    for (auto p : f.parameters)
    {
      parameters.push(p->type.toString());
    }
    result["parameters"] = parameters;

    if (f.isConst())
      result["const"] = true;

    if (f.isExplicit())
      result["explicit"] = true;

    if (f.isStatic())
      result["static"] = true;

    if (f.location.file() != nullptr)
      result["location"] = f.location.file()->path();
  }

  void visit(cxx::Namespace& n) override
  {
    map.bind(n.shared_from_this(), result);

    result["type"] = "namespace";
    result["name"] = n.name;

    if (!n.entities.empty())
      result["elements"] = serialize(n.entities);
  }

};

LiquidGenerator::TypeInfo::TypeInfo(const MGType & t)
  : MGType(t)
{

}


LiquidGenerator::LiquidGenerator(const QString & dir, const MGProjectPtr& pro)
  : mRootDirectory(dir),
    mProject(pro)
{

}

void LiquidGenerator::generate()
{
  QElapsedTimer timer;
  timer.start();

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

  QDirIterator diriterator{ mRootDirectory + "/plugins/" + QString::fromStdString(mProject->module_folder), QDir::Files, QDirIterator::Subdirectories };

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

  QDirIterator diriterator{ mRootDirectory + "/plugins/" + QString::fromStdString(mProject->module_folder), QDir::Files, QDirIterator::Subdirectories };

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
    crlf2lf(processed_content);
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
    tmplt.skipWhitespacesAfterTag();

    json::Object data;
    data["module_namespace"] = mProject->module_namespace;
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
    std::shared_ptr<cxx::Entity> node = m_serialization_map.get(object);
    return qualifiedName(*node);
  }
  else if (name == "typeid")
  {
    std::shared_ptr<cxx::Entity> node = m_serialization_map.get(object);
    auto it = project()->entity_type_map.find(node);

    if (it != project()->entity_type_map.end())
      return it->second->id;
    else
      return "";
  }
  else if (name == "get_symbol")
  {
    MGModulePtr m = mProject->getModule(args.at(0).toString());
    std::shared_ptr<cxx::Entity> node = m->getSymbol(args.at(1).toString());
    return m_serialization_map.get(node);
  }
  else if (name == "get_symbols_by_location")
  {
    MGModulePtr m = mProject->getModule(args.at(0).toString());
    std::vector<std::shared_ptr<cxx::Entity>> nodes = m->getSymbolsByLocation(args.at(1).toString());
    json::Array list;
    for (auto n : nodes)
      list.push(m_serialization_map.get(n));
    return list;
  }
  else if (name == "get_module")
  {
    MGModulePtr m = mProject->getModule(args.at(0).toString());
    return m_serialization_map.get(m);
  }
  else if (name == "parent")
  {
    std::shared_ptr<cxx::Entity> node = m_serialization_map.get(object);
    node = node->weak_parent.lock();
    return node ? m_serialization_map.get(node) : json::null;
  }
  else if (name == "signature")
  {
    std::shared_ptr<cxx::Entity> node = m_serialization_map.get(object);
    auto fn = std::static_pointer_cast<cxx::Function>(node);
    return ::signature(*fn);
  }
  else if (name == "operator_name")
  {
    std::shared_ptr<cxx::Entity> node = m_serialization_map.get(object);
    auto fn = std::static_pointer_cast<cxx::Function>(node);
    return operatorName(*fn).toStdString();
  }
  else if (name == "operator_need_return_type")
  {
    std::shared_ptr<cxx::Entity> node = m_serialization_map.get(object);
    auto fn = std::static_pointer_cast<cxx::Function>(node);
    OperatorSymbol opsym = getOperatorSymbol(QString::fromStdString(fn->name));

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
    std::shared_ptr<cxx::Entity> node = m_serialization_map.get(object);
    auto fn = std::static_pointer_cast<cxx::Function>(node);
    return mangledName(*fn).toStdString();
  }
  else if (name == "check_params")
  {
    std::shared_ptr<cxx::Entity> node = m_serialization_map.get(object);
    auto fn = std::static_pointer_cast<cxx::Function>(node);
    return isExposable(*fn);
  }
  else
  {
    return liquid::Renderer::applyFilter(name, object, args);
  }
}

QString LiquidGenerator::operatorName(const cxx::Function& fn) const
{
  OperatorSymbol op = getOperatorSymbol(QString::fromStdString(fn.name));

  QString result = fn.weak_parent.lock()->is<cxx::Class>() ? "memop_" : "op_";

  const bool is_member = fn.weak_parent.lock()->is<cxx::Class>();
  const int implicit_param_count = is_member ? 1 : 0;

  if (op == Plus)
    op = fn.parameters.size() + implicit_param_count == 1 ? UnaryPlus : Add;
  else if (op == Minus)
    op = fn.parameters.size() + implicit_param_count == 1 ? UnaryMinus : Sub;
  else if (op == PlusPlus)
    op = fn.parameters.size() + implicit_param_count == 1 ? PreIncrement : PostIncrement;
  else if (op == MinusMinus)
    op = fn.parameters.size() + implicit_param_count == 1 ? PreDecrement : PostDecrement;

  if (op == FunctionCall)
  {
    return result + static_operator_infos[op].short_name;
  }
  else if (op == Subscript)
  {
    // operator[] cannot be non-member
    if (fn.isConst())
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
        if (fn.return_type == fn.parameters.at(0)->type)
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

QString LiquidGenerator::mangledName(const cxx::Function& fun)
{
  QString ret = QString::fromStdString(fun.name);

  if (fun.weak_parent.lock() != nullptr && fun.weak_parent.lock()->is<cxx::Class>())
    ret = QString::fromStdString(fun.weak_parent.lock()->name) + "_" + ret;

  if (fun.isStatic())
    ret += "_static";

  for (auto funparam : fun.parameters)
  {
    QString p = QString::fromStdString(funparam->type.toString());

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

QString LiquidGenerator::computeWrapperName(const cxx::Function& fun)
{
  //return fun.implementation.isEmpty() ? mangledName(fun) : fun.implementation;
  return mangledName(fun);
}

bool LiquidGenerator::isExposable(const cxx::Function& fun)
{
  try
  {
    if (!fun.isConstructor() && !fun.isDestructor() && !isExposed(QString::fromStdString(fun.return_type.toString())))
      return false;
      
    for (auto funparam : fun.parameters)
    {
      const QString p = QString::fromStdString(funparam->type.toString());

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

  auto it = mTypeInfos.find(t.toStdString());
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

  for (const auto & t : mProject->types)
    mTypeInfos[t->name] = *t;
}

LiquidGenerator::TypeInfo & LiquidGenerator::typeinfo(const QString & t)
{
  if (t.isEmpty())
    throw UnsupportedType{ t };

  auto it = mTypeInfos.find(t.toStdString());
  if (it == mTypeInfos.end())
  {
    unsupported_types.insert(t);
    throw UnsupportedType{ t };
  }
  return it->second;
}