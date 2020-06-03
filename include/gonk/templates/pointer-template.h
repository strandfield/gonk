// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_TEMPLATES_POINTER_H
#define GONK_TEMPLATES_POINTER_H

#include "gonk/common/pointer.h"

#include <script/classtemplatenativebackend.h>

namespace gonk
{

GONK_API void register_pointer_template(script::Namespace ns);

class PointerTemplate : public script::ClassTemplateNativeBackend
{
  script::Class instantiate(script::ClassTemplateInstanceBuilder& builder) override;
};

} // namespace gonk

#endif // GONK_TEMPLATES_POINTER_H
