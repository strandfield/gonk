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

const QString LiquidGenerator::endl = "\n";
const QString LiquidGenerator::ClassBinderInclude = "gonk/common/binding/class.h";
const QString LiquidGenerator::EnumBinderInclude = "gonk/common/enums.h";;
const QString LiquidGenerator::NamespaceBinderInclude = "gonk/common/binding/namespace.h";;
const QString LiquidGenerator::ClassBuilderInclude = "<script/classbuilder.h>";;
const QString LiquidGenerator::EnumBuilderInclude = "<script/enumbuilder.h>";;


class LiquidGenStateGuard
{
public:
  LiquidGenerator*gen;
  NodeRef node;
  QString current_module_name;
  ModuleRef current_module;
  FileRef current_file;
  CppFile *header;
  CppFile *source;

  LiquidGenStateGuard(LiquidGenerator*g, const NodeRef & n)
    : gen(g)
    , node(n)
  {
    current_module = gen->m_current_module;
    current_file = gen->m_current_file;
    current_module_name = gen->mCurrentModuleName;
    header = gen->m_current_header;
    source = gen->m_current_source;

    if (node != nullptr)
    {
      gen->mProcessingStack.append(n);

      if (node->is<Module>())
      {
        gen->m_current_module = std::static_pointer_cast<Module>(node);

        if (gen->mCurrentModuleName.isEmpty())
          gen->mCurrentModuleName = node->name;
        else
          gen->mCurrentModuleName += "." + node->name;
      }

      if (node->is<File>())
      {
        gen->m_current_file = std::static_pointer_cast<File>(node);
      }
    }
  }

  ~LiquidGenStateGuard()
  {
    if (node != nullptr)
      gen->mProcessingStack.pop();
    gen->m_current_module = current_module;
    gen->mCurrentModuleName = current_module_name;
    gen->m_current_file = current_file;
    gen->m_current_header = header;
    gen->m_current_source = source;
  }
};


class ProjectSerializer : public NodeVisitor
{
public:

  json::Json result;

  static json::Json serialize(Node& n)
  {
    ProjectSerializer s;
    n.accept(s);
    return s.result;
  }

  static json::Array serialize(const QList<NodeRef>& nodes)
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

  static json::Json serialize(ProjectRef pro)
  {
    json::Json result;

    json::Array modules;

    for (auto m : pro->modules)
      modules.push(serialize(*m));

    result["modules"] = modules;

    return result;
  }

  void visit(Class& c) override
  {
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
  }

  void visit(Enum& e) override
  {
    result["type"] = "enum";
    result["name"] = e.name.toStdString();

    json::Array values;

    for (auto ev : e.enumerators)
      values.push(serialize(*ev));

    result["values"] = values;
  }

  void visit(Enumerator& e) override
  {
    result["type"] = "enumerator";
    result["name"] = e.name.toStdString();
  }

  void visit(File& f) override
  {

  }

  void visit(Function& f) override
  {
    result["type"] = "function";
    result["name"] = f.name.toStdString();
    
    if (!f.isConstructor && !f.isDestructor)
      result["return_type"] = f.returnType.toStdString();

    if (f.isConstructor)
      result["constructor"] = true;
    else if (f.isDestructor)
      result["destructor"] = true;

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
  }

  void visit(Namespace& n) override
  {
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
  mSerializedProject = ProjectSerializer::serialize(mProject);

  {
    QFile pro_json{ "project.json" };
    pro_json.open(QIODevice::WriteOnly | QIODevice::Truncate);
    pro_json.write(json::stringify(mSerializedProject).c_str());
  }

  fetchTypesHeaders();

  buildTypeInfo();

  {
    QDir dir{ mRootDirectory };
    if (!dir.exists("plugins"))
      dir.mkdir("plugins");
  }

  QDirIterator diriterator{ mRootDirectory + "/plugins", QDir::Files, QDirIterator::Subdirectories };

  while (diriterator.hasNext())
  {
    diriterator.next();

    QFileInfo item_info = diriterator.fileInfo();

    if (item_info.suffix() == "cpp" || item_info.suffix() == "h")
      processFile(item_info);

  }

  auto elapsed = timer.elapsed();

  for (const QString& t : unsupported_types)
  {
    qDebug() << t;
  }

  qDebug() << "Generation done in" << elapsed;
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
    std::string liquid_rendered = liquid::Renderer::render(tmplt, mSerializedProject.toObject());

    size_t replacement_begin = src.find('\n', src_end) + 1;
    size_t replacement_end = src.find("#endif // METAGONK_SOURCE", replacement_begin);

    src.replace(src.begin() + replacement_begin, src.begin() + replacement_end, liquid_rendered);

    offset = src.find("#if METAGONK_SOURCE", src_begin);
  }

  return src;
}

json::Json LiquidGenerator::applyFilter(const std::string& name, const json::Json& object, const std::vector<json::Json>& args)
{
  return liquid::Renderer::applyFilter(name, object, args);
}

void LiquidGenerator::fetchTypesHeaders()
{
  m_types_headers.clear();

  for (ModuleRef m : project()->modules)
    fetchTypesHeaders(m);

  for (auto t : project()->types.fundamentals)
  {
    if (!t->header.isEmpty())
      m_types_headers[t->name] = t->header;
  }

  for (auto t : project()->types.enums)
  {
    if (!t->header.isEmpty())
      m_types_headers[t->name] = t->header;
  }

  for (auto t : project()->types.classes)
  {
    if (!t->header.isEmpty())
      m_types_headers[t->name] = t->header;
  }
}

void LiquidGenerator::fetchTypesHeaders(NodeRef node)
{
  if (node->is<Module>() || node->is<File>() || node->is<Namespace>())
  {
    LiquidGenStateGuard guard{ this, node };

    for (size_t i(0); i < node->childCount(); ++i)
      fetchTypesHeaders(node->childAt(i));
  }
  else if (node->is<Class>() || node->is<Enum>())
  {
    QString qual = Node::nameQualification(this->mProcessingStack);

    m_types_headers[qual + node->name] = m_current_module->module_dir_name() + "/" + m_current_file->name + ".h";

    LiquidGenStateGuard guard{ this, node };

    for (size_t i(0); i < node->childCount(); ++i)
      fetchTypesHeaders(node->childAt(i));
  }
}

void LiquidGenerator::generate(ModuleRef mod)
{
  m_generated_types.clear();

  LiquidGenStateGuard state{ this, mod };

  {
    QDir dir{ currentSourceDirectory() };
    if (!dir.exists())
      QDir(mRootDirectory + "/plugins").mkdir(mod->module_dir_name());
  }

  for (const auto & n : mod->elements)
  {
    if (n->checkState == Qt::Unchecked)
      continue;

    if (n->is<File>())
    {
      generate(std::static_pointer_cast<File>(n));

      if (mProgressCallback)
      {
        if (!mProgressCallback(std::static_pointer_cast<File>(n)->name))
          return;
      }
    }
    else
    {
      qDebug() << "TODO in generate(ModuleRef mod)";
    }
  }

  generateModuleDefsFile();
  generateModuleFile();
}

static void get_generated_files(std::vector<QString>& result, NodeRef n)
{
  if (n->is<File>())
  {
    result.push_back(n->name);
  }
  else
  {
    for (size_t i(0); i < n->childCount(); ++i)
      get_generated_files(result, n->childAt(i));
  }
}

std::vector<QString> LiquidGenerator::getGeneratedFiles(ModuleRef m)
{
  std::vector<QString> result;
  get_generated_files(result, m);
  return result;
}

void LiquidGenerator::generateModuleDefsFile()
{
  // Generate defs.h
  CppFile defs;

  QString module_snake = m_current_module->module_snake_name();
  QString module_snake_upper = m_current_module->module_snake_name().toUpper();

  defs.header_guard = "GONK_" + module_snake_upper + "_DEFS_H";

  defs.lines.push_back(format("#if (defined(WIN32) || defined(_WIN32))"));
  defs.lines.push_back(format("#if defined(GONK_%1_COMPILE_LIBRARY)", module_snake_upper));
  defs.lines.push_back(format("#  define GONK_%1_API __declspec(dllexport)", module_snake_upper));
  defs.lines.push_back(format("#else"));
  defs.lines.push_back(format("#  define GONK_%1_API __declspec(dllimport)", module_snake_upper));
  defs.lines.push_back(format("#endif"));
  defs.lines.push_back(format("#else"));
  defs.lines.push_back(format("#define GONK_%1_API", module_snake_upper));
  defs.lines.push_back(format("#endif"));
  defs.lines.push_back(format(""));
  defs.lines.push_back(format("namespace gonk"));
  defs.lines.push_back(format("{"));
  defs.lines.push_back(format(""));
  defs.lines.push_back(format("namespace %1", module_snake));
  defs.lines.push_back(format("{"));
  defs.lines.push_back(format(""));
  defs.lines.push_back(format("enum class EnumTypeIds"));
  defs.lines.push_back(format("{"));
  defs.lines.push_back(format("  FirstTypeId,"));

  for (auto t : m_generated_types)
  {
    if (t->is_enum)
    {
      defs.lines.push_back("  " + t->id + ",");
    }
  }

  defs.lines.push_back(format("  LastTypeId,"));
  defs.lines.push_back(format("};"));
  defs.lines.push_back(format(""));

  defs.lines.push_back(format("enum class ClassTypeIds"));
  defs.lines.push_back(format("{"));
  defs.lines.push_back(format("  FirstTypeId,"));

  for (auto t : m_generated_types)
  {
    if (t->is_class)
    {
      defs.lines.push_back("  " + t->id + ",");
    }
  }

  defs.lines.push_back(format("  LastTypeId,"));
  defs.lines.push_back(format("};"));
  defs.lines.push_back(format(""));
  defs.lines.push_back(format("GONK_%1_API int enum_type_id_offset();", module_snake_upper));
  defs.lines.push_back(format("GONK_%1_API int class_type_id_offset();", module_snake_upper));
  defs.lines.push_back(format(""));
  defs.lines.push_back(format("} // namespace %1", module_snake));
  defs.lines.push_back(format(""));
  defs.lines.push_back(format("} // namespace gonk"));
  defs.lines.push_back(format(""));

  defs.write(QFileInfo{ currentHeaderDirectory() + "/" + m_current_module->module_dir_name() + "-defs.h" });
}

void LiquidGenerator::generateModuleFile()
{
  QString module_snake = m_current_module->module_snake_name();
  QString module_snake_upper = m_current_module->module_snake_name().toUpper();
  QString module_camel_case = m_current_module->module_camel_case();
  QString module_dir_name = m_current_module->module_dir_name();

  {
    CppFile header;

    header.header_guard = "GONK_" + module_snake_upper + "_H";

    header.main_include = module_dir_name + "-defs.h";

    header.include("gonk", "gonk/plugin.h");

    header.lines.push_back(format("extern \"C\""));
    header.lines.push_back(format("{"));
    header.lines.push_back(format(""));
    header.lines.push_back(format("  GONK_%1_API gonk::Plugin* gonk_%2_module();", module_snake_upper, module_snake));
    header.lines.push_back(format(""));
    header.lines.push_back(format("} // extern \"C\""));
    header.lines.push_back(format(""));

    header.write(QFileInfo{ currentHeaderDirectory() + "/" + m_current_module->module_dir_name() + ".h" });
  }

  auto generated_files = getGeneratedFiles(m_current_module);

  {
    CppFile source;

    source.main_include = module_dir_name + ".h";

    source.include("libscript", "<script/engine.h>");
    source.include("libscript", "<script/namespace.h>");
    source.include("libscript", "<script/typesystem.h>");

    for (QString f : generated_files)
      source.lines.push_back(format("extern void register_%1_file(script::Namespace ns); // defined in %1.cpp", f));

    source.lines.push_back("");

    source.lines.push_back(format("namespace gonk"));
    source.lines.push_back(format("{"));
    source.lines.push_back(format(""));
    source.lines.push_back(format("namespace %1", module_snake));
    source.lines.push_back(format("{"));
    source.lines.push_back(format(""));
    source.lines.push_back(format("int class_type_id_offset_value = 0;"));
    source.lines.push_back(format("int enum_type_id_offset_value = 0;"));
    source.lines.push_back(format(""));
    source.lines.push_back(format("int class_type_id_offset()"));
    source.lines.push_back(format("{"));
    source.lines.push_back(format("  return class_type_id_offset_value;"));
    source.lines.push_back(format("}"));
    source.lines.push_back(format(""));
    source.lines.push_back(format("int enum_type_id_offset()"));
    source.lines.push_back(format("{"));
    source.lines.push_back(format("  return enum_type_id_offset_value;"));
    source.lines.push_back(format("}"));
    source.lines.push_back(format(""));
    source.lines.push_back(format("} // namespace %1", module_snake));
    source.lines.push_back(format(""));
    source.lines.push_back(format("} // namespace gonk"));

    source.lines.push_back("");

    source.lines.push_back(format("class %1Plugin : public gonk::Plugin", module_camel_case));
    source.lines.push_back(format("{"));
    source.lines.push_back(format("public:"));
    source.lines.push_back(format(""));
    source.lines.push_back(format("  void load(script::Module m) override"));
    source.lines.push_back(format("  {"));
    source.lines.push_back(format("    script::Engine* e = m.engine();"));
    source.lines.push_back(format(""));
    source.lines.push_back(format("    int nb_enum_types = static_cast<int>(gonk::%1::EnumTypeIds::LastTypeId) - static_cast<int>(gonk::%1::EnumTypeIds::FirstTypeId);", module_snake));
    source.lines.push_back(format("    int nb_class_types = static_cast<int>(gonk::%1::ClassTypeIds::LastTypeId) - static_cast<int>(gonk::%1::ClassTypeIds::FirstTypeId);", module_snake));
    source.lines.push_back(format(""));
    source.lines.push_back(format("    gonk::%1::enum_type_id_offset_value = static_cast<int>(e->typeSystem()->reserve(script::Type::EnumFlag, nb_enum_types));", module_snake));
    source.lines.push_back(format("    gonk::%1::class_type_id_offset_value = static_cast<int>(e->typeSystem()->reserve(script::Type::ObjectFlag, nb_class_types));", module_snake));
    source.lines.push_back(format(""));
    source.lines.push_back(format("    script::Namespace ns = m.root();"));
    source.lines.push_back(format(""));

    for (QString f : generated_files)
      source.lines.push_back(format("    register_%1_file(ns);", f));

    source.lines.push_back(format("  }"));
    source.lines.push_back(format(""));
    source.lines.push_back(format("  void unload(script::Module m) override"));
    source.lines.push_back(format("  {"));
    source.lines.push_back(format(""));
    source.lines.push_back(format("  }"));
    source.lines.push_back(format("};"));
    source.lines.push_back(format(""));
    source.lines.push_back(format("gonk::Plugin* gonk_%1_module()", module_snake));
    source.lines.push_back(format("{"));
    source.lines.push_back(format("  return new %1Plugin();", module_camel_case));
    source.lines.push_back(format("}"));

    source.write(QFileInfo{ currentHeaderDirectory() + "/" + module_dir_name + ".cpp" });
  }
}

void LiquidGenerator::generate(FileRef file)
{
  LiquidGenStateGuard state{ this, file };

  CppFile header;
  header.header_guard = "GONK_" + m_current_module->module_snake_name().toUpper() + "_" + file->name.toUpper() + "_H";

  header.include("binding", "gonk/common/types.h");

  header.include("module-defs", m_current_module->module_dir_name() + "-defs.h");

  for (const auto & inc : file->hincludes)
    header.include("general", inc);

  CppFile source;
  source.main_include = file->name + ".h";

  for (const auto & inc : file->cppincludes)
    source.include("general", inc);

  m_current_source = &source;
  m_current_header = &header;

  currentHeader().lines.push_back("namespace script");
  currentHeader().lines.push_back("{");

  for (NodeRef child : file->elements)
  {
    if (child->is<Statement>())
    {
      QString content = child->name;
      content.replace("\\n", "\n");
      currentSource().lines.push_back(content);
    }
  }

  generate(NamespaceRef{ file });

  currentHeader().lines.push_back("} // namespace script");

  header.write(QFileInfo{ currentHeaderDirectory() + "/" + file->name + ".h" });
  source.write(QFileInfo{ currentSourceDirectory() + "/" + file->name + ".cpp" });
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

QString LiquidGenerator::generateWrapper(const Function& fun)
{
  NodeRef parent = fun.parent.lock();

  QString result;

  result += fun.returnType + " " + computeWrapperName(fun) + "(";

  QStringList params;

  if (parent->is<Class>() && !fun.isStatic)
    params.append((fun.isConst ? "const " : "") + parent->name + "& self");

  for (int i(0); i < fun.parameters.size(); ++i)
  {
    params.push_back(fun.parameters.at(i) + " a" + QString::number(i + 1));
  }

  result += params.join(", ") + ")\n";
  result += "{\n";
  result += "  ";

  if (fun.returnType != "void")
    result += "return ";

  params.clear();

  if (parent->is<Class>() && !fun.isStatic)
  {
    result += "self." + fun.name + "(";

    for (int i(0); i < fun.parameters.size(); ++i)
    {
      params.push_back("a" + QString::number(i + 1));
    }

    result += params.join(", ") + ")";
  }
  else
  {
    if (parent->is<Class>())
      result += parent->name + "::";

    result += fun.name + "(";

    for (int i(0); i < fun.parameters.size(); ++i)
    {
      params.push_back("a" + QString::number(i + 1));
    }

    result += params.join(", ") + ")";
  }

  result += ";\n";

  result += "}\n";

  return result;
}

QString LiquidGenerator::generate(FunctionRef fun)
{
  return generate(fun, getBindingMethod(fun));
}

QString LiquidGenerator::generate(FunctionRef fun, Function::BindingMethod bm)
{
  if (bm == Function::ConstructorBinding && fun->parameters.isEmpty())
  {
    return QString("  gonk::bind::default_constructor<%1>(%2).create();").arg(enclosingName(), enclosing_snake_name());
  }
  else if (bm == Function::DestructorBinding)
  {
    return QString("  gonk::bind::destructor<%1>(%2).create();").arg(enclosingName(), enclosing_snake_name());
  }
  else if (bm == Function::OperatorBinding)
  {
    return generateOperator(fun, getOperatorSymbol(fun->name));
  }
  else if (bm == Function::NewConstructor)
  {
    const QString funaddr = "&" + fun->implementation;
    const QString formatted_params = formatParams("gonk::make_type<%1>()", params(*fun));
    const QString explicit_str = fun->isExplicit ? ".setExplicit()" : "";
    return QString("  %1.newConstructor(%2)%4.params(%3).create();").arg(enclosing_snake_name(), funaddr, formatted_params, explicit_str);
  }

  const QString funname = fun->name;
  const QString params = fparamscomma(fun);
  const QString funaddr = [&]() -> QString {
    if (bm == Function::BindingMethod::GenWrapperBinding && fun->implementation.isEmpty())
      return "&" + computeWrapperName(*fun);
    else
      return "&" + (fun->implementation.isEmpty() ? (nameQualification() + fun->name) : fun->implementation);
  }();
  const QString fret = bm == Function::ConstructorBinding ? QString() : fparam(fun->returnType);

  QString ret = [&]() -> QString {
    if (bm == Function::ConstructorBinding)
      return QString("  gonk::bind::constructor<%1%2>(%3)").arg(enclosingName(), ", " + fparams(fun->parameters), enclosing_snake_name());
    else if (bm == Function::StaticVoidBinding)
      return QString("  gonk::bind::static_void_member_function<%1, %2%3>(%4, \"%5\")").arg(enclosingName(), params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::ConstVoidBinding && enclosingEntity() == "Namespace")
      return QString("  gonk::bind::void_function<%2%3>(%4, \"%5\")").arg(params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::ConstVoidBinding && enclosingEntity() == "Class")
      return QString("  gonk::bind::const_void_member_function<%1, %2%3>(%4, \"%5\")").arg(enclosingName(), params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::VoidBinding && enclosingEntity() == "Namespace")
      return QString("  gonk::bind::void_function<%2%3>(%4, \"%5\")").arg(params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::VoidBinding && enclosingEntity() == "Class")
      return QString("  gonk::bind::void_member_function<%1, %2%3>(%4, \"%5\")").arg(enclosingName(), params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::ChainableBinding)
      return QString("  gonk::bind::chainable_memfn<%1, %2%3>(%4, \"%5\")").arg(enclosingName(), params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::StaticBinding)
      return QString("  gonk::bind::static_member_function<%1, %2, %3%4>(%5, \"%6\")").arg(enclosingName(), fret, params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::SimpleBinding && enclosingEntity() == "Namespace")
      return QString("  gonk::bind::free_function<%1, %2%3>(%4, \"%5\")").arg(fret, params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::SimpleBinding && enclosingEntity() == "Class")
      return QString("  gonk::bind::member_function<%1, %2, %3%4>(%5, \"%6\")").arg(enclosingName(), fret, params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::FreeFunctionBinding)
      return QString("  gonk::bind::fn_as_memfn<%1, %2, %3%4>(%5, \"%6\")").arg(enclosingName(), fret, params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::FreeFunctionAsStaticBinding)
      return QString("  gonk::bind::static_member_function<%1, %2, %3%4>(%5, \"%6\")").arg(enclosingName(), fret, params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::GenWrapperBinding && enclosingEntity() == "Class")
      return QString("  gonk::bind::fn_as_memfn<%1, %2, %3%4>(%5, \"%6\")").arg(enclosingName(), fret, params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::GenWrapperBinding && enclosingEntity() == "Namespace" && fun->returnType == "void")
      return QString("  gonk::bind::void_function<%3%4>(%5, \"%6\")").arg(params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::GenWrapperBinding && enclosingEntity() == "Namespace" && fun->returnType != "void")
      return QString("  gonk::bind::free_function<%2, %3%4>(%5, \"%6\")").arg(fret, params, funaddr, enclosing_snake_name(), funname);

    throw std::runtime_error{ "Unsupported bind method !" };
  }();

  if (!fun->defaultArguments.isEmpty())
  {
    currentSource().include("binding", "gonk/common/binding/default_arguments.h");

    ret += endl;
    ret += QString("    .apply(bind::default_arguments(") + fun->defaultArguments.join(", ") + "))";
  }

  ret.append(".create();");
  return ret;
}

QString LiquidGenerator::generateOperator(FunctionRef fun, OperatorSymbol op)
{
  QString out = enclosingEntity() == "Class" ? "  gonk::bind::mem" : "  gonk::bind::";
  out += "op_";

  const int implicit_param_count = isMember() ? 1 : 0;

  if (op == Plus)
    op = fun->parameters.count() + implicit_param_count == 1 ? UnaryPlus : Add;
  else if (op == Minus)
    op = fun->parameters.count() + implicit_param_count == 1 ? UnaryMinus : Sub;
  else if (op == PlusPlus)
    op = fun->parameters.count() + implicit_param_count == 1 ? PreIncrement : PostIncrement;
  else if (op == MinusMinus)
    op = fun->parameters.count() + implicit_param_count == 1 ? PreDecrement : PostDecrement;

  if (op == FunctionCall)
  {
    out += static_operator_infos[op].short_name + QString("<");
    QStringList targs;
    if (static_operator_infos[op].print_return_type)
      targs << fparam(fun->returnType);
    for (int i(0); i < fun->parameters.size(); ++i)
      targs << fparam(fun, i);
    out += targs.join(", ");
    out += ">();";
    return out;
  }
  else if (op == Subscript)
  {
    // operator[] cannot be non-member
    if (fun->isConst)
      return QString("  gonk::bind::memop_const_subscript<%1, %2, %3>(%4);").arg(enclosingName(), fparam(fun->returnType), fparam(fun, 0), enclosing_snake_name());
    else
      return QString("  gonk::bind::memop_subscript<%1, %2, %3>(%4);").arg(enclosingName(), fparam(fun->returnType), fparam(fun, 0), enclosing_snake_name());
  }
  else
  {
    if (op == LeftShift || op == RightShift)
    {
      if (!isMember())
      {
        if (fun->returnType == fun->parameters.at(0))
        {
          out += (op == LeftShift ? "put_to" : "read_from") + QString("<");
          QStringList targs;
          targs << fparam(fun->parameters.first()) << fparam(fun->parameters.at(1));
          out += targs.join(", ");
          out += QString(">(%1);").arg(enclosing_snake_name());
          return out;
        }
      }
      else
      {
        if (fun->returnType == enclosingName() + " &")
        {
          out += (op == LeftShift ? "put_to" : "read_from") + QString("<");
          out += enclosingName() + ", ";
          out += fparam(fun->parameters.first());
          out += QString(">(%1);").arg(enclosing_snake_name());
          return out;
        }
      }
    }

    out += static_operator_infos[op].short_name + QString("<");
    if (enclosingEntity() == "Class")
      out += enclosingName() + ", ";
    QStringList targs;
    if (static_operator_infos[op].print_return_type)
      targs << fparam(fun->returnType);
    if (static_operator_infos[op].nargs - implicit_param_count == 1)
      targs << fparam(fun->parameters.first());
    else if (static_operator_infos[op].nargs - implicit_param_count == 2)
      targs << fparam(fun->parameters.first()) << fparam(fun->parameters.at(1));
    out += targs.join(", ");
    out += QString(">(%1);").arg(enclosing_snake_name());
    return out;
  }
}

QString LiquidGenerator::generateMakeTypeHelper(std::shared_ptr<Type> t)
{
  QStringList lines;

  if (!t->condition.isEmpty())
    lines << QString("#if %1").arg(t->condition);

  if (t->is_class)
  {
    lines << format("template<> struct make_type_helper<%3> { inline static script::Type get()"
    "{ return (gonk::%1::class_type_id_offset() + static_cast<int>(gonk::%1::ClassTypeIds::%2)) | script::Type::ObjectFlag; } };",
      m_current_module->module_snake_name(), t->id, t->name);
  }
  else
  {
    lines << format("template<> struct make_type_helper<%3> { inline static script::Type get()"
      "{ return (gonk::%1::enum_type_id_offset() + static_cast<int>(gonk::%1::EnumTypeIds::%2)) | script::Type::EnumFlag; } };",
      m_current_module->module_snake_name(), t->id, t->name);
  }

  if (!t->condition.isEmpty())
    lines << QString("#endif");

  return lines.join(endl);
}

void LiquidGenerator::recordGeneratedType(int type_database_id)
{
  auto t = project()->getType(type_database_id);

  m_generated_types.push_back(t);

  QString lines = generateMakeTypeHelper(t);

  currentHeader().lines.push_back(lines);
}

QString LiquidGenerator::fparam(FunctionRef fun, int n)
{
  return fparam(fun->parameters.at(n));
}

QString LiquidGenerator::fparam(const QString & p)
{
  checkParam(p);
  return p;
}

QString LiquidGenerator::fparams(const QStringList & params, const QString & prefix)
{
  QStringList ret;
  for (const auto & p : params)
    ret << fparam(p);
  return prefix + ret.join(", ");
}

QString LiquidGenerator::fparams(const FunctionRef fun)
{
  return fparams(fun->parameters);
}

QString LiquidGenerator::fparamscomma(const FunctionRef fun)
{
  QString ret = fparams(fun);
  if (!ret.isEmpty())
    return ret + ", ";
  return ret;
}

const QStringList & LiquidGenerator::checkParams(const QStringList & ps)
{
  for (const auto & p : ps)
    fparam(p);
  return ps;
}

void LiquidGenerator::checkParam(QString p)
{
  bool is_const = false;
  bool is_ref = false;
  bool is_refref = false;

  if (p.startsWith("const "))
  {
    is_const = true;
    p.remove(0, 6);
  }
  if (p.endsWith("&&"))
  {
    is_refref = true;
    p.chop(2);
  }
  if (p.endsWith("&"))
  {
    is_ref = true;
    p.chop(1);
  }

  if (p.endsWith(" *"))
  {
    p.chop(2);
    p.append(QChar('*'));
  }

  p = p.simplified();

  const Type& info = typeinfo(p);

  if (!info.header.isEmpty())
    currentSource().include("general", info.header);
}

QString LiquidGenerator::formatParams(QString fmt, QStringList params, QString prefix, QString suffix)
{
  for (QString& s : params)
    s = fmt.arg(s);

  return prefix + params.join(',') + suffix;
}

QStringList LiquidGenerator::params(const Function& fun)
{
  for (const QString p : fun.parameters)
    checkParam(p);
  return fun.parameters;
}

Function::BindingMethod LiquidGenerator::getBindingMethod(FunctionRef fun) const
{
  if (fun->bindingMethod == Function::AutoBinding)
    return guessBindingMethod(fun);
  return fun->bindingMethod;
}

Function::BindingMethod LiquidGenerator::guessBindingMethod(FunctionRef fun) const
{
  if (fun->isConstructor)
    return Function::ConstructorBinding;

  if (fun->name.startsWith("operator"))
  {
    OperatorSymbol sym = getOperatorSymbol(fun->name);
    if (sym != Invalid)
      return Function::OperatorBinding;
  }

  if (fun->name == "~" + enclosingName())
    return Function::DestructorBinding;

  if (fun->returnType == "void")
  {
    if (fun->isStatic)
      return Function::StaticVoidBinding;
    else if (fun->isConst)
      return Function::ConstVoidBinding;
    else
      return Function::VoidBinding;
  }
  else if (fun->returnType == enclosingName() + " &")
  {
    return Function::ChainableBinding;
  }

  if (fun->isStatic)
    return Function::StaticBinding;
  else
    return Function::SimpleBinding;
}

void LiquidGenerator::generate(ClassRef cla)
{
  if (cla->checkState == Qt::Unchecked)
    return;

  currentSource().include("libscript", ClassBuilderInclude);

  const QString pre = prefix();
  const QString enclosing_snake = enclosing_snake_name();
  const QString enclosing_entity = enclosingEntity();
  const QString qual = nameQualification();

  LiquidGenStateGuard state{ this, cla };

  QString snake = to_snake_case(cla->name);
  Type class_info = typeinfo(qual + cla->name);
  QString claname = class_info.name;

  bool has_any_gen_wrapper = false;

  for (const auto n : cla->elements)
  {
    if (n->is<Class>() || n->is<Enum>())
      generate(n);
    else if (n->is<Function>() && n->as<Function>().bindingMethod == Function::GenWrapperBinding)
      has_any_gen_wrapper = true;
  }

  QStringList lines;

  if (!cla->condition.isEmpty())
    lines << QString("#if %1").arg(cla->condition);


  if (has_any_gen_wrapper)
  {
    lines << QString("namespace {");
    for (const auto n : cla->elements)
    {
      if (n->is<Function>() && n->as<Function>().bindingMethod == Function::GenWrapperBinding)
        lines << generateWrapper(n->as<Function>());
    }
    lines << QString("}\n");
  }

  lines << QString("static void register_%1%2_class(script::%3 %4)").arg(pre, snake, enclosing_entity, enclosing_snake);
  lines << "{";
  lines << "  using namespace script;";
  lines << QString();

  {
    QString format = "  Class %1 = %2.%3(\"%4\").setId(script::Type::make<%5>().data())__BASE____FINAL__.get();";
    QString arg3 = enclosing_entity == "Namespace" ? "newClass" : "newNestedClass";
    QString line = format.arg(snake, enclosing_snake, arg3, claname, class_info.name);
    if (!cla->base.isEmpty())
    {
      Type base_class_info = typeinfo(cla->base);
      line.replace("__BASE__", endl + "    .setBase(script::Type::" + base_class_info.id + ")");
    }
    else
    {
      line.remove("__BASE__");
    }
    if (cla->isFinal)
    {
      line.replace("__FINAL__", endl + "    .setFinal()");
    }
    else
    {
      line.remove("__FINAL__");
    }

    lines << line;
  }

  lines << QString();

  for (const auto n : cla->elements)
  {
    if (n->checkState == Qt::Unchecked)
      continue;

    if (!n->is<Enum>() && !n->is<Class>())
      continue;

    if (!n->condition.isEmpty())
      lines << QString("#if %1").arg(n->condition);

    if (n->is<Enum>())
      lines << QString("  register_%1%2_enum(%3);").arg(prefix(), to_snake_case(n->name), snake);
    else if (n->is<Class>())
      lines << QString("  register_%1%2_class(%3);").arg(prefix(), to_snake_case(n->name), snake);

    if (!n->condition.isEmpty())
      lines << QString("#endif");
  }

  currentSource().include("binding", ClassBinderInclude);

  lines << QString();


  for (const auto n : cla->elements)
  {
    if (n->checkState == Qt::Unchecked)
      continue;

    if (n->is<Function>())
    {
      FunctionRef fun = std::static_pointer_cast<Function>(n);

      if (!fun->condition.isEmpty())
        lines << QString("#if %1").arg(fun->condition);

      lines << ("  // " + fun->signature());
      try
      {
        lines << generate(fun);
      }
      catch (...)
      {
        lines << ("  /// TODO: " + fun->signature());
      }

      if (!fun->condition.isEmpty())
        lines << QString("#endif");
    }
    else if (n->is<Statement>())
    {
      QString content = n->name;
      content.replace("\\n", "\n");
      lines << ("  " + content);
    }
  }

  lines << "}";

  if (!cla->condition.isEmpty())
    lines << QString("#endif");

  lines << endl;

  QString body = lines.join(endl);

  currentSource().lines.push_back(body);

  recordGeneratedType(cla->type_id);
}

void LiquidGenerator::generate(EnumRef enm)
{
  if (enm->checkState == Qt::Unchecked)
    return;

  currentSource().include("libscript", EnumBuilderInclude);
  currentSource().include("binding", EnumBinderInclude);

  const QString pre = prefix();

  QString snake = to_snake_case(enm->name);
  Type enum_info = typeinfo(nameQualification() + enm->name);
  QString enmname = removeQualification(enum_info.name);

  QStringList lines;

  if (!enm->condition.isEmpty())
    lines << QString("#if %1").arg(enm->condition);

  lines << QString("static void register_%1%2_enum(script::%3 %4)").arg(pre, snake, enclosingEntity(), enclosing_snake_name());
  lines << "{";
  lines << "  using namespace script;";
  lines << QString();
  lines << QString("  Enum %1 = %2.newEnum(\"%3\").setId(script::Type::make<%4>().data())").arg(snake, enclosing_snake_name(), enmname, enum_info.name);
  if (enm->isEnumClass)
    lines.back().append(endl + "    .setEnumClass()");
  lines.back().append(".get();");
  lines << QString();

  QString format;
  if (enm->isCppEnumClass)
    format = "  %1.addValue(\"%2\", static_cast<int>(%3));";
  else
    format = "  %1.addValue(\"%2\", %3);";

  for (const auto & v : enm->enumerators)
  {
    if (v->checkState == Qt::Unchecked)
      continue;

    if (!v->condition.isEmpty())
      lines << QString("#if %1").arg(v->condition);

    lines << format.arg(snake, v->name, nameQualification() + v->name);

    if (!v->condition.isEmpty())
      lines << QString("#endif");
  }

  lines << "}";

  if (!enm->condition.isEmpty())
    lines << QString("#endif");

  lines << endl;

  currentSource().lines.push_back(lines.join(endl));

  recordGeneratedType(enm->type_id);
}

void LiquidGenerator::generate(NamespaceRef ns)
{
  if (ns->checkState == Qt::Unchecked)
    return;

  //currentSource().libscriptIncludes.insert();
  currentSource().include("binding", NamespaceBinderInclude);

  const QString pre = prefix();
  const QString enclosing_snake = enclosing_snake_name();

  LiquidGenStateGuard guard{ this, ns };

  QString snake = ns->is<File>() ? "ns" : to_snake_case(ns->name);

  bool has_any_gen_wrapper = false;

  for (const auto n : ns->elements)
  {
    if (n->is<Namespace>() || n->is<Class>() || n->is<Enum>())
      generate(n);
    else if (n->is<Function>() && n->as<Function>().bindingMethod == Function::GenWrapperBinding)
      has_any_gen_wrapper = true;
  }

  QStringList lines;

  if (has_any_gen_wrapper)
  {
    lines << QString("namespace {");
    for (const auto n : ns->elements)
    {
      if (n->is<Function>() && n->as<Function>().bindingMethod == Function::GenWrapperBinding)
        lines << generateWrapper(n->as<Function>());
    }
    lines << QString("}\n");
  }

  if (ns->is<File>())
    lines << QString("void register_%1%2_file(script::Namespace %3)").arg(pre, to_snake_case(ns->name), enclosing_snake);
  else
    lines << QString("static void register_%1%2_namespace(script::Namespace %3)").arg(pre, snake, enclosing_snake);
  lines << "{";
  lines << "  using namespace script;";
  lines << QString();
  if (ns->is<File>())
  {
    if(snake != enclosing_snake)
      lines << QString("  Namespace %1 = %2;").arg(snake, enclosing_snake);
  }
  else
  {
    const QString name = ns->rename.isEmpty() ? ns->name : ns->rename;

    if (name == "@")
      lines << QString("  Namespace %1 = %2;").arg(snake, enclosing_snake);
    else
      lines << QString("  Namespace %1 = %2.getNamespace(\"%3\");").arg(snake, enclosing_snake, name);
  }
  lines << QString();

  for (const auto n : ns->elements)
  {
    if (n->checkState == Qt::Unchecked)
      continue;

    if (!n->is<Enum>() && !n->is<Class>() && !n->is<Namespace>())
      continue;

    if (!n->condition.isEmpty())
      lines << QString("#if %1").arg(n->condition);

    if (n->is<Enum>())
      lines << QString("  register_%1%2_enum(%3);").arg(prefix(), to_snake_case(n->name), snake);
    else if (n->is<Class>())
      lines << QString("  register_%1%2_class(%3);").arg(prefix(), to_snake_case(n->name), snake);
    else if (n->is<Namespace>())
      lines << QString("  register_%1%2_namespace(%3);").arg(prefix(), to_snake_case(n->name), snake);

    if (!n->condition.isEmpty())
      lines << QString("#endif");
  }

  lines << QString();

  for (const auto n : ns->elements)
  {
    if (n->checkState == Qt::Unchecked)
      continue;

    if (n->is<Function>())
    {
      FunctionRef fun = std::static_pointer_cast<Function>(n);
      if (!fun->condition.isEmpty())
        lines << QString("#if %1").arg(fun->condition);

      lines << ("  // " + fun->signature());
      try
      {
        lines << generate(fun);
      }
      catch (...)
      {
        lines << ("  /// TODO: " + fun->signature());
      }

      if (!fun->condition.isEmpty())
        lines << QString("#endif");
    }
    else if (n->is<Statement>() && !ns->is<File>())
    {
      QString content = n->name;
      content.replace("\\n", "\n");
      lines << ("  " + content);
    }
  }

  lines << "}";

  currentSource().lines.push_back(lines.join(endl));
}

void LiquidGenerator::generate(NodeRef n)
{
  if (n->is<File>())
  {
    qDebug() << "Error in generate(NodeRef) : input is a file";
    return;
  }

  if (n->is<Module>())
    generate(std::static_pointer_cast<Module>(n));
  else if (n->is<Namespace>())
    generate(std::static_pointer_cast<Namespace>(n));
  else if (n->is<Class>())
    generate(std::static_pointer_cast<Class>(n));
  else if (n->is<Enum>())
    generate(std::static_pointer_cast<Enum>(n));
  else
  {
    qDebug() << "Error in generate(NodeRef)";
  }
}

QString LiquidGenerator::prefix() const
{
  if (mProcessingStack.isEmpty())
    return "";

  QStringList names;
  for (const auto & node : mProcessingStack)
  {
    if (node->is<File>() || node->is<Module>())
      continue;

    names << to_snake_case(node->name);
  }

  if (names.empty())
    return "";
  return names.join("_") + "_";
}

QString LiquidGenerator::enclosingEntity() const
{
  if (mProcessingStack.isEmpty() || mProcessingStack.top()->is<Namespace>())
    return "Namespace";

  return "Class";
}

QString LiquidGenerator::enclosingName() const
{
  if (mProcessingStack.isEmpty())
    return "";

  return mProcessingStack.top()->name;
}

QString LiquidGenerator::enclosing_snake_name() const
{
  if (mProcessingStack.isEmpty() || mProcessingStack.top()->is<File>())
    return "ns";

  return to_snake_case(mProcessingStack.top()->name);
}

QString LiquidGenerator::nameQualification() const
{
  return Node::nameQualification(mProcessingStack);
}

QString LiquidGenerator::removeQualification(QString name) const
{
  const int index = name.lastIndexOf("::");
  if (index == -1)
    return name;
  return name.mid(index + 2);
}

bool LiquidGenerator::isMember() const
{
  return !mProcessingStack.isEmpty() && mProcessingStack.top()->is<Class>();
}

CppFile& LiquidGenerator::currentHeader()
{
  return *m_current_header;
}

CppFile& LiquidGenerator::currentSource()
{
  return *m_current_source;
}

QString LiquidGenerator::pluginDirectory() const
{
  return mRootDirectory + "/plugins";
}

QString LiquidGenerator::currentHeaderDirectory()
{
  return currentSourceDirectory();
}

QString LiquidGenerator::currentSourceDirectory()
{
  return mRootDirectory + "/plugins/" + m_current_module->module_dir_name();
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

  for (const auto& e : m_types_headers)
  {
    mTypeInfos[e.first].header = e.second;
  }
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