// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_SEMVALUE_H
#define GONK_SEMVALUE_H

#include "gonk/gonk-defs.h"

#include <script/function.h>
#include <script/value.h>
#include <script/userdata.h>

namespace gonk
{

struct GONK_API TypeInfo : public script::UserData
{
  script::Type element_type;
  script::Engine *engine;
  script::Function eq;
  script::Function assign;
  script::Function less;
  script::Function hash;

  inline bool hasLess() const { return !less.isNull(); }
  inline bool supportsHashing() const { return !hash.isNull(); }

  static std::shared_ptr<TypeInfo> get(script::Engine *e, const script::Type & t);
  static std::shared_ptr<TypeInfo> get(const script::Class & cla);
};

class GONK_API SemValue
{
public:
  SemValue();
  SemValue(const SemValue& other);
  SemValue(SemValue&& other);
  SemValue(const std::shared_ptr<TypeInfo> & ti, const script::Value & val);
  SemValue(const std::shared_ptr<TypeInfo> & ti, script::Value && val);
  explicit SemValue(const script::Value & val);
  explicit SemValue(script::Value && val);
  ~SemValue();

  inline script::Value & get() { return value_; }
  inline const script::Value & get() const { return value_; }
  script::Value release();

  inline bool isValid() const { return typeinfo_ != nullptr && !value_.isNull(); }
  inline bool isNull() const { return typeinfo_ == nullptr || value_.isNull(); }
  inline script::Engine *engine() const { return typeinfo_->engine; }

  void assign(const script::Value & v);

  int hash() const;

  SemValue& operator=(const SemValue& other);
  SemValue& operator=(SemValue&& other);
  bool operator==(const SemValue& other) const;
  bool operator!=(const SemValue& other) const;

  bool operator<(const SemValue& other) const;

protected:
  std::shared_ptr<TypeInfo> typeinfo_;
  script::Value value_;
};

class GONK_API ObserverValue : public SemValue
{
public:
  ObserverValue(const std::shared_ptr<TypeInfo> & ti, const script::Value & val);
  ~ObserverValue();
};

} // namespace gonk

#endif // GONK_SEMVALUE_H
