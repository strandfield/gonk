// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "std-vector-defs.h"

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

#include <vector>

namespace script {
template<> struct make_type_helper<std::vector<int>> { inline static script::Type get() { return (gonk::std_vector::class_type_id_offset() + gonk::std_vector::ClassTypeIds::VectorInt) | script::Type::ObjectFlag; } };
} // namespace script

namespace gonk
{

class VectorTemplate : public script::ClassTemplateNativeBackend
{
public:

  struct InstanceInfo : public script::UserData
  {
    std::shared_ptr<TypeInfo> element_type;
  };

  static InstanceInfo& info(script::FunctionCall* c);

  script::Class instantiate(script::ClassTemplateInstanceBuilder& builder) override;
};

namespace std_vector
{

namespace callbacks
{

template<typename T>
int size(const std::vector<T>& self)
{
  return static_cast<int>(self.size());
}

template<typename T>
void assign(std::vector<T>& self, int count, const T& value)
{
  self.assign(static_cast<size_t>(count), value);
}

template<typename T>
T at(const std::vector<T>& self, int index)
{
  return self.at(static_cast<size_t>(index));
}

template<typename T>
T front_const(const std::vector<T>& self)
{
  return self.front();
}

template<typename T>
T& front(std::vector<T>& self)
{
  return self.front();
}

template<typename T>
T back_const(const std::vector<T>& self)
{
  return self.back();
}

template<typename T>
T& back(std::vector<T>& self)
{
  return self.back();
}

template<typename T>
void reserve(std::vector<T>& self, int capacity)
{
  self.reserve(static_cast<size_t>(capacity));
}

template<typename T>
int capacity(const std::vector<T>& self)
{
  return static_cast<int>(self.capacity());
}

// @TODO: remove once iterators are supported
template<typename T>
void insert(std::vector<T>& self, int index, const T& value)
{
  self.insert(self.begin() + static_cast<size_t>(index), value);
}

// @TODO: remove once iterators are supported
template<typename T>
void erase(std::vector<T>& self, int index)
{
  self.erase(self.begin() + static_cast<size_t>(index));
}

template<typename T>
void resize_int(std::vector<T>& self, int size)
{
  self.resize(static_cast<size_t>(size));
}

template<typename T>
void resize_int_T(std::vector<T>& self, int size, const T& value)
{
  self.resize(static_cast<size_t>(size), value);
}

} // namespace callbacks

template<typename T>
void fill_instance(script::Class& c)
{
  // std::vector();
  gonk::bind::default_constructor<std::vector<T>>(c).create();
  // std::vector(const std::vector<T>& other);
  gonk::bind::copy_constructor<std::vector<T>>(c).create();
  // ~std::vector();
  gonk::bind::destructor<std::vector<T>>(c).create();

  // std::vector(int count);
  gonk::bind::constructor<std::vector<T>, int>(c).create();
  // std::vector(int size, const T& value);
  gonk::bind::constructor<std::vector<T>, int, const T&>(c).create();

  // std::vector<T>& operator=(const std::vector<T>& other);
  gonk::bind::memop_assign<std::vector<T>, const std::vector<T>&>(c);
  // void assign(int count, const T& value);
  gonk::bind::void_fn_as_memfn<std::vector<T>, int, const T&, &callbacks::assign<T>>(c, "assign").create();

  // T at(int index) const;
  gonk::bind::fn_as_memfn<std::vector<T>, T, int, &callbacks::at<T>>(c, "at").create();
  // T& operator[](int index);
  gonk::bind::memop_subscript<std::vector<T>, T&, int>(c);
  // T front() const;
  gonk::bind::fn_as_memfn<std::vector<T>, T, &callbacks::front_const<T>>(c, "front").create();
  // T& front();
  gonk::bind::fn_as_memfn<std::vector<T>, T&, &callbacks::front<T>>(c, "front").create();
  // T back() const;
  gonk::bind::fn_as_memfn<std::vector<T>, T, &callbacks::back_const<T>>(c, "back").create();
  // T& back();
  gonk::bind::fn_as_memfn<std::vector<T>, T&, &callbacks::back<T>>(c, "back").create();

  // bool empty() const;
  gonk::bind::member_function<std::vector<T>, bool, &std::vector<T>::empty>(c, "empty").create();
  // int size() const;
  gonk::bind::fn_as_memfn<std::vector<T>, int, &callbacks::size<T>>(c, "size").create();
  // void reserve(int capacity);
  gonk::bind::void_fn_as_memfn<std::vector<T>, int, &callbacks::reserve<T>>(c, "reserve").create();
  // int capacity() const;
  gonk::bind::fn_as_memfn<std::vector<T>, int, &callbacks::capacity<T>>(c, "capacity").create();
  // void shrink_to_fit();
  gonk::bind::void_member_function<std::vector<T>, &std::vector<T>::shrink_to_fit>(c, "shrink_to_fit").create();

  // void clear();
  gonk::bind::void_member_function<std::vector<T>, &std::vector<T>::clear>(c, "clear").create();
  // void insert(int index, const T& value);
  gonk::bind::void_fn_as_memfn<std::vector<T>, int, const T&, &callbacks::insert<T>>(c, "insert").create();
  // void erase(int index);
  gonk::bind::void_fn_as_memfn<std::vector<T>, int, &callbacks::erase<T>>(c, "erase").create();
  // void push_back(const T& value);
  gonk::bind::void_member_function<std::vector<T>, const T&, &std::vector<T>::push_back>(c, "push_back").create();
  // void pop_back();
  gonk::bind::void_member_function<std::vector<T>, &std::vector<T>::pop_back>(c, "pop_back").create();
  // void resize(int size);
  gonk::bind::void_fn_as_memfn<std::vector<T>, int, &callbacks::resize_int<T>>(c, "resize").create();
  // void resize(int size, const T& value);
  gonk::bind::void_fn_as_memfn<std::vector<T>, int, const T&, &callbacks::resize_int_T<T>>(c, "resize").create();
}

template<typename T>
void register_specialization(script::ClassTemplate vector_template, script::Type type_id)
{
  using namespace script;

  Class vector = vector_template.engine()->typeSystem()->getClass(type_id);
  if (!vector.isNull() && vector.id() == type_id.data())
    return;

  std::vector<TemplateArgument> targs{
    TemplateArgument{ script::make_type<T>() }
  };

  vector = vector_template.Specialization(std::move(targs))
    .setId(type_id.data())
    .setFinal()
    .get();

  fill_instance<T>(vector);

  script::Namespace ns = vector.enclosingNamespace();

  // bool operator==(const std::vector<T>& lhs, const std::vector<T>& rhs);
  gonk::bind::op_eq<const std::vector<T>&, const std::vector<T>&>(ns);
  // bool operator!=(const std::vector<T>& lhs, const std::vector<T>& rhs);
  gonk::bind::op_neq<const std::vector<T>&, const std::vector<T>&>(ns);
}

} // namespace std_vector

} // namespace gonk