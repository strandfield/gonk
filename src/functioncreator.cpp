// Copyright (C) 2022 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/functioncreator.h"

#include <script/ast/node.h>

#include <iostream>
#include <sstream>

namespace gonk
{

CFunctionCreator::~CFunctionCreator()
{

}

FunctionCreator::FunctionCreator(script::Engine* e, dynlib::Library& lib)
  : m_engine(e),
    m_library(lib)
{

}

FunctionCreator::~FunctionCreator()
{

}

script::Function FunctionCreator::create(script::FunctionBlueprint& blueprint, const std::shared_ptr<script::ast::FunctionDecl>& fdecl, std::vector<script::Attribute>& attrs)
{
  if (blueprint.name().kind() == script::SymbolKind::Constructor)
    return create_ctor(blueprint, fdecl, attrs);
  else
    return create_function(blueprint, fdecl, attrs);
}

std::string FunctionCreator::stringify(const script::Prototype& proto)
{
  std::stringstream ss;

  ss << m_engine->toString(proto.returnType());

  if (proto.size() > 0)
  {
    ss << ", ";

    for (int i(0); i < proto.count(); ++i)
    {
      ss <<  m_engine->toString(proto.at(i));
      if (i < proto.count() - 1)
        ss << ", ";
    }
  }

  return ss.str();
}

void FunctionCreator::addCreator(const std::string& key, std::unique_ptr<CFunctionCreator> creator)
{
  m_function_creators[key] = std::move(creator);
}

void FunctionCreator::addCCreator(const std::string& key, std::unique_ptr<CFunctionCreator> creator)
{
  m_ctor_creators[key] = std::move(creator);
}

static bool get_callback_name(std::vector<script::Attribute>& attrs, std::string& out)
{
  if (attrs.empty() || !attrs.front()->is<script::ast::FunctionCall>())
    return false;

  const auto& fcall = attrs.front()->as<script::ast::FunctionCall>();

  if (fcall.callee->source().toString() != "native" || fcall.arguments.size() != 1 || !fcall.arguments.at(0)->is<script::ast::StringLiteral>())
    return false;

  const auto& strliteral = fcall.arguments.front()->as<script::ast::StringLiteral>();
  out = strliteral.source().toString();
  out = std::string(out.begin() + 1, out.end() - 1);

  return true;
}

script::Function FunctionCreator::create_function(script::FunctionBlueprint& blueprint, const std::shared_ptr<script::ast::FunctionDecl>& fdecl, std::vector<script::Attribute>& attrs)
{
  std::string callback_name;

  if (get_callback_name(attrs, callback_name))
  {
    void(*proc)() = m_library.resolve(callback_name.c_str());

    if (!proc)
    {
      std::cerr << "Unknown procedure: " << callback_name << std::endl;
    }

    std::string key = stringify(blueprint.prototype_);

    auto it = m_function_creators.find(key);

    if (it == m_function_creators.end())
    {
      std::cerr << "Unknown function signature: " << key << std::endl;
      return script::FunctionCreator::create(blueprint, fdecl, attrs);
    }

    std::unique_ptr<CFunctionCreator>& creator = it->second;
    return creator->create(proc, blueprint);
  }
  else
  {
    return script::FunctionCreator::create(blueprint, fdecl, attrs);
  }
}

static bool ctor_has_native_attr(std::vector<script::Attribute>& attrs)
{
  if (attrs.empty())
    return false;

  if (attrs.front()->is<script::ast::StringLiteral>())
  {
    const auto& strlit = attrs.front()->as<script::ast::StringLiteral>();

    std::string str = strlit.source().toString();
    str = std::string(str.begin() + 1, str.end() - 1);

    return str == "native";
  }
  else if (attrs.front()->is<script::ast::SimpleIdentifier>())
  {
    const auto& simpleid = attrs.front()->as<script::ast::SimpleIdentifier>();

    std::string str = simpleid.source().toString();

    return str == "native";
  }

  return false;
}

script::Function FunctionCreator::create_ctor(script::FunctionBlueprint& blueprint, const std::shared_ptr<script::ast::FunctionDecl>& fdecl, std::vector<script::Attribute>& attrs)
{
  if (ctor_has_native_attr(attrs))
  {
    std::string key = stringify(blueprint.prototype_);

    auto it = m_ctor_creators.find(key);

    if (it == m_ctor_creators.end())
    {
      std::cerr << "Unknown ctor signature: " << key << std::endl;
      return script::FunctionCreator::create(blueprint, fdecl, attrs);
    }

    std::unique_ptr<CFunctionCreator>& creator = it->second;
    return creator->create(nullptr, blueprint);
  }
  else
  {
    return script::FunctionCreator::create(blueprint, fdecl, attrs);
  }
}

} // namespace gonk
