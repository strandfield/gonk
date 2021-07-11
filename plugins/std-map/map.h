// Copyright (C) 2021 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "std-map-defs.h"

#include "gonk/common/binding/class.h"
#include "gonk/common/semvalue.h"
#include "gonk/common/types.h"

#include <script/interpreter/executioncontext.h>

#include <script/classtemplate.h>
#include <script/classtemplatenativebackend.h>
#include <script/classtemplatespecializationbuilder.h>
#include <script/constructorbuilder.h>
#include <script/destructorbuilder.h>
#include <script/engine.h>
#include <script/typesystem.h>

#include <map>

namespace gonk
{

namespace map
{

class GONK_STD_MAP_API KeyType
{
public:
  script::Engine* engine;
  script::Type type;
  script::Function assign;
  script::Function less;
  script::Function eq;

public:

  static std::map<int, std::shared_ptr<KeyType>>& get_map();
  static std::shared_ptr<KeyType> get(script::Engine* e, const script::Type& t);
};

class GONK_STD_MAP_API Key
{
public:
  std::shared_ptr<KeyType> type;
  script::Value value;

public:
  Key();
  Key(const Key& other);
  Key(Key&& other);
  ~Key();

  Key& operator=(const Key& other);

  script::Engine* engine() const;
};

class GONK_STD_MAP_API FakeKey : public Key
{
public:
  FakeKey(script::FunctionCall* c, script::Value val);
  ~FakeKey();
};

GONK_STD_MAP_API bool operator<(const Key& lhs, const Key& rhs);
GONK_STD_MAP_API bool operator==(const Key& lhs, const Key& rhs);

class GONK_STD_MAP_API ElementType
{
public:
  script::Engine* engine;
  script::Type type;
  script::Function assign;
  script::Function eq;

public:
  static std::map<int, std::shared_ptr<ElementType>>& get_map();
  static std::shared_ptr<ElementType> get(script::Engine* e, const script::Type& t);
};

class GONK_STD_MAP_API Element
{
public:
  std::shared_ptr<ElementType> type;
  script::Value value;

public:
  Element();
  Element(const Element& other);
  Element(Element&& other);
  ~Element();

  Element(std::shared_ptr<ElementType> type);
  Element(std::shared_ptr<ElementType> type, script::Value&& value);

  Element& operator=(const Element& other);

  script::Engine* engine() const;
};

GONK_STD_MAP_API bool operator==(const Element& lhs, const Element& rhs);
GONK_STD_MAP_API bool operator!=(const Element& lhs, const Element& rhs);

} // namespace map

using Map = std::map<map::Key, map::Element>;

class GONK_STD_MAP_API MapTemplate : public script::ClassTemplateNativeBackend
{
public:

  struct InstanceInfo : public script::UserData
  {
    std::shared_ptr<map::KeyType> key_type;
    std::shared_ptr<map::ElementType> element_type;
  };

  static InstanceInfo& info(script::FunctionCall* c);

  script::Class instantiate(script::ClassTemplateInstanceBuilder& builder) override;
};

} // namespace gonk