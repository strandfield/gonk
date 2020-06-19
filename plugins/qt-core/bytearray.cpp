// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

// Warning: This file was generated by metagonk, all modifications will be lost. //

#include "bytearray.h"

#include "gonk/common/binding/class.h"
#include "gonk/common/binding/namespace.h"
#include "gonk/common/enums.h"

#include <script/classbuilder.h>
#include <script/enumbuilder.h>

#include "qt-core/bytearray.h"
#include "qt-core/string.h"

#if METAGONK_SOURCE
const char* module_name = "{{ project.modules[0].name }}";
{% assign bytearray_class = project | get_symbol: 'Qt.Core', 'QByteArray' %}
{% include generate_function with function = bytearray_class.members[4] %}
{% include generate_function with function = bytearray_class.members[8] %}
{% include generate_function with function = bytearray_class.members[14] %}
{% include generate_function with function = bytearray_class.members[16] %}
{% include generate_function with function = bytearray_class.members[9] %}
{% include generate_wrapper with function = bytearray_class.members[17] %}
#else
const char* module_name = "Qt.Core";

// QByteArray(int, char);
  gonk::bind::constructor<QByteArray, int, char>(c).create();

// ~QByteArray();
  gonk::bind::destructor<QByteArray>(c).create();

// int size() const;
  gonk::bind::member_function<QByteArray, int, &QByteArray::size>(c, "size").create();

// void resize(int);
  gonk::bind::void_member_function<QByteArray, int, &QByteArray::resize>(c, "resize").create();

// QByteArray& operator=(const QByteArray&);
  gonk::bind::memop_assign<QByteArray, const QByteArray&>(c).create();

#endif // METAGONK_SOURCE


namespace
{
script::Value bytearray_string_ctor(script::FunctionCall* c)
{
  c->thisObject().init<QByteArray>(script::get<std::string>(c->arg(1)).c_str());
  return c->thisObject();
}
}

static void register_byte_array_base64_option_enum(script::Class byte_array)
{
  using namespace script;

  Enum base64_option = byte_array.newEnum("Base64Option").setId(script::Type::make<QByteArray::Base64Option>().data()).get();

  base64_option.addValue("Base64Encoding", QByteArray::Base64Encoding);
  base64_option.addValue("Base64UrlEncoding", QByteArray::Base64UrlEncoding);
  base64_option.addValue("KeepTrailingEquals", QByteArray::KeepTrailingEquals);
  base64_option.addValue("OmitTrailingEquals", QByteArray::OmitTrailingEquals);
}


namespace {
QByteArray bytearray_toLower(const QByteArray& self)
{
  return self.toLower();
}

QByteArray bytearray_toUpper(const QByteArray& self)
{
  return self.toUpper();
}

QByteArray bytearray_trimmed(const QByteArray& self)
{
  return self.trimmed();
}

QByteArray bytearray_simplified(const QByteArray& self)
{
  return self.simplified();
}

}

static void register_byte_array_class(script::Namespace ns)
{
  using namespace script;

  Class byte_array = ns.newClass("QByteArray").setId(script::Type::make<QByteArray>().data()).get();

  register_byte_array_base64_option_enum(byte_array);

  // QByteArray::DataPtr& data_ptr();
  /// TODO: QByteArray::DataPtr& data_ptr();
  // QByteArray();
  gonk::bind::default_constructor<QByteArray>(byte_array).create();
  // QByteArray(const char*, int);
  /// TODO: QByteArray(const char*, int);
  // QByteArray(int, char);
  gonk::bind::constructor<QByteArray, int, char>(byte_array).create();
  // QByteArray(int, Qt::Initialization);
  /// TODO: QByteArray(int, Qt::Initialization);
  // QByteArray(const std::string&);
  byte_array.newConstructor(&bytearray_string_ctor).params(gonk::make_type<const std::string&>()).create();
  // QByteArray(const QByteArray&);
  gonk::bind::constructor<QByteArray, const QByteArray&>(byte_array).create();
  // ~QByteArray();
  gonk::bind::destructor<QByteArray>(byte_array).create();
  // QByteArray& operator=(const QByteArray&);
  gonk::bind::memop_assign<QByteArray, const QByteArray&>(byte_array);
  // QByteArray& operator=(const char*);
  /// TODO: QByteArray& operator=(const char*);
  // QByteArray(QByteArray&&);
  gonk::bind::constructor<QByteArray, QByteArray&&>(byte_array).create();
  // QByteArray& operator=(QByteArray&&);
  gonk::bind::memop_assign<QByteArray, QByteArray&&>(byte_array);
  // void swap(QByteArray&);
  gonk::bind::void_member_function<QByteArray, QByteArray&, &QByteArray::swap>(byte_array, "swap").create();
  // int size() const;
  gonk::bind::member_function<QByteArray, int, &QByteArray::size>(byte_array, "size").create();
  // bool isEmpty() const;
  gonk::bind::member_function<QByteArray, bool, &QByteArray::isEmpty>(byte_array, "isEmpty").create();
  // void resize(int);
  gonk::bind::void_member_function<QByteArray, int, &QByteArray::resize>(byte_array, "resize").create();
  // QByteArray& fill(char, int);
  gonk::bind::member_function<QByteArray, QByteArray&, char, int, &QByteArray::fill>(byte_array, "fill").create();
  // int capacity() const;
  gonk::bind::member_function<QByteArray, int, &QByteArray::capacity>(byte_array, "capacity").create();
  // void reserve(int);
  gonk::bind::void_member_function<QByteArray, int, &QByteArray::reserve>(byte_array, "reserve").create();
  // void squeeze();
  gonk::bind::void_member_function<QByteArray, &QByteArray::squeeze>(byte_array, "squeeze").create();
  // char* data();
  /// TODO: char* data();
  // const char* data() const;
  /// TODO: const char* data() const;
  // const char* constData() const;
  /// TODO: const char* constData() const;
  // void detach();
  gonk::bind::void_member_function<QByteArray, &QByteArray::detach>(byte_array, "detach").create();
  // bool isDetached() const;
  gonk::bind::member_function<QByteArray, bool, &QByteArray::isDetached>(byte_array, "isDetached").create();
  // bool isSharedWith(const QByteArray&) const;
  gonk::bind::member_function<QByteArray, bool, const QByteArray&, &QByteArray::isSharedWith>(byte_array, "isSharedWith").create();
  // void clear();
  gonk::bind::void_member_function<QByteArray, &QByteArray::clear>(byte_array, "clear").create();
  // char at(int) const;
  gonk::bind::member_function<QByteArray, char, int, &QByteArray::at>(byte_array, "at").create();
  // char operator[](int) const;
  gonk::bind::memop_const_subscript<QByteArray, char, int>(byte_array);
  // char operator[](uint) const;
  /// TODO: char operator[](uint) const;
  // QByteRef operator[](int);
  gonk::bind::memop_subscript<QByteArray, QByteRef, int>(byte_array);
  // QByteRef operator[](uint);
  /// TODO: QByteRef operator[](uint);
  // char front() const;
  gonk::bind::member_function<QByteArray, char, &QByteArray::front>(byte_array, "front").create();
  // QByteRef front();
  gonk::bind::member_function<QByteArray, QByteRef, &QByteArray::front>(byte_array, "front").create();
  // char back() const;
  gonk::bind::member_function<QByteArray, char, &QByteArray::back>(byte_array, "back").create();
  // QByteRef back();
  gonk::bind::member_function<QByteArray, QByteRef, &QByteArray::back>(byte_array, "back").create();
  // int indexOf(char, int) const;
  gonk::bind::member_function<QByteArray, int, char, int, &QByteArray::indexOf>(byte_array, "indexOf").create();
  // int indexOf(const char*, int) const;
  /// TODO: int indexOf(const char*, int) const;
  // int indexOf(const QByteArray&, int) const;
  gonk::bind::member_function<QByteArray, int, const QByteArray&, int, &QByteArray::indexOf>(byte_array, "indexOf").create();
  // int lastIndexOf(char, int) const;
  gonk::bind::member_function<QByteArray, int, char, int, &QByteArray::lastIndexOf>(byte_array, "lastIndexOf").create();
  // int lastIndexOf(const char*, int) const;
  /// TODO: int lastIndexOf(const char*, int) const;
  // int lastIndexOf(const QByteArray&, int) const;
  gonk::bind::member_function<QByteArray, int, const QByteArray&, int, &QByteArray::lastIndexOf>(byte_array, "lastIndexOf").create();
  // bool contains(char) const;
  gonk::bind::member_function<QByteArray, bool, char, &QByteArray::contains>(byte_array, "contains").create();
  // bool contains(const char*) const;
  /// TODO: bool contains(const char*) const;
  // bool contains(const QByteArray&) const;
  gonk::bind::member_function<QByteArray, bool, const QByteArray&, &QByteArray::contains>(byte_array, "contains").create();
  // int count(char) const;
  gonk::bind::member_function<QByteArray, int, char, &QByteArray::count>(byte_array, "count").create();
  // int count(const char*) const;
  /// TODO: int count(const char*) const;
  // int count(const QByteArray&) const;
  gonk::bind::member_function<QByteArray, int, const QByteArray&, &QByteArray::count>(byte_array, "count").create();
  // QByteArray left(int) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, &QByteArray::left>(byte_array, "left").create();
  // QByteArray right(int) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, &QByteArray::right>(byte_array, "right").create();
  // QByteArray mid(int, int) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, int, &QByteArray::mid>(byte_array, "mid").create();
  // QByteArray chopped(int) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, &QByteArray::chopped>(byte_array, "chopped").create();
  // bool startsWith(const QByteArray&) const;
  gonk::bind::member_function<QByteArray, bool, const QByteArray&, &QByteArray::startsWith>(byte_array, "startsWith").create();
  // bool startsWith(char) const;
  gonk::bind::member_function<QByteArray, bool, char, &QByteArray::startsWith>(byte_array, "startsWith").create();
  // bool startsWith(const char*) const;
  /// TODO: bool startsWith(const char*) const;
  // bool endsWith(const QByteArray&) const;
  gonk::bind::member_function<QByteArray, bool, const QByteArray&, &QByteArray::endsWith>(byte_array, "endsWith").create();
  // bool endsWith(char) const;
  gonk::bind::member_function<QByteArray, bool, char, &QByteArray::endsWith>(byte_array, "endsWith").create();
  // bool endsWith(const char*) const;
  /// TODO: bool endsWith(const char*) const;
  // void truncate(int);
  gonk::bind::void_member_function<QByteArray, int, &QByteArray::truncate>(byte_array, "truncate").create();
  // void chop(int);
  gonk::bind::void_member_function<QByteArray, int, &QByteArray::chop>(byte_array, "chop").create();
  // QByteArray toLower() const;
  gonk::bind::fn_as_memfn<QByteArray, QByteArray, &bytearray_toLower>(byte_array, "toLower").create();
  // QByteArray toUpper() const;
  gonk::bind::fn_as_memfn<QByteArray, QByteArray, &bytearray_toUpper>(byte_array, "toUpper").create();
  // QByteArray trimmed() const;
  gonk::bind::fn_as_memfn<QByteArray, QByteArray, &bytearray_trimmed>(byte_array, "trimmed").create();
  // QByteArray simplified() const;
  gonk::bind::fn_as_memfn<QByteArray, QByteArray, &bytearray_simplified>(byte_array, "simplified").create();
  // QByteArray leftJustified(int, char, bool) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, char, bool, &QByteArray::leftJustified>(byte_array, "leftJustified").create();
  // QByteArray rightJustified(int, char, bool) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, char, bool, &QByteArray::rightJustified>(byte_array, "rightJustified").create();
  // QByteArray& prepend(char);
  gonk::bind::member_function<QByteArray, QByteArray&, char, &QByteArray::prepend>(byte_array, "prepend").create();
  // QByteArray& prepend(int, char);
  gonk::bind::member_function<QByteArray, QByteArray&, int, char, &QByteArray::prepend>(byte_array, "prepend").create();
  // QByteArray& prepend(const char*);
  /// TODO: QByteArray& prepend(const char*);
  // QByteArray& prepend(const char*, int);
  /// TODO: QByteArray& prepend(const char*, int);
  // QByteArray& prepend(const QByteArray&);
  gonk::bind::member_function<QByteArray, QByteArray&, const QByteArray&, &QByteArray::prepend>(byte_array, "prepend").create();
  // QByteArray& append(char);
  gonk::bind::member_function<QByteArray, QByteArray&, char, &QByteArray::append>(byte_array, "append").create();
  // QByteArray& append(int, char);
  gonk::bind::member_function<QByteArray, QByteArray&, int, char, &QByteArray::append>(byte_array, "append").create();
  // QByteArray& append(const char*);
  /// TODO: QByteArray& append(const char*);
  // QByteArray& append(const char*, int);
  /// TODO: QByteArray& append(const char*, int);
  // QByteArray& append(const QByteArray&);
  gonk::bind::member_function<QByteArray, QByteArray&, const QByteArray&, &QByteArray::append>(byte_array, "append").create();
  // QByteArray& insert(int, char);
  gonk::bind::member_function<QByteArray, QByteArray&, int, char, &QByteArray::insert>(byte_array, "insert").create();
  // QByteArray& insert(int, int, char);
  gonk::bind::member_function<QByteArray, QByteArray&, int, int, char, &QByteArray::insert>(byte_array, "insert").create();
  // QByteArray& insert(int, const char*);
  /// TODO: QByteArray& insert(int, const char*);
  // QByteArray& insert(int, const char*, int);
  /// TODO: QByteArray& insert(int, const char*, int);
  // QByteArray& insert(int, const QByteArray&);
  gonk::bind::member_function<QByteArray, QByteArray&, int, const QByteArray&, &QByteArray::insert>(byte_array, "insert").create();
  // QByteArray& remove(int, int);
  gonk::bind::member_function<QByteArray, QByteArray&, int, int, &QByteArray::remove>(byte_array, "remove").create();
  // QByteArray& replace(int, int, const char*);
  /// TODO: QByteArray& replace(int, int, const char*);
  // QByteArray& replace(int, int, const char*, int);
  /// TODO: QByteArray& replace(int, int, const char*, int);
  // QByteArray& replace(int, int, const QByteArray&);
  gonk::bind::member_function<QByteArray, QByteArray&, int, int, const QByteArray&, &QByteArray::replace>(byte_array, "replace").create();
  // QByteArray& replace(char, const char*);
  /// TODO: QByteArray& replace(char, const char*);
  // QByteArray& replace(char, const QByteArray&);
  gonk::bind::member_function<QByteArray, QByteArray&, char, const QByteArray&, &QByteArray::replace>(byte_array, "replace").create();
  // QByteArray& replace(const char*, const char*);
  /// TODO: QByteArray& replace(const char*, const char*);
  // QByteArray& replace(const char*, int, const char*, int);
  /// TODO: QByteArray& replace(const char*, int, const char*, int);
  // QByteArray& replace(const QByteArray&, const QByteArray&);
  gonk::bind::member_function<QByteArray, QByteArray&, const QByteArray&, const QByteArray&, &QByteArray::replace>(byte_array, "replace").create();
  // QByteArray& replace(const QByteArray&, const char*);
  /// TODO: QByteArray& replace(const QByteArray&, const char*);
  // QByteArray& replace(const char*, const QByteArray&);
  /// TODO: QByteArray& replace(const char*, const QByteArray&);
  // QByteArray& replace(char, char);
  gonk::bind::member_function<QByteArray, QByteArray&, char, char, &QByteArray::replace>(byte_array, "replace").create();
  // QByteArray& operator+=(char);
  gonk::bind::memop_add_assign<QByteArray, char>(byte_array);
  // QByteArray& operator+=(const char*);
  /// TODO: QByteArray& operator+=(const char*);
  // QByteArray& operator+=(const QByteArray&);
  gonk::bind::memop_add_assign<QByteArray, const QByteArray&>(byte_array);
  // QList<QByteArray> split(char) const;
  /// TODO: QList<QByteArray> split(char) const;
  // QByteArray repeated(int) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, &QByteArray::repeated>(byte_array, "repeated").create();
  // QByteArray& append(const QString&);
  gonk::bind::member_function<QByteArray, QByteArray&, const QString&, &QByteArray::append>(byte_array, "append").create();
  // QByteArray& insert(int, const QString&);
  gonk::bind::member_function<QByteArray, QByteArray&, int, const QString&, &QByteArray::insert>(byte_array, "insert").create();
  // QByteArray& replace(const QString&, const char*);
  /// TODO: QByteArray& replace(const QString&, const char*);
  // QByteArray& replace(char, const QString&);
  gonk::bind::member_function<QByteArray, QByteArray&, char, const QString&, &QByteArray::replace>(byte_array, "replace").create();
  // QByteArray& replace(const QString&, const QByteArray&);
  gonk::bind::member_function<QByteArray, QByteArray&, const QString&, const QByteArray&, &QByteArray::replace>(byte_array, "replace").create();
  // QByteArray& operator+=(const QString&);
  gonk::bind::memop_add_assign<QByteArray, const QString&>(byte_array);
  // int indexOf(const QString&, int) const;
  gonk::bind::member_function<QByteArray, int, const QString&, int, &QByteArray::indexOf>(byte_array, "indexOf").create();
  // int lastIndexOf(const QString&, int) const;
  gonk::bind::member_function<QByteArray, int, const QString&, int, &QByteArray::lastIndexOf>(byte_array, "lastIndexOf").create();
  // bool operator==(const QString&) const;
  gonk::bind::memop_eq<QByteArray, const QString&>(byte_array);
  // bool operator!=(const QString&) const;
  gonk::bind::memop_neq<QByteArray, const QString&>(byte_array);
  // bool operator<(const QString&) const;
  gonk::bind::memop_less<QByteArray, const QString&>(byte_array);
  // bool operator>(const QString&) const;
  gonk::bind::memop_greater<QByteArray, const QString&>(byte_array);
  // bool operator<=(const QString&) const;
  gonk::bind::memop_leq<QByteArray, const QString&>(byte_array);
  // bool operator>=(const QString&) const;
  gonk::bind::memop_geq<QByteArray, const QString&>(byte_array);
  // short toShort(bool*, int) const;
  /// TODO: short toShort(bool*, int) const;
  // ushort toUShort(bool*, int) const;
  /// TODO: ushort toUShort(bool*, int) const;
  // int toInt(bool*, int) const;
  /// TODO: int toInt(bool*, int) const;
  // uint toUInt(bool*, int) const;
  /// TODO: uint toUInt(bool*, int) const;
  // long toLong(bool*, int) const;
  /// TODO: long toLong(bool*, int) const;
  // ulong toULong(bool*, int) const;
  /// TODO: ulong toULong(bool*, int) const;
  // qlonglong toLongLong(bool*, int) const;
  /// TODO: qlonglong toLongLong(bool*, int) const;
  // qulonglong toULongLong(bool*, int) const;
  /// TODO: qulonglong toULongLong(bool*, int) const;
  // float toFloat(bool*) const;
  /// TODO: float toFloat(bool*) const;
  // double toDouble(bool*) const;
  /// TODO: double toDouble(bool*) const;
  // QByteArray toBase64(QByteArray::Base64Options) const;
  /// TODO: QByteArray toBase64(QByteArray::Base64Options) const;
  // QByteArray toBase64() const;
  gonk::bind::member_function<QByteArray, QByteArray, &QByteArray::toBase64>(byte_array, "toBase64").create();
  // QByteArray toHex() const;
  gonk::bind::member_function<QByteArray, QByteArray, &QByteArray::toHex>(byte_array, "toHex").create();
  // QByteArray toHex(char) const;
  gonk::bind::member_function<QByteArray, QByteArray, char, &QByteArray::toHex>(byte_array, "toHex").create();
  // QByteArray toPercentEncoding(const QByteArray&, const QByteArray&, char) const;
  gonk::bind::member_function<QByteArray, QByteArray, const QByteArray&, const QByteArray&, char, &QByteArray::toPercentEncoding>(byte_array, "toPercentEncoding").create();
  // QByteArray& setNum(short, int);
  /// TODO: QByteArray& setNum(short, int);
  // QByteArray& setNum(ushort, int);
  /// TODO: QByteArray& setNum(ushort, int);
  // QByteArray& setNum(int, int);
  gonk::bind::member_function<QByteArray, QByteArray&, int, int, &QByteArray::setNum>(byte_array, "setNum").create();
  // QByteArray& setNum(uint, int);
  /// TODO: QByteArray& setNum(uint, int);
  // QByteArray& setNum(qlonglong, int);
  /// TODO: QByteArray& setNum(qlonglong, int);
  // QByteArray& setNum(qulonglong, int);
  /// TODO: QByteArray& setNum(qulonglong, int);
  // QByteArray& setNum(float, char, int);
  gonk::bind::member_function<QByteArray, QByteArray&, float, char, int, &QByteArray::setNum>(byte_array, "setNum").create();
  // QByteArray& setNum(double, char, int);
  gonk::bind::member_function<QByteArray, QByteArray&, double, char, int, &QByteArray::setNum>(byte_array, "setNum").create();
  // QByteArray& setRawData(const char*, uint);
  /// TODO: QByteArray& setRawData(const char*, uint);
  // static QByteArray number(int, int);
  gonk::bind::static_member_function<QByteArray, QByteArray, int, int, &QByteArray::number>(byte_array, "number").create();
  // static QByteArray number(uint, int);
  /// TODO: static QByteArray number(uint, int);
  // static QByteArray number(qlonglong, int);
  /// TODO: static QByteArray number(qlonglong, int);
  // static QByteArray number(qulonglong, int);
  /// TODO: static QByteArray number(qulonglong, int);
  // static QByteArray number(double, char, int);
  gonk::bind::static_member_function<QByteArray, QByteArray, double, char, int, &QByteArray::number>(byte_array, "number").create();
  // static QByteArray fromRawData(const char*, int);
  /// TODO: static QByteArray fromRawData(const char*, int);
  // static QByteArray fromBase64(const QByteArray&, QByteArray::Base64Options);
  /// TODO: static QByteArray fromBase64(const QByteArray&, QByteArray::Base64Options);
  // static QByteArray fromBase64(const QByteArray&);
  gonk::bind::static_member_function<QByteArray, QByteArray, const QByteArray&, &QByteArray::fromBase64>(byte_array, "fromBase64").create();
  // static QByteArray fromHex(const QByteArray&);
  gonk::bind::static_member_function<QByteArray, QByteArray, const QByteArray&, &QByteArray::fromHex>(byte_array, "fromHex").create();
  // static QByteArray fromPercentEncoding(const QByteArray&, char);
  gonk::bind::static_member_function<QByteArray, QByteArray, const QByteArray&, char, &QByteArray::fromPercentEncoding>(byte_array, "fromPercentEncoding").create();
  // QByteArray::iterator begin();
  /// TODO: QByteArray::iterator begin();
  // QByteArray::const_iterator begin() const;
  /// TODO: QByteArray::const_iterator begin() const;
  // QByteArray::const_iterator cbegin() const;
  /// TODO: QByteArray::const_iterator cbegin() const;
  // QByteArray::const_iterator constBegin() const;
  /// TODO: QByteArray::const_iterator constBegin() const;
  // QByteArray::iterator end();
  /// TODO: QByteArray::iterator end();
  // QByteArray::const_iterator end() const;
  /// TODO: QByteArray::const_iterator end() const;
  // QByteArray::const_iterator cend() const;
  /// TODO: QByteArray::const_iterator cend() const;
  // QByteArray::const_iterator constEnd() const;
  /// TODO: QByteArray::const_iterator constEnd() const;
  // QByteArray::reverse_iterator rbegin();
  /// TODO: QByteArray::reverse_iterator rbegin();
  // QByteArray::reverse_iterator rend();
  /// TODO: QByteArray::reverse_iterator rend();
  // QByteArray::const_reverse_iterator rbegin() const;
  /// TODO: QByteArray::const_reverse_iterator rbegin() const;
  // QByteArray::const_reverse_iterator rend() const;
  /// TODO: QByteArray::const_reverse_iterator rend() const;
  // QByteArray::const_reverse_iterator crbegin() const;
  /// TODO: QByteArray::const_reverse_iterator crbegin() const;
  // QByteArray::const_reverse_iterator crend() const;
  /// TODO: QByteArray::const_reverse_iterator crend() const;
  // void push_back(char);
  gonk::bind::void_member_function<QByteArray, char, &QByteArray::push_back>(byte_array, "push_back").create();
  // void push_back(const char*);
  /// TODO: void push_back(const char*);
  // void push_back(const QByteArray&);
  gonk::bind::void_member_function<QByteArray, const QByteArray&, &QByteArray::push_back>(byte_array, "push_back").create();
  // void push_front(char);
  gonk::bind::void_member_function<QByteArray, char, &QByteArray::push_front>(byte_array, "push_front").create();
  // void push_front(const char*);
  /// TODO: void push_front(const char*);
  // void push_front(const QByteArray&);
  gonk::bind::void_member_function<QByteArray, const QByteArray&, &QByteArray::push_front>(byte_array, "push_front").create();
  // void shrink_to_fit();
  gonk::bind::void_member_function<QByteArray, &QByteArray::shrink_to_fit>(byte_array, "shrink_to_fit").create();
  // static QByteArray fromStdString(const std::string&);
  gonk::bind::static_member_function<QByteArray, QByteArray, const std::string&, &QByteArray::fromStdString>(byte_array, "fromStdString").create();
  // std::string toStdString() const;
  gonk::bind::member_function<QByteArray, std::string, &QByteArray::toStdString>(byte_array, "toStdString").create();
  // int count() const;
  gonk::bind::member_function<QByteArray, int, &QByteArray::count>(byte_array, "count").create();
  // int length() const;
  gonk::bind::member_function<QByteArray, int, &QByteArray::length>(byte_array, "length").create();
  // bool isNull() const;
  gonk::bind::member_function<QByteArray, bool, &QByteArray::isNull>(byte_array, "isNull").create();
  // QByteArray(QByteArrayDataPtr);
  /// TODO: QByteArray(QByteArrayDataPtr);
}


static void register_byte_ref_class(script::Namespace ns)
{
  using namespace script;

  Class byte_ref = ns.newClass("QByteRef").setId(script::Type::make<QByteRef>().data()).get();


  // QByteRef& operator=(char);
  gonk::bind::memop_assign<QByteRef, char>(byte_ref);
  // QByteRef& operator=(const QByteRef&);
  gonk::bind::memop_assign<QByteRef, const QByteRef&>(byte_ref);
  // bool operator==(char) const;
  gonk::bind::memop_eq<QByteRef, char>(byte_ref);
  // bool operator!=(char) const;
  gonk::bind::memop_neq<QByteRef, char>(byte_ref);
  // bool operator>(char) const;
  gonk::bind::memop_greater<QByteRef, char>(byte_ref);
  // bool operator>=(char) const;
  gonk::bind::memop_geq<QByteRef, char>(byte_ref);
  // bool operator<(char) const;
  gonk::bind::memop_less<QByteRef, char>(byte_ref);
  // bool operator<=(char) const;
  gonk::bind::memop_leq<QByteRef, char>(byte_ref);
}


namespace {
int bytearray_qstrcmp(const QByteArray& a1, const QByteArray& a2)
{
  return qstrcmp(a1, a2);
}

QByteArray bytearray_qCompress(const QByteArray& a1, int a2)
{
  return qCompress(a1, a2);
}

QByteArray bytearray_qUncompress(const QByteArray& a1)
{
  return qUncompress(a1);
}

}

void register_bytearray_file(script::Namespace ns)
{
  using namespace script;


  register_byte_array_class(ns);
  register_byte_ref_class(ns);

  // char* qstrdup(const char*);
  /// TODO: char* qstrdup(const char*);
  // uint qstrlen(const char*);
  /// TODO: uint qstrlen(const char*);
  // uint qstrnlen(const char*, uint);
  /// TODO: uint qstrnlen(const char*, uint);
  // char* qstrcpy(char*, const char*);
  /// TODO: char* qstrcpy(char*, const char*);
  // char* qstrncpy(char*, const char*, uint);
  /// TODO: char* qstrncpy(char*, const char*, uint);
  // int qstrcmp(const char*, const char*);
  /// TODO: int qstrcmp(const char*, const char*);
  // int qstrcmp(const QByteArray&, const QByteArray&);
  gonk::bind::free_function<int, const QByteArray&, const QByteArray&, &bytearray_qstrcmp>(ns, "qstrcmp").create();
  // int qstrcmp(const QByteArray&, const char*);
  /// TODO: int qstrcmp(const QByteArray&, const char*);
  // int qstrcmp(const char*, const QByteArray&);
  /// TODO: int qstrcmp(const char*, const QByteArray&);
  // int qstrncmp(const char*, const char*, uint);
  /// TODO: int qstrncmp(const char*, const char*, uint);
  // int qstricmp(const char*, const char*);
  /// TODO: int qstricmp(const char*, const char*);
  // int qstrnicmp(const char*, const char*, uint);
  /// TODO: int qstrnicmp(const char*, const char*, uint);
  // int qvsnprintf(char*, size_t, const char*, va_list);
  /// TODO: int qvsnprintf(char*, size_t, const char*, va_list);
  // int qsnprintf(char*, size_t, const char*);
  /// TODO: int qsnprintf(char*, size_t, const char*);
  // quint16 qChecksum(const char*, uint);
  /// TODO: quint16 qChecksum(const char*, uint);
  // quint16 qChecksum(const char*, uint, Qt::ChecksumType);
  /// TODO: quint16 qChecksum(const char*, uint, Qt::ChecksumType);
  // QFlags<QByteArray::Base64Options::enum_type> operator|(QByteArray::Base64Options::enum_type, QByteArray::Base64Options::enum_type);
  /// TODO: QFlags<QByteArray::Base64Options::enum_type> operator|(QByteArray::Base64Options::enum_type, QByteArray::Base64Options::enum_type);
  // QFlags<QByteArray::Base64Options::enum_type> operator|(QByteArray::Base64Options::enum_type, QFlags<QByteArray::Base64Options::enum_type>);
  /// TODO: QFlags<QByteArray::Base64Options::enum_type> operator|(QByteArray::Base64Options::enum_type, QFlags<QByteArray::Base64Options::enum_type>);
  // QIncompatibleFlag operator|(QByteArray::Base64Options::enum_type, int);
  /// TODO: QIncompatibleFlag operator|(QByteArray::Base64Options::enum_type, int);
  // bool operator==(const QByteArray&, const QByteArray&);
  gonk::bind::op_eq<const QByteArray&, const QByteArray&>(ns);
  // bool operator==(const QByteArray&, const char*);
  /// TODO: bool operator==(const QByteArray&, const char*);
  // bool operator==(const char*, const QByteArray&);
  /// TODO: bool operator==(const char*, const QByteArray&);
  // bool operator!=(const QByteArray&, const QByteArray&);
  gonk::bind::op_neq<const QByteArray&, const QByteArray&>(ns);
  // bool operator!=(const QByteArray&, const char*);
  /// TODO: bool operator!=(const QByteArray&, const char*);
  // bool operator!=(const char*, const QByteArray&);
  /// TODO: bool operator!=(const char*, const QByteArray&);
  // bool operator<(const QByteArray&, const QByteArray&);
  gonk::bind::op_less<const QByteArray&, const QByteArray&>(ns);
  // bool operator<(const QByteArray&, const char*);
  /// TODO: bool operator<(const QByteArray&, const char*);
  // bool operator<(const char*, const QByteArray&);
  /// TODO: bool operator<(const char*, const QByteArray&);
  // bool operator<=(const QByteArray&, const QByteArray&);
  gonk::bind::op_leq<const QByteArray&, const QByteArray&>(ns);
  // bool operator<=(const QByteArray&, const char*);
  /// TODO: bool operator<=(const QByteArray&, const char*);
  // bool operator<=(const char*, const QByteArray&);
  /// TODO: bool operator<=(const char*, const QByteArray&);
  // bool operator>(const QByteArray&, const QByteArray&);
  gonk::bind::op_greater<const QByteArray&, const QByteArray&>(ns);
  // bool operator>(const QByteArray&, const char*);
  /// TODO: bool operator>(const QByteArray&, const char*);
  // bool operator>(const char*, const QByteArray&);
  /// TODO: bool operator>(const char*, const QByteArray&);
  // bool operator>=(const QByteArray&, const QByteArray&);
  gonk::bind::op_geq<const QByteArray&, const QByteArray&>(ns);
  // bool operator>=(const QByteArray&, const char*);
  /// TODO: bool operator>=(const QByteArray&, const char*);
  // bool operator>=(const char*, const QByteArray&);
  /// TODO: bool operator>=(const char*, const QByteArray&);
  // const QByteArray operator+(const QByteArray&, const QByteArray&);
  gonk::bind::op_add<const QByteArray, const QByteArray&, const QByteArray&>(ns);
  // const QByteArray operator+(const QByteArray&, const char*);
  /// TODO: const QByteArray operator+(const QByteArray&, const char*);
  // const QByteArray operator+(const QByteArray&, char);
  gonk::bind::op_add<const QByteArray, const QByteArray&, char>(ns);
  // const QByteArray operator+(const char*, const QByteArray&);
  /// TODO: const QByteArray operator+(const char*, const QByteArray&);
  // const QByteArray operator+(char, const QByteArray&);
  gonk::bind::op_add<const QByteArray, char, const QByteArray&>(ns);
  // QDataStream& operator<<(QDataStream&, const QByteArray&);
  /// TODO: QDataStream& operator<<(QDataStream&, const QByteArray&);
  // QDataStream& operator>>(QDataStream&, QByteArray&);
  /// TODO: QDataStream& operator>>(QDataStream&, QByteArray&);
  // QByteArray qCompress(const QByteArray&, int);
  gonk::bind::free_function<QByteArray, const QByteArray&, int, &bytearray_qCompress>(ns, "qCompress").create();
  // QByteArray qUncompress(const QByteArray&);
  gonk::bind::free_function<QByteArray, const QByteArray&, &bytearray_qUncompress>(ns, "qUncompress").create();
  // void swap(QByteArray&, QByteArray&);
  gonk::bind::void_function<QByteArray&, QByteArray&, &swap>(ns, "swap").create();
}
