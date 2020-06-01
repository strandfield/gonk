// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "generator.h"

#include "gen/headerfile.h"
#include "gen/sourcefile.h"
#include "gen/cppfile.h"
#include "gen/format.h"

#include "project/statement.h"

#include <QDir>
#include <QElapsedTimer>
#include <QFile>
#include <QJsonObject>
#include <QTextStream>

#include <QDebug>

const QString Generator::endl = "\n";
const QString Generator::ClassBinderInclude = "gonk/common/binding/class.h";
const QString Generator::EnumBinderInclude = "gonk/common/enums.h";;
const QString Generator::NamespaceBinderInclude = "gonk/common/binding/namespace.h";;
const QString Generator::ClassBuilderInclude = "<script/classbuilder.h>";;
const QString Generator::EnumBuilderInclude = "<script/enumbuilder.h>";;


class StateGuard
{
public:
  Generator *gen;
  NodeRef node;
  QString current_module_name;
  ModuleRef current_module;
  FileRef current_file;
  HeaderFile *header;
  SourceFile *source;

  StateGuard(Generator *g, const NodeRef & n)
    : gen(g)
    , node(n)
  {
    current_module = gen->m_current_module;
    current_file = gen->m_current_file;
    current_module_name = gen->mCurrentModuleName;
    header = gen->mCurrentHeader;
    source = gen->mCurrentSource;

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

  ~StateGuard()
  {
    if (node != nullptr)
      gen->mProcessingStack.pop();
    gen->m_current_module = current_module;
    gen->mCurrentModuleName = current_module_name;
    gen->m_current_file = current_file;
    gen->mCurrentHeader = header;
    gen->mCurrentSource = source;
  }
};


Generator::TypeInfo::TypeInfo(const Type & t)
  : Type(t)
{

}


Generator::Generator(const QString & dir)
  : mRootDirectory(dir)
  , mCurrentHeader(nullptr)
  , mCurrentSource(nullptr)
{

}

void Generator::generate(const ProjectRef & p)
{
  QElapsedTimer timer;
  timer.start();

  m_generated_files.clear();
  m_generated_types.clear();

  mProject = p;

  buildTypeInfo();

  fetchTypesHeaders();

  {
    QDir dir{ mRootDirectory };
    if (!dir.exists("plugins"))
      dir.mkdir("plugins");
  }

  for (const auto & m : p->modules)
    generate(m);

  auto elapsed = timer.elapsed();
  qDebug() << "Generation done in" << elapsed;
}

QString Generator::to_snake_case(const QString & camelCaseName)
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

Generator::OperatorSymbol Generator::getOperatorSymbol(const QString & str)
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


void Generator::fetchTypesHeaders()
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

void Generator::fetchTypesHeaders(NodeRef node)
{
  if (node->is<Module>() || node->is<File>() || node->is<Namespace>())
  {
    StateGuard guard{ this, node };

    for (size_t i(0); i < node->childCount(); ++i)
      fetchTypesHeaders(node->childAt(i));
  }
  else if (node->is<Class>() || node->is<Enum>())
  {
    StateGuard guard{ this, node };

    m_types_headers[Node::nameQualification(this->mProcessingStack)] = m_current_module->module_dir_name() + "/" + m_current_file->name + ".h";

    for (size_t i(0); i < node->childCount(); ++i)
      fetchTypesHeaders(node->childAt(i));
  }
}

void Generator::generate(ModuleRef mod)
{
  m_generated_files.clear();
  m_generated_types.clear();

  StateGuard state{ this, mod };

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

void Generator::generateModuleDefsFile()
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

void Generator::generateModuleFile()
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

  {
    CppFile source;

    source.main_include = module_dir_name + ".h";

    source.include("libscript", "<script/engine.h>");
    source.include("libscript", "<script/namespace.h>");
    source.include("libscript", "<script/typesystem.h>");

    for (QString f : m_generated_files)
      source.lines.push_back(format("extern register_%1_file(script::Namespace ns); // defined in %1.cpp", f));

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

    for (QString f : m_generated_files)
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

void Generator::generate(FileRef file)
{
  StateGuard state{ this, file };

  HeaderFile header;
  header.file = QFileInfo{ currentHeaderDirectory() + "/" + file->name + ".h" };
  header.moduleName = mCurrentModuleName;
  for (const auto & inc : file->hincludes)
    header.generalIncludes.insert(inc);
  SourceFile source;
  source.file = QFileInfo{ currentSourceDirectory() + "/" + file->name + ".cpp" };
  source.header = file->name + ".h";
  for (const auto & inc : file->cppincludes)
    source.generalIncludes.insert(inc);

  mCurrentSource = &source;
  mCurrentHeader = &header;

  generate(NamespaceRef{ file });

  header.write();
  source.write();

  m_generated_files.push_back(file->name);
}

QString Generator::generate(FunctionRef fun)
{
  return generate(fun, getBindingMethod(fun));
}

QString Generator::generate(FunctionRef fun, Function::BindingMethod bm)
{
  if (bm == Function::ConstructorBinding && fun->parameters.isEmpty())
  {
    return QString("  bind::default_constructor<%1>(%2).create();").arg(enclosingName(), enclosing_snake_name());
  }
  else if (bm == Function::DestructorBinding)
  {
    return QString("  bind::destructor<%1>(%2).create();").arg(enclosingName(), enclosing_snake_name());
  }
  else if (bm == Function::OperatorBinding)
  {
    return generateOperator(fun, getOperatorSymbol(fun->name));
  }
  else if (fun->bindingMethod == Function::SignalBinding)
  {
    return generateSignal(fun);
  }
  else if (bm == Function::NewFunctionBinding)
  {
    return generateNewFunction(fun);
  }

  const QString funname = fun->rename.isEmpty() ? fun->name : fun->rename;
  const QString params = fparamscomma(fun);
  const QString funaddr = "&" + nameQualification() + fun->name;
  const QString fret = bm == Function::ConstructorBinding ? QString() : fparam(fun->returnType);

  QString ret = [&]() -> QString {
    if (bm == Function::ConstructorBinding)
      return QString("  bind::constructor<%1%2>(%3)").arg(enclosingName(), ", " + fparams(fun->parameters), enclosing_snake_name());
    else if (bm == Function::StaticVoidBinding)
      return QString("  bind::static_void_member_function<%1, %2%3>(%4, \"%5\")").arg(enclosingName(), params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::ConstVoidBinding && enclosingEntity() == "Namespace")
      return QString("  bind::void_function<%2%3>(%4, \"%5\")").arg(params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::ConstVoidBinding && enclosingEntity() == "Class")
      return QString("  bind::const_void_member_function<%1, %2%3>(%4, \"%5\")").arg(enclosingName(), params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::VoidBinding && enclosingEntity() == "Namespace")
      return QString("  bind::void_function<%2%3>(%4, \"%5\")").arg(params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::VoidBinding && enclosingEntity() == "Class")
      return QString("  bind::void_member_function<%1, %2%3>(%4, \"%5\")").arg(enclosingName(), params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::ChainableBinding)
      return QString("  bind::chainable_memfn<%1, %2%3>(%4, \"%5\")").arg(enclosingName(), params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::StaticBinding)
      return QString("  bind::static_member_function<%1, %2, %3%4>(%5, \"%6\")").arg(enclosingName(), fret, params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::SimpleBinding && enclosingEntity() == "Namespace")
      return QString("  bind::function<%1, %2%3>(%4, \"%5\")").arg(fret, params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::SimpleBinding && enclosingEntity() == "Class")
      return QString("  bind::member_function<%1, %2, %3%4>(%5, \"%6\")").arg(enclosingName(), fret, params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::ReferenceBinding)
      return QString("  bind::non_const_getter<%1, %2, %3%4>(%5, \"%6\")").arg(enclosingName(), fret, params, funaddr, enclosing_snake_name(), funname);
    else if (bm == Function::FreeFunctionBinding)
      return QString("  bind::fn_as_memfn<%1, %2, %3%4>(%5, \"%6\")").arg(enclosingName(), fret, params, "&" + fun->name, enclosing_snake_name(), funname);
    else if (bm == Function::FreeFunctionAsStaticBinding)
      return QString("  bind::static_member_function<%1, %2, %3%4>(%5, \"%6\")").arg(enclosingName(), fret, params, "&" + fun->name, enclosing_snake_name(), funname);

    throw std::runtime_error{ "Unsupported bind method !" };
  }();

  if (!fun->defaultArguments.isEmpty())
  {
    currentSource().bindingIncludes.insert("yasl/common/binding/default_arguments.h");

    ret += endl;
    ret += QString("    .apply(bind::default_arguments(") + fun->defaultArguments.join(", ") + "))";
  }

  ret.append(".create();");
  return ret;
}

QString Generator::generateSignal(FunctionRef fun)
{
  const QString funname = fun->rename.isEmpty() ? fun->name : fun->rename;
  const QString signature = fun->name + "(" + fun->parameters.join(",") + ")";

  if (fun->parameters.size() == 0)
  {
    QString format = "  bind::signal<%1>(%2, \"%3\", \"%4\");";
    return format.arg(enclosingName(), enclosing_snake_name(), funname, signature);
  }
  else
  {
    QString format = "  bind::signal<%1%5>(%2, \"%3\", \"%4\");";
    const QString params = fparams(fun->parameters, ", ");
    return format.arg(enclosingName(), enclosing_snake_name(), funname, signature, params);
  }
}

QString Generator::generateOperator(FunctionRef fun, OperatorSymbol op)
{
  QString out = enclosingEntity() == "Class" ? "  bind::mem" : "  bind::";
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
      return QString("  bind::memop_const_subscript<%1, %2, %3>(%4);").arg(enclosingName(), fparam(fun->returnType), fparam(fun, 0), enclosing_snake_name());
    else
      return QString("  bind::memop_subscript<%1, %2, %3>(%4);").arg(enclosingName(), fparam(fun->returnType), fparam(fun, 0), enclosing_snake_name());
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

QString Generator::generateNewFunction(FunctionRef fn)
{
  currentSource().bindingIncludes.insert("yasl/common/binding/newfunction.h");

  QString rettype = fn->returnType;
  if (rettype.endsWith(" &"))
    rettype.chop(2);
  else if (rettype.endsWith("&"))
    rettype.chop(1);

  QStringList targs{ rettype };
  targs.append(checkParams(fn->parameters));

  QString ret = "  ";
  ret += QString("bind::new_function<%2>(%3, \"%4\");").arg(targs.join(", "), enclosing_snake_name(), fn->name);
  return ret;
}

QString Generator::fparam(FunctionRef fun, int n)
{
  return fparam(fun->parameters.at(n));
}

QString Generator::fparam(const QString & p)
{
  QString pp = p;

  bool is_const = false;
  bool is_ref = false;
  bool is_refref = false;

  if (pp.startsWith("const "))
  {
    is_const = true;
    pp.remove(0, 6);
  }
  if (pp.endsWith("&&"))
  {
    is_refref = true;
    pp.chop(2);
  }
  if (pp.endsWith("&"))
  {
    is_ref = true;
    pp.chop(1);
  }

  if (pp.endsWith(" *"))
  {
    pp.chop(2);
    pp.append(QChar('*'));
  }

  pp = pp.simplified();

  const Type & info = typeinfo(pp);

  if (!info.header.isEmpty())
    currentSource().generalIncludes.insert(info.header);

  return p;
}

QString Generator::fparams(const QStringList & params, const QString & prefix)
{
  QStringList ret;
  for (const auto & p : params)
    ret << fparam(p);
  return prefix + ret.join(", ");
}

QString Generator::fparams(const FunctionRef fun)
{
  return fparams(fun->parameters);
}

QString Generator::fparamscomma(const FunctionRef fun)
{
  QString ret = fparams(fun);
  if (!ret.isEmpty())
    return ret + ", ";
  return ret;
}

const QStringList & Generator::checkParams(const QStringList & ps)
{
  for (const auto & p : ps)
    fparam(p);
  return ps;
}

Function::BindingMethod Generator::getBindingMethod(FunctionRef fun) const
{
  if (fun->bindingMethod == Function::AutoBinding)
    return guessBindingMethod(fun);
  return fun->bindingMethod;
}

Function::BindingMethod Generator::guessBindingMethod(FunctionRef fun) const
{
  if (fun->is<Constructor>())
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

QList<QPair<QString, QString>> Generator::extractLinks(const QString & str)
{
  QList<QPair<QString, QString>> ret;

  QStringList links = str.split(";", QString::SkipEmptyParts);
  for (auto lstr : links)
  {
    const int at_index = lstr.indexOf("@");
    if (at_index == -1)
      continue;

    QPair<QString, QString> link;
    link.first = lstr.mid(0, at_index).simplified();
    link.second = lstr.mid(at_index + 1).simplified();
    ret.append(link);
  }

  return ret;
}

QString Generator::fundisplay(FunctionRef fun)
{
  QStringList builder;

  if (fun->isExplicit)
    builder << "explicit ";
  if (fun->isStatic)
    builder << "static ";

  if (!fun->returnType.isEmpty())
  {
    builder << fun->returnType << " ";
  }
  builder << fun->name;
  builder << "(";
  {
    QStringList params = fun->parameters;
    for (int i(0); i < fun->defaultArguments.size(); ++i)
      params[params.size() - i - 1] += " = " + fun->defaultArguments.at(i);
    builder << params.join(", ");
  }
  builder << ")";

  if (fun->isConst)
    builder << " const";

  if (fun->isDeleted)
    builder << " = delete";

  builder << ";";

  return builder.join("");
}

static bool contains_any_signal(const QList<NodeRef> & nodes)
{
  for (const auto & n : nodes)
  {
    if (n->is<Function>() && n->as<Function>().bindingMethod == Function::SignalBinding)
      return true;
  }

  return false;
}

void Generator::generate(ClassRef cla)
{
  if (cla->checkState == Qt::Unchecked)
    return;

  currentSource().libscriptIncludes.insert(ClassBuilderInclude);

  const QString pre = prefix();
  const QString enclosing_snake = enclosing_snake_name();
  const QString enclosing_entity = enclosingEntity();
  const QString qual = nameQualification();

  StateGuard state{ this, cla };

  QString snake = to_snake_case(cla->name);
  Type class_info = typeinfo(qual + cla->name);
  QString claname = class_info.name;

  for (const auto n : cla->elements)
  {
    if (n->is<Class>() || n->is<Enum>())
      generate(n);
  }

  QStringList lines;

  if (!cla->condition.isEmpty())
    lines << QString("#if %1").arg(cla->condition);

  lines << QString("static void register_%1%2_class(script::%3 %4)").arg(pre, snake, enclosing_entity, enclosing_snake);
  lines << "{";
  lines << "  using namespace script;";
  lines << QString();

  {
    QString format = "  Class %1 = %2.%3(\"%4\").setId(script::Type::%5)__DATA____BASE____FINAL__.get();";
    QString arg3 = enclosing_entity == "Namespace" ? "newClass" : "newNestedClass";
    QString line = format.arg(snake, enclosing_snake, arg3, claname, class_info.id);
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
    if (contains_any_signal(cla->elements))
      line.replace("__DATA__", endl + "    .setData(yasl::createSignalTable())");
    else
      line.remove("__DATA__");

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

  currentSource().bindingIncludes.insert(ClassBinderInclude);

  lines << QString();


  for (const auto n : cla->elements)
  {
    if (n->checkState == Qt::Unchecked || !n->is<Function>())
      continue;

    FunctionRef fun = std::static_pointer_cast<Function>(n);

    if (!fun->condition.isEmpty())
      lines << QString("#if %1").arg(fun->condition);

    lines << ("  // " + fundisplay(fun));
    try
    {
      lines << generate(fun);
    }
    catch (...)
    {
      lines << ("  /// TODO: " + fundisplay(fun));
    }

    if (!fun->condition.isEmpty())
      lines << QString("#endif");
  }

  lines << "}";

  if (!cla->condition.isEmpty())
    lines << QString("#endif");

  lines << endl;

  QString body = lines.join(endl);

  currentSource().functions.append(body);

  m_generated_types.push_back(project()->getType(cla->type_id));
}

void Generator::generate(EnumRef enm)
{
  if (enm->checkState == Qt::Unchecked)
    return;

  currentSource().libscriptIncludes.insert(EnumBuilderInclude);
  currentSource().bindingIncludes.insert(EnumBinderInclude);

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
  lines << QString("  Enum %1 = %2.newEnum(\"%3\").setId(script::Type::%4)").arg(snake, enclosing_snake_name(), enmname, enum_info.id);
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

  currentSource().functions.append(lines.join(endl));

  m_generated_types.push_back(project()->getType(enm->type_id));
}

void Generator::generate(NamespaceRef ns)
{
  if (ns->checkState == Qt::Unchecked)
    return;

  //currentSource().libscriptIncludes.insert();
  currentSource().bindingIncludes.insert(NamespaceBinderInclude);

  const QString pre = prefix();
  const QString enclosing_snake = enclosing_snake_name();

  StateGuard guard{ this, ns };

  QString snake = ns->is<File>() ? "ns" : to_snake_case(ns->name);

  for (const auto n : ns->elements)
  {
    if (n->is<Namespace>() || n->is<Class>() || n->is<Enum>())
      generate(n);
  }

  QStringList lines;
  if (ns->is<File>())
    lines << QString("void register_%1%2_file(script::Namespace %3)").arg(pre, to_snake_case(ns->name), enclosing_snake);
  else
    lines << QString("static void register_%1%2_namespace(script::Namespace %3)").arg(pre, snake, enclosing_snake);
  lines << "{";
  lines << "  using namespace script;";
  lines << QString();
  if (ns->is<File>())
  {
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

      lines << ("  // " + fundisplay(fun));
      try
      {
        lines << generate(fun);
      }
      catch (...)
      {
        lines << ("  /// TODO: " + fundisplay(fun));
      }

      if (!fun->condition.isEmpty())
        lines << QString("#endif");
    }
    else if (n->is<Statement>())
    {
      lines << ("  " + n->name);
    }
  }

  lines << "}";

  currentSource().functions.append(lines.join(endl));
}

void Generator::generate(NodeRef n)
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

QString Generator::prefix() const
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

QString Generator::enclosingEntity() const
{
  if (mProcessingStack.isEmpty() || mProcessingStack.top()->is<Namespace>())
    return "Namespace";

  return "Class";
}

QString Generator::enclosingName() const
{
  if (mProcessingStack.isEmpty())
    return "";

  return mProcessingStack.top()->name;
}

QString Generator::enclosing_snake_name() const
{
  if (mProcessingStack.isEmpty() || mProcessingStack.top()->is<File>())
    return "ns";

  return to_snake_case(mProcessingStack.top()->name);
}

QString Generator::nameQualification() const
{
  return Node::nameQualification(mProcessingStack);
}

QString Generator::removeQualification(QString name) const
{
  const int index = name.lastIndexOf("::");
  if (index == -1)
    return name;
  return name.mid(index + 2);
}

bool Generator::isMember() const
{
  return !mProcessingStack.isEmpty() && mProcessingStack.top()->is<Class>();
}

HeaderFile & Generator::currentHeader()
{
  return *mCurrentHeader;
}

SourceFile & Generator::currentSource()
{
  return *mCurrentSource;
}

QString Generator::pluginDirectory() const
{
  return mRootDirectory + "/plugins";
}

QString Generator::currentHeaderDirectory()
{
  return currentSourceDirectory();
}

QString Generator::currentSourceDirectory()
{
  return mRootDirectory + "/plugins/" + m_current_module->module_dir_name();
}

void Generator::buildTypeInfo()
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

Generator::TypeInfo & Generator::typeinfo(const QString & t)
{
  if (t.isEmpty())
    throw UnsupportedType{ t };

  auto it = mTypeInfos.find(t);
  if (it == mTypeInfos.end())
  {
    //qDebug() << "Unsupported type: " << t;
    throw UnsupportedType{ t };
  }
  return it.value();
}