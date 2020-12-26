// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk-debugger-defs.h"

#include <script/ast/visitor.h>
#include <script/ast.h>

#include <QJsonArray>
#include <QJsonObject>

namespace gonk
{

class GonkAstProducer : public script::ast::AstVisitor
{
public:

  script::Ast ast;
  QJsonObject result;
  QJsonArray children;

  QJsonObject produce(script::Ast a)
  {
    ast = a;

    QJsonObject ret;
    
    QJsonArray nodes;

    auto root = std::static_pointer_cast<script::ast::ScriptRootNode>(a.root());

    for (auto s : root->statements)
    {
      nodes.push_back(prod(s));
    }

    ret["nodes"] = nodes;

    return ret;
  }

  static QString to_str(script::ast::NodeType nt)
  {
    switch (nt)
    {
    case script::ast::NodeType::BoolLiteral: return "BoolLiteral";
    case script::ast::NodeType::IntegerLiteral: return "IntegerLiteral";
    case script::ast::NodeType::FloatingPointLiteral: return "FloatingPointLiteral";
    case script::ast::NodeType::StringLiteral: return "StringLiteral";
    case script::ast::NodeType::UserDefinedLiteral: return "UserDefinedLiteral";
    case script::ast::NodeType::SimpleIdentifier: return "SimpleIdentifier";
    case script::ast::NodeType::TemplateIdentifier: return "TemplateIdentifier";
    case script::ast::NodeType::QualifiedIdentifier: return "QualifiedIdentifier";
    case script::ast::NodeType::OperatorName: return "OperatorName";
    case script::ast::NodeType::LiteralOperatorName: return "LiteralOperatorName";
    case script::ast::NodeType::QualifiedType: return "QualifiedType";
    case script::ast::NodeType::FunctionCall: return "FunctionCall";
    case script::ast::NodeType::BraceConstruction: return "BraceConstruction";
    case script::ast::NodeType::ArraySubscript: return "ArraySubscript";
    case script::ast::NodeType::Operation: return "Operation";
    case script::ast::NodeType::ConditionalExpression: return "ConditionalExpression";
    case script::ast::NodeType::ArrayExpression: return "ArrayExpression";
    case script::ast::NodeType::ListExpression: return "ListExpression";
    case script::ast::NodeType::LambdaExpression: return "LambdaExpression";
    case script::ast::NodeType::NullStatement: return "NullStatement";
    case script::ast::NodeType::ExpressionStatement: return "ExpressionStatement";
    case script::ast::NodeType::CompoundStatement: return "CompoundStatement";
    case script::ast::NodeType::IfStatement: return "IfStatement";
    case script::ast::NodeType::WhileLoop: return "WhileLoop";
    case script::ast::NodeType::ForLoop: return "ForLoop";
    case script::ast::NodeType::ReturnStatement: return "ReturnStatement";
    case script::ast::NodeType::ContinueStatement: return "ContinueStatement";
    case script::ast::NodeType::BreakStatement: return "BreakStatement";
    case script::ast::NodeType::EnumDeclaration: return "EnumDeclaration";
    case script::ast::NodeType::VariableDeclaration: return "VariableDeclaration";
    case script::ast::NodeType::ClassDeclaration: return "ClassDeclaration";
    case script::ast::NodeType::FunctionDeclaration: return "FunctionDeclaration";
    case script::ast::NodeType::ConstructorDeclaration: return "ConstructorDeclaration";
    case script::ast::NodeType::DestructorDeclaration: return "DestructorDeclaration";
    case script::ast::NodeType::OperatorOverloadDeclaration: return "OperatorOverloadDeclaration";
    case script::ast::NodeType::CastDeclaration: return "CastDeclaration";
    case script::ast::NodeType::AccessSpecifier: return "AccessSpecifier";
    case script::ast::NodeType::ConstructorInitialization: return "ConstructorInitialization";
    case script::ast::NodeType::BraceInitialization: return "BraceInitialization";
    case script::ast::NodeType::AssignmentInitialization: return "AssignmentInitialization";
    case script::ast::NodeType::Typedef: return "Typedef";
    case script::ast::NodeType::NamespaceDecl: return "NamespaceDecl";
    case script::ast::NodeType::ClassFriendDecl: return "ClassFriendDecl";
    case script::ast::NodeType::UsingDeclaration: return "UsingDeclaration";
    case script::ast::NodeType::UsingDirective: return "UsingDirective";
    case script::ast::NodeType::NamespaceAliasDef: return "NamespaceAliasDef";
    case script::ast::NodeType::TypeAliasDecl: return "TypeAliasDecl";
    case script::ast::NodeType::ImportDirective: return "ImportDirective";
    case script::ast::NodeType::TemplateDecl: return "TemplateDecl";
    case script::ast::NodeType::ScriptRoot: return "ScriptRoot";
    default: return "other";
    }
  }

  QJsonObject prod(std::shared_ptr<script::ast::Node> n)
  {
    QJsonObject ret;
    QJsonArray childs;

    ret["type"] = to_str(n->type());
    ret["offset"] = static_cast<int>(ast.offset(*n));
    ret["length"] = static_cast<int>(n->source().size());

    std::swap(ret, result);
    std::swap(childs, children);

    recurse(n);

    std::swap(ret, result);
    std::swap(childs, children);

    if (childs.size() > 0)
      ret["children"] = childs;

    return ret;
  }

  void visit(What w, script::ast::NodeRef n) override
  {
    if (n)
    {
      children.append(prod(n));
    }
  }

  QJsonObject prodtok(const script::parser::Token& tok)
  {
    QJsonObject ret;
    
    if (tok.flags & script::parser::Token::Keyword)
      ret["type"] = "kw";
    else if (tok.flags & script::parser::Token::Literal)
      ret["type"] = "lit";
    else if (tok.flags & script::parser::Token::Identifier)
      ret["type"] = "id";
    else if (tok.flags & script::parser::Token::OperatorToken)
      ret["type"] = "op";
    else if (tok.flags & script::parser::Token::Punctuator)
      ret["type"] = "p";
    else if (tok.flags & script::parser::Token::Punctuator)
      ret["type"] = "o";

    ret["offset"] = static_cast<int>(ast.offset(tok));
    ret["length"] = static_cast<int>(tok.text().size());
    ret["text"] = QString::fromStdString(tok.text().toString());

    return ret;
  }

  void visit(What w, script::parser::Token tok) override
  {
    children.append(prodtok(tok));
  }
};

} // namespace gonk