// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include <libclang-utils/clang-cursor.h>
#include <libclang-utils/clang-translation-unit.h>
#include <libclang-utils/clang-index.h>

#include <map>

#include <iostream>
#include <sstream>

class Class;
class Entity;
class Enum;
class Function;
class Namespace;
class Scope;

CXFile get_cursor_file(libclang::Cursor cursor)
{
  CXSourceLocation location = cursor.api->clang_getCursorLocation(cursor);

  CXFile file;
  unsigned int line, col, offset;
  cursor.api->clang_getSpellingLocation(location, &file, &line, &col, &offset);

  return file;
}

class Entity : public std::enable_shared_from_this<Entity>
{
public:
  libclang::Cursor cursor;
  std::string name;
  std::weak_ptr<Entity> weak_parent;

public:
  virtual ~Entity() { }

  explicit Entity(libclang::Cursor c, std::shared_ptr<Entity> p) : cursor(c), weak_parent(p) 
  { 
    name = c.getSpelling();
  }

  std::shared_ptr<Entity> parent() const { return weak_parent.lock(); }

  virtual bool isScope() const { return false; }
  virtual std::string className() const = 0;

  std::string qualifiedName() const
  {
    auto p = parent();
    return (p && !p->name.empty()) ? (p->qualifiedName() + "::" + name) : name;
  }

  std::string prefix() const
  {
    auto p = parent();
    return (p && !p->name.empty()) ? (p->prefix() + p->name + "_") : "";
  }

  bool isMember() const;
};

class Enum : public Entity
{
public:
  std::vector<std::string> values;

public:
  Enum(libclang::Cursor c, std::shared_ptr<Entity> p) : Entity(c, p)
  {
    c.visitChildren([&](libclang::Cursor child) {

      if (child.kind() != CXCursor_EnumConstantDecl)
        return;

      std::string cname = child.getSpelling();

      if (!cname.empty())
        values.push_back(cname);

      });
  }

  std::string className() const override
  {
    return "Enum";
  }

  bool isEnumClass() const
  {
    return cursor.api->clang_EnumDecl_isScoped(cursor);
  }
};

enum class OperatorName {
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
  PutTo,
  ReadFrom,
};

OperatorName get_operator_name(const std::string& str)
{
  static const std::map<std::string, OperatorName> symbols = {
    {"operator=", OperatorName::Assign},
    { "operator==", OperatorName::Eq },
    { "operator!=", OperatorName::Neq },
    { "operator<", OperatorName::Less },
    { "operator<=", OperatorName::Leq },
    { "operator>", OperatorName::Greater },
    { "operator>=", OperatorName::Geq },
    { "operator+", OperatorName::Plus },
    { "operator-", OperatorName::Minus },
    { "operator*", OperatorName::Mul },
    { "operator/", OperatorName::Div },
    { "operator%", OperatorName::Modulo },
    { "operator+=", OperatorName::AddAssign },
    { "operator-=", OperatorName::SubAssign },
    { "operator*=", OperatorName::MulAssign },
    { "operator/=", OperatorName::DivAssign },
    { "operator&", OperatorName::And },
    { "operator|", OperatorName::Or },
    { "operator&=", OperatorName::AndAssign },
    { "operator|=", OperatorName::OrAssign },
    { "operator[]", OperatorName::Subscript },
    { "operator()", OperatorName::FunctionCall },
    { "operator++", OperatorName::PlusPlus },
    { "operator--", OperatorName::MinusMinus },
    { "operator<<", OperatorName::LeftShift },
    { "operator>>", OperatorName::RightShift },
    { "operator<<=", OperatorName::LeftShiftAssign },
    { "operator>>=", OperatorName::RightShiftAssign },
    { "operator^", OperatorName::Xor },
    { "operator^=", OperatorName::XorAssign },
    { "operator~", OperatorName::BitwiseNot },
  };

  auto it = symbols.find(str);
  return it != symbols.end() ? it->second : OperatorName::Invalid;
}

struct OperatorInfo {
  std::string short_name;
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
  { "put_to", false, 2 },
  { "read_from", false, 2 },
};

class Function : public Entity
{
public:
  std::string return_type;
  std::vector<std::string> params;

public:
  Function(libclang::Cursor c, std::shared_ptr<Entity> p) : Entity(c, p) 
  { 
    {
      CXCursorKind kind = c.kind();
      CXType function_type = c.getType();

      if (kind != CXCursor_Constructor && kind != CXCursor_Destructor)
      {
        CXType rt = c.api->clang_getResultType(function_type);
        return_type = c.api->toStdString(c.api->clang_getTypeSpelling(rt));
      }
      else
      {
        return_type = "void";
      }
    }

    {
      const int num_args = c.getNumArguments();

      params.reserve(num_args);

      for (int i = 0; i < num_args; ++i)
      {
        libclang::Cursor arg_cursor = c.getArgument(i);

        CXType arg_type = arg_cursor.getType();
        params.push_back(c.api->toStdString(c.api->clang_getTypeSpelling(arg_type)));
      }
    }
  }

  std::string className() const override
  {
    return "Function";
  }

  bool isConstructor() const
  {
    return cursor.kind() == CXCursor_Constructor;
  }

  bool isDestructor() const
  {
    return cursor.kind() == CXCursor_Destructor;
  }

  bool isConst() const
  {
    return cursor.api->clang_CXXMethod_isConst(cursor);
  }

  bool isStatic() const
  {
    return cursor.api->clang_CXXMethod_isStatic(cursor);
  }

  bool isOverloaded() const;

  bool isOperator() const
  {
    return std::strncmp(name.c_str(), "operator", 8) == 0;
  }

  OperatorName operatorName() const
  {
    OperatorName op = get_operator_name(name);

    bool is_member = isMember();
    const int implicit_param_count = is_member ? 1 : 0;

    if (op == OperatorName::Plus)
      return params.size() + implicit_param_count == 1 ? OperatorName::UnaryPlus : OperatorName::Add;
    else if (op == OperatorName::Minus)
      return implicit_param_count == 1 ? OperatorName::UnaryMinus : OperatorName::Sub;
    else if (op == OperatorName::PlusPlus)
      return params.size() + implicit_param_count == 1 ? OperatorName::PreIncrement : OperatorName::PostIncrement;
    else if (op == OperatorName::MinusMinus)
      return params.size() + implicit_param_count == 1 ? OperatorName::PreDecrement : OperatorName::PostDecrement;

    if (op == OperatorName::LeftShift || op == OperatorName::RightShift)
    {
      if (!is_member)
      {
        if (return_type == params.at(0))
        {
          return op == OperatorName::LeftShift ? OperatorName::PutTo : OperatorName::ReadFrom;
        }
      }
      else
      {
        // TODO: check if not l- or r-shift
        return op == OperatorName::LeftShift ? OperatorName::PutTo : OperatorName::ReadFrom;
      }
    }

    return op;
  }
};

class Scope : public Entity
{
public:
  std::vector<std::shared_ptr<Enum>> enums;
  std::vector<std::shared_ptr<Class>> classes;
  std::vector<std::shared_ptr<Function>> functions;
  std::map<std::string, int> names_map;

public:
  Scope(libclang::Cursor c, std::shared_ptr<Entity> p) : Entity(c, p) { }

  bool isScope() const override { return true; }

  virtual bool isClass() const { return false; }
  virtual bool isNamespace() const { return false; }

  bool isEmpty() const
  {
    return name.empty() || (enums.empty() && classes.empty() && functions.empty());
  }

  void addFunction(std::shared_ptr<Function> f)
  {
    auto it = names_map.find(f->name);

    if (it == names_map.end())
    {
      names_map[f->name] = 1;
    }
    else
    {
      it->second += 1;
    }

    functions.push_back(f);
  }

  bool isOverloaded(const Function& f)
  {
    auto it = names_map.find(f.name);
    return it != names_map.end() && it->second > 1;
  }
};

class Class : public Scope
{
public:
  Class(libclang::Cursor c, std::shared_ptr<Entity> p) : Scope(c, p)
  {

  }

  void parse()
  {
    CX_CXXAccessSpecifier access_specifier = CX_CXXAccessSpecifier::CX_CXXPrivate;

    cursor.visitChildren([&](libclang::Cursor child) {

      CXCursorKind kind = child.kind();

      if (kind == CXCursor_CXXAccessSpecifier)
      {
        access_specifier = child.getCXXAccessSpecifier();
        return;
      }

      if (access_specifier != CX_CXXAccessSpecifier::CX_CXXPublic)
        return;

      switch (kind)
      {
      case CXCursor_EnumDecl:
      {
        auto enm = std::make_shared<Enum>(child, shared_from_this());

        if (!enm->name.empty())
          enums.push_back(enm);
      }
      break;
      case CXCursor_CXXMethod:
      case CXCursor_FunctionDecl:
      case CXCursor_Constructor:
      case CXCursor_Destructor:
      {
        auto f = std::make_shared<Function>(child, shared_from_this());
        addFunction(f);
      }
      break;
      case CXCursor_ClassDecl:
      {
        auto cla = Class::make(child, shared_from_this());

        if (!cla->isEmpty())
          classes.push_back(cla);
      }
      break;
      default:
        break;
      }

      });
  }

  static std::shared_ptr<Class> make(libclang::Cursor c, std::shared_ptr<Entity> p)
  {
    std::shared_ptr<Class> self = std::make_shared<Class>(c, p);
    self->parse();
    return self;
  }

  std::string className() const override
  {
    return "Class";
  }

  bool isClass() const override { return true; }

  bool isNestedClass() const
  {
    auto p = parent();
    return p && p->isScope() && static_cast<Scope&>(*p).isClass();
  }
};

class Namespace : public Scope
{
public:
  std::vector<std::shared_ptr<Namespace>> namespaces;

public:

  Namespace(libclang::Cursor c, std::shared_ptr<Entity> p) 
    : Scope(c, p)
  {
  
  }

  void parse(std::function<bool(libclang::Cursor)> validator)
  {
    cursor.visitChildren([&](libclang::Cursor child) {

      if (!validator(child))
        return;

      CXCursorKind kind = child.kind();

      switch (kind)
      {
      case CXCursor_EnumDecl:
      {
        auto enm = std::make_shared<Enum>(child, shared_from_this());

        if (!enm->name.empty())
          enums.push_back(enm);
      }
      break;
      case CXCursor_CXXMethod:
      case CXCursor_FunctionDecl:
      case CXCursor_Constructor:
      case CXCursor_Destructor:
      {
        auto f = std::make_shared<Function>(child, shared_from_this());
        addFunction(f);
      }
      break;
      case CXCursor_ClassDecl:
      {
        auto cla = Class::make(child, shared_from_this());

        if (!cla->isEmpty())
          classes.push_back(cla);
      }
      break;
      case CXCursor_Namespace:
      {
        auto ns = Namespace::make(child, shared_from_this());
        namespaces.push_back(ns);
      }
      break;
      default:
        break;
      }

      });
  }

  static std::shared_ptr<Namespace> make(libclang::Cursor c, std::shared_ptr<Entity> p, std::function<bool(libclang::Cursor)> validator)
  {
    std::shared_ptr<Namespace> self = std::make_shared<Namespace>(c, p);
    self->parse(validator);
    return self;
  }

  static std::shared_ptr<Namespace> make(libclang::Cursor c, std::shared_ptr<Entity> p)
  {
    return make(c, p, [](libclang::Cursor) -> bool { return true; });
  }

  std::string className() const override
  {
    return "Namespace";
  }

  bool isNamespace() const override { return true; }
};


inline bool Entity::isMember() const
{
  auto p = parent();
  return p->isScope() && static_cast<Scope&>(*p).isClass();
}

inline bool Function::isOverloaded() const
{
  auto scp = std::dynamic_pointer_cast<Scope>(parent());
  return scp && scp->isOverloaded(*this);
}

static bool starts_with(const std::string& str, const char* pattern)
{
  return std::strncmp(str.data(), pattern, std::strlen(pattern)) == 0;
}

static std::string to_snake_case(const std::string& input)
{
  std::string result;

  for (char c : input)
  {
    if (std::isupper(c))
    {
      result += "_";
      result += char(std::tolower(c));
    }
    else
    {
      result += c;
    }
  }

  if (starts_with(result, "_"))
    result.erase(result.begin());
  if (starts_with(result, "q_"))
    result.erase(result.begin(), result.begin()+2);
  if (result == "class" || result == "char")
    result += '_';

  return result;
}

static std::string join(const std::vector<std::string>& list, char sep = ',')
{
  std::string ret;

  for (const auto& s : list)
  {
    ret += s;
    ret.push_back(sep);
  }

  if (!list.empty())
    ret.pop_back();

  return ret;
}

std::ostream& output()
{
  return std::cout;
}

std::string get_operator_binder_name(const Function& fn, OperatorName op)
{
  std::string result = fn.isMember() ? "memop_" : "op_";

  const bool is_member = fn.isMember();
  const int implicit_param_count = is_member ? 1 : 0;

   if (op == OperatorName::Subscript)
  {
    // operator[] cannot be non-member
    if (fn.isConst())
      return result + "const_subscript";
    else
      return result + "subscript";
  }

   return result + static_operator_infos[static_cast<size_t>(op)].short_name;
}

void generate(std::shared_ptr<Enum> enm)
{
  std::string snake_name = to_snake_case(enm->name);
  std::string qualname = enm->qualifiedName();

  output() << "static void register_" << enm->prefix() + snake_name << "(script::" << enm->parent()->className() << "& parent)\n";
  output() << "{\n";
  output() << "  using namespace script;\n";
  output() << "\n";
  output() << "  script::Enum " << snake_name << " = gonk::bind::enumeration<" << qualname << ">(parent, \"" << enm->name << "\").get();\n";

  bool is_enum_class = enm->isEnumClass();

  auto do_cast = [&](std::string str) -> std::string {
    if (is_enum_class)
      return "static_cast<int>(" + str + ")";
    else
      return str;
  };

  for (const auto& v : enm->values)
  {
    output() << "  " << snake_name << ".addValue(\"" << v << "\", " << do_cast(qualname + "::" + v) << ");\n";
  }

  output() << "}\n";
}

void generate(std::shared_ptr<Function> fn)
{
  if (fn->isConstructor())
  {
    if (fn->params.empty())
    {
      output() << "  gonk::bind::default_constructor<" << fn->parent()->qualifiedName() << ">(scope).create();\n";
    }
    else
    {
      output() << "  gonk::bind::constructor<" << fn->parent()->qualifiedName() << "," << join(fn->params) << ">(scope).create();\n";
    }
  }
  else if (fn->isDestructor())
  {
    output() << "  gonk::bind::destructor<" << fn->parent()->qualifiedName() << ">(scope).create();\n";
  }
  else if (fn->isOperator())
  {
    OperatorName name = fn->operatorName();

    if (name == OperatorName::Invalid)
      return;

    const OperatorInfo& info = static_operator_infos[static_cast<size_t>(name)];

    std::string op_name = get_operator_binder_name(*fn, name);

    if (fn->isMember())
    {
      if (info.print_return_type)
        output() << "  gonk::bind::" << op_name << "<" << fn->parent()->qualifiedName() << ", " << fn->return_type << ", " << join(fn->params) << ">(scope);\n";
      else
        output() << "  gonk::bind::" << op_name << "<" << fn->parent()->qualifiedName() << ", " << join(fn->params) << ">(scope);\n";
    }
    else
    {
      if (info.print_return_type)
        output() << "  gonk::bind::" << op_name << "<" << fn->return_type << ", " << join(fn->params) << ">(scope);\n";
      else
        output() << "  gonk::bind::" << op_name << "<" << join(fn->params) << ">(scope);\n";
    }
  }
  else
  {
    std::string addr;

    if (fn->isOverloaded())
      addr = "qOverload<" + join(fn->params) + ">(&" + fn->qualifiedName() + ")";
    else
      addr = "&" + fn->qualifiedName();

    if(fn->isMember() && !fn->isStatic())
      output() << "  gonk::bind::method(scope, \"" << fn->name << "\", " << addr << ");\n";
    else
      output() << "  gonk::bind::function(scope, \"" << fn->name << "\", " << addr << ");\n";

  }
}

void generate(std::shared_ptr<Class> cla)
{
  std::string snake_name = to_snake_case(cla->name);

  for (auto cc : cla->classes)
    generate(cc);

  for (auto enm : cla->enums)
    generate(enm);

  if (cla->isNestedClass())
    output() << "static void register_" << cla->prefix() << snake_name << "(script::Class& parent)\n";
  else
    output() << "static void register_" << cla->prefix() << snake_name << "(script::Namespace& parent)\n";

  output() << "{\n";
  output() << "  using namespace script;\n";
  output() << "\n";

  if (cla->isNestedClass())
    output() << "  script::Class " << snake_name << " = parent.newNestedClass(\"" << cla->name << "\")";
  else
    output() << "  script::Class " << snake_name << " = parent.newClass(\"" << cla->name << "\")";

  output() << ".setId(script::Type::make<" << cla->qualifiedName() << ">().data())\n";

  // @TODO: set base if any
  // @TODO: set final 

  output() << "  .get();\n";

  output() << "\n";
  output() << "  script::Class& c = " << snake_name << ";\n";
  output() << "  script::Class& scope = c;\n";

  output() << "\n";

  for (auto cc : cla->classes)
  {
    output() << "  register_" << cc->prefix() << to_snake_case(cc->name) << "(c);\n";
  }

  for (auto enm : cla->enums)
  {
    output() << "  register_" << enm->prefix() << to_snake_case(enm->name) << "(c);\n";
  }

  for (auto f : cla->functions)
  {
    generate(f);
  }

  output() << "}\n";
}

void generate(std::shared_ptr<Namespace> ns)
{
  std::string snake_name = to_snake_case(ns->name);

  for (auto cc : ns->classes)
    generate(cc);

  for (auto enm : ns->enums)
    generate(enm);

  output() << "static void register_" << ns->prefix() << snake_name << "(script::Namespace& parent)\n";
    

  output() << "{\n";
  output() << "  using namespace script;\n";
  output() << "\n";

  output() << "  script::Namespace " << snake_name << " = parent.newNamespace(\"" << ns->name << "\")";


  output() << "  .get();\n";

  output() << "\n";
  output() << "  script::Namespace& ns = " << snake_name << ";\n";
  output() << "  script::Namespace& scope = ns;\n";

  output() << "\n";

  for (auto cc : ns->classes)
  {
    output() << "  register_" << cc->prefix() << to_snake_case(cc->name) << "(ns);\n";
  }

  for (auto enm : ns->enums)
  {
    output() << "  register_" << enm->prefix() << to_snake_case(enm->name) << "(ns);\n";
  }

  for (auto f : ns->functions)
  {
    generate(f);
  }

  output() << "}\n";
}

int main(int argc, char *argv[])
{
  std::string file;
  std::set<std::string> includedirs;

  for (int i(1); i < argc; ++i)
  {
    std::string arg{ argv[i] };

    if (std::strncmp(arg.data(), "-I", 2) == 0)
      includedirs.insert(std::string(arg.begin() + 2, arg.end()));
    else
      file = arg;
  }

  if (file.empty())
  {
    std::cerr << "Missing file input..." << std::endl;
    return 1;
  }

  libclang::LibClang clang;

  libclang::Index index = clang.createIndex();

  libclang::TranslationUnit tu = index.parseTranslationUnit(file, includedirs, CXTranslationUnit_SkipFunctionBodies);
  CXFile tu_file = tu.api->clang_getFile(tu, file.data());

  libclang::Cursor c = tu.getCursor();

  std::shared_ptr<Namespace> global_namespace = Namespace::make(c, nullptr, [&](libclang::Cursor child) -> bool {
    CXFile child_file = get_cursor_file(child);
    return child.api->clang_File_isEqual(tu_file, child_file);
    });
  global_namespace->name.clear();

  generate(global_namespace);

  return 0;
}