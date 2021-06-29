// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "bytearray.h"

#include "gonk/common/binding/class.h"
#include "gonk/common/binding/enum.h"
#include "gonk/common/binding/namespace.h"
#include "gonk/common/enums.h"

#include <script/classbuilder.h>
#include <script/enumbuilder.h>

#include "qt-core/bytearray.h"
#include "qt-core/string.h"

namespace
{
script::Value bytearray_string_ctor(script::FunctionCall* c)
{
  c->thisObject().init<QByteArray>(script::get<std::string>(c->arg(1)).c_str());
  return c->thisObject();
}
}

static void register_byte_array__base64_option(script::Class& parent)
{
  using namespace script;
  
  Enum base64_option = gonk::bind::enumeration<QByteArray::Base64Option>(parent, "Base64Option").get();

  base64_option.addValue("Base64Encoding", QByteArray::Base64Option::Base64Encoding);
  base64_option.addValue("Base64UrlEncoding", QByteArray::Base64Option::Base64UrlEncoding);
  base64_option.addValue("KeepTrailingEquals", QByteArray::Base64Option::KeepTrailingEquals);
  base64_option.addValue("OmitTrailingEquals", QByteArray::Base64Option::OmitTrailingEquals);
  base64_option.addValue("IgnoreBase64DecodingErrors", QByteArray::Base64Option::IgnoreBase64DecodingErrors);
  base64_option.addValue("AbortOnBase64DecodingErrors", QByteArray::Base64Option::AbortOnBase64DecodingErrors);
}
static void register_byte_array__base64_decoding_status(script::Class& parent)
{
  using namespace script;
  
  Enum base64_decoding_status = gonk::bind::enumeration<QByteArray::Base64DecodingStatus>(parent, "Base64DecodingStatus").get();

  base64_decoding_status.addValue("Ok", static_cast<int>(QByteArray::Base64DecodingStatus::Ok));
  base64_decoding_status.addValue("IllegalInputLength", static_cast<int>(QByteArray::Base64DecodingStatus::IllegalInputLength));
  base64_decoding_status.addValue("IllegalCharacter", static_cast<int>(QByteArray::Base64DecodingStatus::IllegalCharacter));
  base64_decoding_status.addValue("IllegalPadding", static_cast<int>(QByteArray::Base64DecodingStatus::IllegalPadding));
}

static void register_byte_array__from_base64_result(script::Class& parent)
{
  using namespace script;
  
  Class from_base64_result = parent.newNestedClass("FromBase64Result").setId(script::Type::make<QByteArray::FromBase64Result>().data())
    .get();

  Class& c = from_base64_result;
  

}
namespace 
{
QByteArray QByteArray_toLower(const QByteArray& self)
{
  return self.toLower();
}
QByteArray QByteArray_toUpper(const QByteArray& self)
{
  return self.toUpper();
}
QByteArray QByteArray_trimmed(const QByteArray& self)
{
  return self.trimmed();
}
QByteArray QByteArray_simplified(const QByteArray& self)
{
  return self.simplified();
}
}

static void register_byte_array(script::Namespace& parent)
{
  using namespace script;
  
  Class byte_array = parent.newClass("QByteArray").setId(script::Type::make<QByteArray>().data())
    .get();

  Class& c = byte_array;
  
  register_byte_array__base64_option(c);
          register_byte_array__base64_decoding_status(c);
          register_byte_array__from_base64_result(c);
        
  // QByteArray();
  gonk::bind::default_constructor<QByteArray>(c).create();
  // QByteArray(const char*, int size);
  /// TODO: QByteArray(const char*, int size);
  // QByteArray(int size, char c);
  gonk::bind::constructor<QByteArray, int, char>(c).create();
  // QByteArray(int size, Qt::Initialization);
  /// TODO: QByteArray(int size, Qt::Initialization);
  // QByteArray(const QByteArray&);
  gonk::bind::constructor<QByteArray, const QByteArray&>(c).create();
  // ~QByteArray();
  gonk::bind::destructor<QByteArray>(c).create();
  // QByteArray& operator=(const QByteArray&);
  gonk::bind::memop_assign<QByteArray, const QByteArray&>(c);
  // QByteArray& operator=(const char* str);
  /// TODO: QByteArray& operator=(const char* str);
  // QByteArray(QByteArray&& other);
  gonk::bind::constructor<QByteArray, QByteArray&&>(c).create();
  // QByteArray& operator=(QByteArray&& other);
  gonk::bind::memop_assign<QByteArray, QByteArray&&>(c);
  // void swap(QByteArray& other);
  gonk::bind::void_member_function<QByteArray, QByteArray&, &QByteArray::swap>(c, "swap").create();
  // int size() const;
  gonk::bind::member_function<QByteArray, int, &QByteArray::size>(c, "size").create();
  // bool isEmpty() const;
  gonk::bind::member_function<QByteArray, bool, &QByteArray::isEmpty>(c, "isEmpty").create();
  // void resize(int size);
  gonk::bind::void_member_function<QByteArray, int, &QByteArray::resize>(c, "resize").create();
  // QByteArray& fill(char c, int size);
  gonk::bind::member_function<QByteArray, QByteArray&, char, int, &QByteArray::fill>(c, "fill").create();
  // int capacity() const;
  gonk::bind::member_function<QByteArray, int, &QByteArray::capacity>(c, "capacity").create();
  // void reserve(int size);
  gonk::bind::void_member_function<QByteArray, int, &QByteArray::reserve>(c, "reserve").create();
  // void squeeze();
  gonk::bind::void_member_function<QByteArray, &QByteArray::squeeze>(c, "squeeze").create();
  // char* data();
  /// TODO: char* data();
  // const char* data() const;
  /// TODO: const char* data() const;
  // const char* constData() const;
  /// TODO: const char* constData() const;
  // void detach();
  gonk::bind::void_member_function<QByteArray, &QByteArray::detach>(c, "detach").create();
  // bool isDetached() const;
  gonk::bind::member_function<QByteArray, bool, &QByteArray::isDetached>(c, "isDetached").create();
  // bool isSharedWith(const QByteArray& other) const;
  gonk::bind::member_function<QByteArray, bool, const QByteArray&, &QByteArray::isSharedWith>(c, "isSharedWith").create();
  // void clear();
  gonk::bind::void_member_function<QByteArray, &QByteArray::clear>(c, "clear").create();
  // char at(int i) const;
  gonk::bind::member_function<QByteArray, char, int, &QByteArray::at>(c, "at").create();
  // char operator[](int i) const;
  gonk::bind::memop_const_subscript<QByteArray, char, int>(c);
  // char operator[](uint i) const;
  /// TODO: char operator[](uint i) const;
  // QByteRef operator[](int i);
  gonk::bind::memop_subscript<QByteArray, QByteRef, int>(c);
  // QByteRef operator[](uint i);
  /// TODO: QByteRef operator[](uint i);
  // char front() const;
  gonk::bind::member_function<QByteArray, char, &QByteArray::front>(c, "front").create();
  // QByteRef front();
  gonk::bind::member_function<QByteArray, QByteRef, &QByteArray::front>(c, "front").create();
  // char back() const;
  gonk::bind::member_function<QByteArray, char, &QByteArray::back>(c, "back").create();
  // QByteRef back();
  gonk::bind::member_function<QByteArray, QByteRef, &QByteArray::back>(c, "back").create();
  // int indexOf(char c, int from) const;
  gonk::bind::member_function<QByteArray, int, char, int, &QByteArray::indexOf>(c, "indexOf").create();
  // int indexOf(const char* c, int from) const;
  /// TODO: int indexOf(const char* c, int from) const;
  // int indexOf(const QByteArray& a, int from) const;
  gonk::bind::member_function<QByteArray, int, const QByteArray&, int, &QByteArray::indexOf>(c, "indexOf").create();
  // int lastIndexOf(char c, int from) const;
  gonk::bind::member_function<QByteArray, int, char, int, &QByteArray::lastIndexOf>(c, "lastIndexOf").create();
  // int lastIndexOf(const char* c, int from) const;
  /// TODO: int lastIndexOf(const char* c, int from) const;
  // int lastIndexOf(const QByteArray& a, int from) const;
  gonk::bind::member_function<QByteArray, int, const QByteArray&, int, &QByteArray::lastIndexOf>(c, "lastIndexOf").create();
  // bool contains(char c) const;
  gonk::bind::member_function<QByteArray, bool, char, &QByteArray::contains>(c, "contains").create();
  // bool contains(const char* a) const;
  /// TODO: bool contains(const char* a) const;
  // bool contains(const QByteArray& a) const;
  gonk::bind::member_function<QByteArray, bool, const QByteArray&, &QByteArray::contains>(c, "contains").create();
  // int count(char c) const;
  gonk::bind::member_function<QByteArray, int, char, &QByteArray::count>(c, "count").create();
  // int count(const char* a) const;
  /// TODO: int count(const char* a) const;
  // int count(const QByteArray& a) const;
  gonk::bind::member_function<QByteArray, int, const QByteArray&, &QByteArray::count>(c, "count").create();
  // int compare(const char* c, Qt::CaseSensitivity cs) const;
  /// TODO: int compare(const char* c, Qt::CaseSensitivity cs) const;
  // int compare(const QByteArray& a, Qt::CaseSensitivity cs) const;
  /// TODO: int compare(const QByteArray& a, Qt::CaseSensitivity cs) const;
  // QByteArray left(int len) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, &QByteArray::left>(c, "left").create();
  // QByteArray right(int len) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, &QByteArray::right>(c, "right").create();
  // QByteArray mid(int index, int len) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, int, &QByteArray::mid>(c, "mid").create();
  // QByteArray chopped(int len) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, &QByteArray::chopped>(c, "chopped").create();
  // bool startsWith(const QByteArray& a) const;
  gonk::bind::member_function<QByteArray, bool, const QByteArray&, &QByteArray::startsWith>(c, "startsWith").create();
  // bool startsWith(char c) const;
  gonk::bind::member_function<QByteArray, bool, char, &QByteArray::startsWith>(c, "startsWith").create();
  // bool startsWith(const char* c) const;
  /// TODO: bool startsWith(const char* c) const;
  // bool endsWith(const QByteArray& a) const;
  gonk::bind::member_function<QByteArray, bool, const QByteArray&, &QByteArray::endsWith>(c, "endsWith").create();
  // bool endsWith(char c) const;
  gonk::bind::member_function<QByteArray, bool, char, &QByteArray::endsWith>(c, "endsWith").create();
  // bool endsWith(const char* c) const;
  /// TODO: bool endsWith(const char* c) const;
  // bool isUpper() const;
  gonk::bind::member_function<QByteArray, bool, &QByteArray::isUpper>(c, "isUpper").create();
  // bool isLower() const;
  gonk::bind::member_function<QByteArray, bool, &QByteArray::isLower>(c, "isLower").create();
  // void truncate(int pos);
  gonk::bind::void_member_function<QByteArray, int, &QByteArray::truncate>(c, "truncate").create();
  // void chop(int n);
  gonk::bind::void_member_function<QByteArray, int, &QByteArray::chop>(c, "chop").create();
  // QByteArray toLower() const;
  gonk::bind::fn_as_memfn<QByteArray, QByteArray, &QByteArray_toLower>(c, "toLower").create();
  // QByteArray toUpper() const;
  gonk::bind::fn_as_memfn<QByteArray, QByteArray, &QByteArray_toUpper>(c, "toUpper").create();
  // QByteArray trimmed() const;
  gonk::bind::fn_as_memfn<QByteArray, QByteArray, &QByteArray_trimmed>(c, "trimmed").create();
  // QByteArray simplified() const;
  gonk::bind::fn_as_memfn<QByteArray, QByteArray, &QByteArray_simplified>(c, "simplified").create();
  // QByteArray leftJustified(0) const;
  /// TODO: QByteArray leftJustified(0) const;
  // QByteArray rightJustified(int width, char fill, bool truncate) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, char, bool, &QByteArray::rightJustified>(c, "rightJustified").create();
  // QByteArray& prepend(char c);
  gonk::bind::member_function<QByteArray, QByteArray&, char, &QByteArray::prepend>(c, "prepend").create();
  // QByteArray& prepend(int count, char c);
  gonk::bind::member_function<QByteArray, QByteArray&, int, char, &QByteArray::prepend>(c, "prepend").create();
  // QByteArray& prepend(const char* s);
  /// TODO: QByteArray& prepend(const char* s);
  // QByteArray& prepend(const char* s, int len);
  /// TODO: QByteArray& prepend(const char* s, int len);
  // QByteArray& prepend(const QByteArray& a);
  gonk::bind::member_function<QByteArray, QByteArray&, const QByteArray&, &QByteArray::prepend>(c, "prepend").create();
  // QByteArray& append(char c);
  gonk::bind::member_function<QByteArray, QByteArray&, char, &QByteArray::append>(c, "append").create();
  // QByteArray& append(int count, char c);
  gonk::bind::member_function<QByteArray, QByteArray&, int, char, &QByteArray::append>(c, "append").create();
  // QByteArray& append(const char* s);
  /// TODO: QByteArray& append(const char* s);
  // QByteArray& append(const char* s, int len);
  /// TODO: QByteArray& append(const char* s, int len);
  // QByteArray& append(const QByteArray& a);
  gonk::bind::member_function<QByteArray, QByteArray&, const QByteArray&, &QByteArray::append>(c, "append").create();
  // QByteArray& insert(int i, char c);
  gonk::bind::member_function<QByteArray, QByteArray&, int, char, &QByteArray::insert>(c, "insert").create();
  // QByteArray& insert(int i, int count, char c);
  gonk::bind::member_function<QByteArray, QByteArray&, int, int, char, &QByteArray::insert>(c, "insert").create();
  // QByteArray& insert(int i, const char* s);
  /// TODO: QByteArray& insert(int i, const char* s);
  // QByteArray& insert(int i, const char* s, int len);
  /// TODO: QByteArray& insert(int i, const char* s, int len);
  // QByteArray& insert(int i, const QByteArray& a);
  gonk::bind::member_function<QByteArray, QByteArray&, int, const QByteArray&, &QByteArray::insert>(c, "insert").create();
  // QByteArray& remove(int index, int len);
  gonk::bind::member_function<QByteArray, QByteArray&, int, int, &QByteArray::remove>(c, "remove").create();
  // QByteArray& replace(int index, int len, const char* s);
  /// TODO: QByteArray& replace(int index, int len, const char* s);
  // QByteArray& replace(int index, int len, const char* s, int alen);
  /// TODO: QByteArray& replace(int index, int len, const char* s, int alen);
  // QByteArray& replace(int index, int len, const QByteArray& s);
  gonk::bind::member_function<QByteArray, QByteArray&, int, int, const QByteArray&, &QByteArray::replace>(c, "replace").create();
  // QByteArray& replace(char before, const char* after);
  /// TODO: QByteArray& replace(char before, const char* after);
  // QByteArray& replace(char before, const QByteArray& after);
  gonk::bind::member_function<QByteArray, QByteArray&, char, const QByteArray&, &QByteArray::replace>(c, "replace").create();
  // QByteArray& replace(const char* before, const char* after);
  /// TODO: QByteArray& replace(const char* before, const char* after);
  // QByteArray& replace(const char* before, int bsize, const char* after, int asize);
  /// TODO: QByteArray& replace(const char* before, int bsize, const char* after, int asize);
  // QByteArray& replace(const QByteArray& before, const QByteArray& after);
  gonk::bind::member_function<QByteArray, QByteArray&, const QByteArray&, const QByteArray&, &QByteArray::replace>(c, "replace").create();
  // QByteArray& replace(const QByteArray& before, const char* after);
  /// TODO: QByteArray& replace(const QByteArray& before, const char* after);
  // QByteArray& replace(const char* before, const QByteArray& after);
  /// TODO: QByteArray& replace(const char* before, const QByteArray& after);
  // QByteArray& replace(char before, char after);
  gonk::bind::member_function<QByteArray, QByteArray&, char, char, &QByteArray::replace>(c, "replace").create();
  // QByteArray& operator+=(char c);
  gonk::bind::memop_add_assign<QByteArray, char>(c);
  // QByteArray& operator+=(const char* s);
  /// TODO: QByteArray& operator+=(const char* s);
  // QByteArray& operator+=(const QByteArray& a);
  gonk::bind::memop_add_assign<QByteArray, const QByteArray&>(c);
  // QList<QByteArray> split(char sep) const;
  /// TODO: QList<QByteArray> split(char sep) const;
  // QByteArray repeated(int times) const;
  gonk::bind::member_function<QByteArray, QByteArray, int, &QByteArray::repeated>(c, "repeated").create();
  // QByteArray& append(const QString& s);
  gonk::bind::member_function<QByteArray, QByteArray&, const QString&, &QByteArray::append>(c, "append").create();
  // QByteArray& insert(int i, const QString& s);
  gonk::bind::member_function<QByteArray, QByteArray&, int, const QString&, &QByteArray::insert>(c, "insert").create();
  // QByteArray& replace(const QString& before, const char* after);
  /// TODO: QByteArray& replace(const QString& before, const char* after);
  // QByteArray& replace(char c, const QString& after);
  gonk::bind::member_function<QByteArray, QByteArray&, char, const QString&, &QByteArray::replace>(c, "replace").create();
  // QByteArray& replace(const QString& before, const QByteArray& after);
  gonk::bind::member_function<QByteArray, QByteArray&, const QString&, const QByteArray&, &QByteArray::replace>(c, "replace").create();
  // QByteArray& operator+=(const QString& s);
  gonk::bind::memop_add_assign<QByteArray, const QString&>(c);
  // int indexOf(const QString& s, int from) const;
  gonk::bind::member_function<QByteArray, int, const QString&, int, &QByteArray::indexOf>(c, "indexOf").create();
  // int lastIndexOf(const QString& s, int from) const;
  gonk::bind::member_function<QByteArray, int, const QString&, int, &QByteArray::lastIndexOf>(c, "lastIndexOf").create();
  // bool operator==(const QString& s2) const;
  gonk::bind::memop_eq<QByteArray, const QString&>(c);
  // bool operator!=(const QString& s2) const;
  gonk::bind::memop_neq<QByteArray, const QString&>(c);
  // bool operator<(const QString& s2) const;
  gonk::bind::memop_less<QByteArray, const QString&>(c);
  // bool operator>(const QString& s2) const;
  gonk::bind::memop_greater<QByteArray, const QString&>(c);
  // bool operator<=(const QString& s2) const;
  gonk::bind::memop_leq<QByteArray, const QString&>(c);
  // bool operator>=(const QString& s2) const;
  gonk::bind::memop_geq<QByteArray, const QString&>(c);
  // short toShort(bool* ok, int base) const;
  /// TODO: short toShort(bool* ok, int base) const;
  // ushort toUShort(bool* ok, int base) const;
  /// TODO: ushort toUShort(bool* ok, int base) const;
  // int toInt(bool* ok, int base) const;
  /// TODO: int toInt(bool* ok, int base) const;
  // uint toUInt(bool* ok, int base) const;
  /// TODO: uint toUInt(bool* ok, int base) const;
  // long toLong(bool* ok, int base) const;
  /// TODO: long toLong(bool* ok, int base) const;
  // ulong toULong(bool* ok, int base) const;
  /// TODO: ulong toULong(bool* ok, int base) const;
  // qlonglong toLongLong(bool* ok, int base) const;
  /// TODO: qlonglong toLongLong(bool* ok, int base) const;
  // qulonglong toULongLong(bool* ok, int base) const;
  /// TODO: qulonglong toULongLong(bool* ok, int base) const;
  // float toFloat(bool* ok) const;
  /// TODO: float toFloat(bool* ok) const;
  // double toDouble(bool* ok) const;
  /// TODO: double toDouble(bool* ok) const;
  // QByteArray toBase64(QByteArray::Base64Options options) const;
  /// TODO: QByteArray toBase64(QByteArray::Base64Options options) const;
  // QByteArray toBase64() const;
  gonk::bind::member_function<QByteArray, QByteArray, &QByteArray::toBase64>(c, "toBase64").create();
  // QByteArray toHex() const;
  gonk::bind::member_function<QByteArray, QByteArray, &QByteArray::toHex>(c, "toHex").create();
  // QByteArray toHex(char separator) const;
  gonk::bind::member_function<QByteArray, QByteArray, char, &QByteArray::toHex>(c, "toHex").create();
  // QByteArray toPercentEncoding(0) const;
  /// TODO: QByteArray toPercentEncoding(0) const;
  // QByteArray& setNum(short, int base);
  /// TODO: QByteArray& setNum(short, int base);
  // QByteArray& setNum(ushort, int base);
  /// TODO: QByteArray& setNum(ushort, int base);
  // QByteArray& setNum(int, int base);
  gonk::bind::member_function<QByteArray, QByteArray&, int, int, &QByteArray::setNum>(c, "setNum").create();
  // QByteArray& setNum(uint, int base);
  /// TODO: QByteArray& setNum(uint, int base);
  // QByteArray& setNum(qlonglong, int base);
  /// TODO: QByteArray& setNum(qlonglong, int base);
  // QByteArray& setNum(qulonglong, int base);
  /// TODO: QByteArray& setNum(qulonglong, int base);
  // QByteArray& setNum(0);
  /// TODO: QByteArray& setNum(0);
  // QByteArray& setNum(0);
  /// TODO: QByteArray& setNum(0);
  // QByteArray& setRawData(const char* a, uint n);
  /// TODO: QByteArray& setRawData(const char* a, uint n);
  // static QByteArray number(int, int base);
  gonk::bind::static_member_function<QByteArray, QByteArray, int, int, &QByteArray::number>(c, "number").create();
  // static QByteArray number(uint, int base);
  /// TODO: static QByteArray number(uint, int base);
  // static QByteArray number(qlonglong, int base);
  /// TODO: static QByteArray number(qlonglong, int base);
  // static QByteArray number(qulonglong, int base);
  /// TODO: static QByteArray number(qulonglong, int base);
  // static QByteArray number(0);
  /// TODO: static QByteArray number(0);
  // static QByteArray fromRawData(const char*, int size);
  /// TODO: static QByteArray fromRawData(const char*, int size);
  // static QByteArray::FromBase64Result fromBase64Encoding(QByteArray&& base64, QByteArray::Base64Options options);
  /// TODO: static QByteArray::FromBase64Result fromBase64Encoding(QByteArray&& base64, QByteArray::Base64Options options);
  // static QByteArray::FromBase64Result fromBase64Encoding(const QByteArray& base64, QByteArray::Base64Options options);
  /// TODO: static QByteArray::FromBase64Result fromBase64Encoding(const QByteArray& base64, QByteArray::Base64Options options);
  // static QByteArray fromBase64(const QByteArray& base64, QByteArray::Base64Options options);
  /// TODO: static QByteArray fromBase64(const QByteArray& base64, QByteArray::Base64Options options);
  // static QByteArray fromBase64(const QByteArray& base64);
  gonk::bind::static_member_function<QByteArray, QByteArray, const QByteArray&, &QByteArray::fromBase64>(c, "fromBase64").create();
  // static QByteArray fromHex(const QByteArray& hexEncoded);
  gonk::bind::static_member_function<QByteArray, QByteArray, const QByteArray&, &QByteArray::fromHex>(c, "fromHex").create();
  // static QByteArray fromPercentEncoding(0);
  /// TODO: static QByteArray fromPercentEncoding(0);
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
  // void push_back(char c);
  gonk::bind::void_member_function<QByteArray, char, &QByteArray::push_back>(c, "push_back").create();
  // void push_back(const char* c);
  /// TODO: void push_back(const char* c);
  // void push_back(const QByteArray& a);
  gonk::bind::void_member_function<QByteArray, const QByteArray&, &QByteArray::push_back>(c, "push_back").create();
  // void push_front(char c);
  gonk::bind::void_member_function<QByteArray, char, &QByteArray::push_front>(c, "push_front").create();
  // void push_front(const char* c);
  /// TODO: void push_front(const char* c);
  // void push_front(const QByteArray& a);
  gonk::bind::void_member_function<QByteArray, const QByteArray&, &QByteArray::push_front>(c, "push_front").create();
  // void shrink_to_fit();
  gonk::bind::void_member_function<QByteArray, &QByteArray::shrink_to_fit>(c, "shrink_to_fit").create();
  // static QByteArray fromStdString(const std::string& s);
  gonk::bind::static_member_function<QByteArray, QByteArray, const std::string&, &QByteArray::fromStdString>(c, "fromStdString").create();
  // std::string toStdString() const;
  gonk::bind::member_function<QByteArray, std::string, &QByteArray::toStdString>(c, "toStdString").create();
  // int count() const;
  gonk::bind::member_function<QByteArray, int, &QByteArray::count>(c, "count").create();
  // int length() const;
  gonk::bind::member_function<QByteArray, int, &QByteArray::length>(c, "length").create();
  // bool isNull() const;
  gonk::bind::member_function<QByteArray, bool, &QByteArray::isNull>(c, "isNull").create();
  // QByteArray(const std::string&);
  gonk::bind::custom_constructor<QByteArray, const std::string&>(c, &bytearray_string_ctor).create();
}

static void register_byte_ref(script::Namespace& parent)
{
  using namespace script;
  
  Class byte_ref = parent.newClass("QByteRef").setId(script::Type::make<QByteRef>().data())
    .get();

  Class& c = byte_ref;
  

  // QByteRef& operator=(char c);
  gonk::bind::memop_assign<QByteRef, char>(c);
  // QByteRef& operator=(const QByteRef& c);
  gonk::bind::memop_assign<QByteRef, const QByteRef&>(c);
  // bool operator==(char c) const;
  gonk::bind::memop_eq<QByteRef, char>(c);
  // bool operator!=(char c) const;
  gonk::bind::memop_neq<QByteRef, char>(c);
  // bool operator>(char c) const;
  gonk::bind::memop_greater<QByteRef, char>(c);
  // bool operator>=(char c) const;
  gonk::bind::memop_geq<QByteRef, char>(c);
  // bool operator<(char c) const;
  gonk::bind::memop_less<QByteRef, char>(c);
  // bool operator<=(char c) const;
  gonk::bind::memop_leq<QByteRef, char>(c);
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


  register_byte_array(ns);
  register_byte_ref(ns);

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
