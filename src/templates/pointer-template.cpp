// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/templates/pointer-template.h"

#include <script/class.h>
#include <script/classtemplate.h>
#include <script/engine.h>
#include <script/namespace.h>
#include <script/templatebuilder.h>
#include <script/typesystem.h>

namespace gonk
{

void register_pointer_template(script::Namespace ns)
{
  using namespace script;

  std::vector<TemplateParameter> params;
  params.push_back(TemplateParameter{ TemplateParameter::TypeParameter{}, "T" });

  ClassTemplate ref = Symbol{ ns }.newClassTemplate("Pointer")
    .setParams(std::move(params))
    .setScope(Scope{ ns })
    .withBackend<gonk::PointerTemplate>()
    .get();
}

script::Class PointerTemplate::instantiate(script::ClassTemplateInstanceBuilder& builder)
{
  return script::Class{};
}

} // namespace gonk
