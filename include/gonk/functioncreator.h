// Copyright (C) 2022 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_FUNCTIONCREATOR_H
#define GONK_FUNCTIONCREATOR_H

#include "gonk/gonk-defs.h"

#include "gonk/common/values.h"

#include <script/interpreter/executioncontext.h>
#include <script/function-blueprint.h>
#include <script/function-impl.h>
#include <script/functioncreator.h>

#include <dynlib/dynlib.h>

#include <map>
#include <memory>
#include <string>

namespace gonk
{

class GONK_API CFunctionCreator
{
public:
  virtual ~CFunctionCreator();

  virtual script::Function create(void (*proc)(), script::FunctionBlueprint& blueprint) = 0;
};

class GONK_API FunctionCreator : public script::FunctionCreator
{
public:
  explicit FunctionCreator(script::Engine* e, dynlib::Library& lib);
  FunctionCreator(const FunctionCreator&) = delete;
  ~FunctionCreator();

  script::Function create(script::FunctionBlueprint& blueprint, const std::shared_ptr<script::ast::FunctionDecl>& fdecl, std::vector<script::Attribute>& attrs) override;

  template<typename R, typename... Args>
  void addCreator();

  template<typename R, typename... Args>
  void addCtorCreator();

  FunctionCreator& operator=(const FunctionCreator&) = delete;

protected:
  std::string stringify(const script::Prototype& proto);
  void addCreator(const std::string& key, std::unique_ptr<CFunctionCreator> creator);
  void addCCreator(const std::string& key, std::unique_ptr<CFunctionCreator> creator);
  script::Function create_function(script::FunctionBlueprint& blueprint, const std::shared_ptr<script::ast::FunctionDecl>& fdecl, std::vector<script::Attribute>& attrs);
  script::Function create_ctor(script::FunctionBlueprint& blueprint, const std::shared_ptr<script::ast::FunctionDecl>& fdecl, std::vector<script::Attribute>& attrs);

private:
  script::Engine* m_engine;
  dynlib::Library& m_library;
  std::map<std::string, std::unique_ptr<CFunctionCreator>> m_function_creators;
  std::map<std::string, std::unique_ptr<CFunctionCreator>> m_ctor_creators;
};

template<typename T, typename... Args>
class CFunction : public script::FunctionImpl
{
public:
  T(*proc)(Args...);
  script::Name m_name;
  script::DynamicPrototype m_proto;

public:
  explicit CFunction(script::FunctionBlueprint& blueprint, T(*fun)(Args...))
    : FunctionImpl(blueprint.engine()),
      proc(fun),
      m_name(blueprint.name())
  {
    enclosing_symbol = blueprint.parent().impl();
    //m_proto.setReturnType(gonk::make_type<T>(e));
    //m_proto.set({ gonk::make_type<Args>(e)... });
    m_proto = blueprint.prototype_;
    flags = blueprint.flags();
  }

  script::SymbolKind get_kind() const override
  {
    return m_name.kind();
  }

  const std::string& name() const override
  {
    return m_name.string();
  }

  script::Name get_name() const override
  {
    return m_name;
  }

  bool is_native() const override
  {
    return true;
  }

  void set_body(std::shared_ptr<script::program::Statement>) override
  {

  }

  const script::Prototype& prototype() const override
  {
    return m_proto;
  }

  template<std::size_t... Is>
  script::Value do_invoke(script::FunctionCall* c, std::index_sequence<Is...>, std::false_type)
  {
    using Tuple = std::tuple<Args...>;
    return gonk::make_value<T>(proc(gonk::value_cast<typename std::tuple_element<Is, Tuple>::type>(c->arg(Is))...), c->engine());
  }

  template<std::size_t... Is>
  script::Value do_invoke(script::FunctionCall* c, std::index_sequence<Is...>, std::true_type)
  {
    using Tuple = std::tuple<Args...>;
    proc(gonk::value_cast<typename std::tuple_element<Is, Tuple>::type>(c->arg(Is))...);
    return script::Value::Void;
  }

  script::Value invoke(script::FunctionCall* c) override
  {
    return do_invoke(c, std::index_sequence_for<Args...>{}, std::integral_constant<bool, std::is_void<T>::value>());
  }
};

template<typename T, typename...Args>
class CFunctionCreatorImpl : public CFunctionCreator
{
public:

  ~CFunctionCreatorImpl() = default;

  script::Function create(void (*proc)(), script::FunctionBlueprint& blueprint) override
  {
    return script::Function(std::make_shared<CFunction<T, Args...>>(blueprint, reinterpret_cast<T(*)(Args...)>(proc)));
  };
};

template<typename T, typename... Args>
class CConstructor : public script::FunctionImpl
{
public:
  script::Name m_name;
  script::DynamicPrototype m_proto;

public:
  explicit CConstructor(script::FunctionBlueprint& blueprint)
    : script::FunctionImpl(blueprint.engine(), blueprint.flags()),
      m_name(blueprint.name()),
      m_proto(blueprint.prototype_)
  {
    enclosing_symbol = blueprint.parent().impl();
  }

  script::SymbolKind get_kind() const override
  {
    return script::SymbolKind::Constructor;
  }

  const std::string& name() const override
  {
    return m_name.string();
  }

  script::Name get_name() const override
  {
    return m_name;
  }

  bool is_native() const override
  {
    return true;
  }

  void set_body(std::shared_ptr<script::program::Statement>) override
  {

  }

  const script::Prototype& prototype() const override
  {
    return m_proto;
  }

  template<std::size_t... Is>
  script::Value do_invoke(script::FunctionCall* c, std::index_sequence<Is...>)
  {
    using Tuple = std::tuple<Args...>;
    c->thisObject().init<T>(gonk::value_cast<typename std::tuple_element<Is, Tuple>::type>(c->arg(Is + 1))...);
    return c->arg(0);
  }

  script::Value invoke(script::FunctionCall* c) override
  {
    return do_invoke(c, std::index_sequence_for<Args...>{});
  }
};

template<typename T, typename...Args>
class CConstructorCreatorImpl : public CFunctionCreator
{
public:
  ~CConstructorCreatorImpl() = default;

  script::Function create(void (*proc)(), script::FunctionBlueprint& blueprint) override
  {
    return script::Function(std::make_shared<CConstructor<T, Args...>>(blueprint));
  };
};

template<typename R, typename... Args>
inline void FunctionCreator::addCreator()
{
  script::DynamicPrototype proto;
  proto.setReturnType(gonk::make_type<R>(m_engine));
  proto.set({ gonk::make_type<Args>(m_engine)... });

  std::string key = stringify(proto);

  auto creator = std::make_unique<CFunctionCreatorImpl<R, Args...>>();

  addCreator(key, std::move(creator));
}

template<typename R, typename... Args>
inline void FunctionCreator::addCtorCreator()
{
  script::DynamicPrototype proto;
  proto.setReturnType(script::Type::Void);
  proto.set({ gonk::make_type<R&>(m_engine), gonk::make_type<Args>(m_engine)... });

  std::string key = stringify(proto);

  auto creator = std::make_unique<CConstructorCreatorImpl<R, Args...>>();

  addCCreator(key, std::move(creator));
}


} // namespace gonk

#endif // GONK_FUNCTIONCREATOR_H
