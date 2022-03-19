// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_COMMON_BINDING_POINTER_H
#define GONK_COMMON_BINDING_POINTER_H

#include "gonk/templates/pointer-template.h"

#include "gonk/common/binding/class.h"
#include "gonk/common/binding/conversion.h"

#include <script/classtemplate.h>
#include <script/classtemplatespecializationbuilder.h>

namespace gonk
{

namespace bind
{

template<typename T>
void pointer(script::Engine* e)
{
  using namespace script;

  using PointerType = gonk::Pointer<T>;

  script::Type type_id = e->registerType<gonk::Pointer<T>>();
  Class ptr_type = e->typeSystem()->getClass(type_id);
  if (!ptr_type.isNull() && type_id == ptr_type.id())
    return;

  auto ptr_template = script::ClassTemplate::get<PointerTemplate>(e);

  std::vector<TemplateArgument> targs{
    TemplateArgument{ make_type<T>(e) },
  };

  ptr_type = ptr_template.Specialization(std::move(targs))
    .setId(type_id.data())
    .setFinal().get();

  // Pointer<T>();
  gonk::bind::default_constructor<PointerType>(ptr_type).create();
  // Pointer<T>(const Pointer<T> &);
  gonk::bind::copy_constructor<Pointer<T>>(ptr_type).create();
  // ~Pointer<T>();
  gonk::bind::destructor<Pointer<T>>(ptr_type).create();

  // Pointer<T>(T&);
  gonk::bind::constructor<Pointer<T>, T&>(ptr_type).create();

  // operator T&() const;
  gonk::bind::conversion<const Pointer<T>, T&>(ptr_type);
}

} // namespace bind

} // namespace gonk

#endif // GONK_COMMON_BINDING_POINTER_H
