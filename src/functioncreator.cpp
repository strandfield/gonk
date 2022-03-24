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

FunctionCreator::FunctionCreator(script::Engine* e)
  : m_engine(e)
{

}

FunctionCreator::~FunctionCreator()
{

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

script::Function FunctionCreator::create(script::FunctionBlueprint& blueprint, const std::shared_ptr<script::ast::FunctionDecl>& fdecl, std::vector<script::Attribute>& attrs)
{
  std::string callback_name;

  if (get_callback_name(attrs, callback_name))
  {

    void(*proc)() = nullptr;

    std::string key = stringify(blueprint.prototype_);

    auto it = m_creators.find(key);

    if(it == m_creators.end())
    {
      std::cerr << "Unknown signature: " << key << std::endl;
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
  m_creators[key] = std::move(creator);
}

} // namespace gonk
