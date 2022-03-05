// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "regex.h"

#include "gonk/common/binding/class.h"
#include "gonk/common/binding/namespace.h"
#include "gonk/common/enums.h"

#include <script/classbuilder.h>
#include <script/enumbuilder.h>

static void register_regex_class(script::Namespace ns)
{
  using namespace script;

  Type t = ns.engine()->registerType<std::regex>();
  Class regex = ns.newClass("regex").setId(t.data()).get();

  // regex()
  gonk::bind::default_constructor<std::regex>(regex).create();
  // regex(const std::string& str);
  gonk::bind::constructor<std::regex, const std::string&>(regex).create();
  // ~regex();
  gonk::bind::destructor<std::regex>(regex).create();
  // regex& operator=(const regex& other);
  gonk::bind::memop_assign<std::regex, const std::regex&>(regex);
}

static bool regex_match(const std::string& s, const std::regex& e)
{
  return std::regex_match(s, e);
}

void register_regex_file(script::Namespace ns)
{
  using namespace script;

  register_regex_class(ns);

  // bool regex_match(const std::string& s, const std::regex& e);
  gonk::bind::free_function<bool, const std::string&, const std::regex&, &regex_match>(ns, "regex_match").create();
}
