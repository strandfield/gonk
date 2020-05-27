// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#ifndef GONK_WRAPPERS_REF_MEM_FUN_WRAPPER_H
#define GONK_WRAPPERS_REF_MEM_FUN_WRAPPER_H

#include "gonk/common/values.h"

#include <script/interpreter/executioncontext.h>

/*
 * This header defines class templates that can be used to 
 * wrap C++ member functions returning references (mostly 
 * non-const references).
 */

namespace script
{

namespace wrapper
{

template<typename MemberType, MemberType f>
struct ref_member_wrapper_t;

//template<typename R, typename ClassType, R(ClassType::*f)()const>
//struct ref_member_wrapper_t<R(ClassType::*)()const, f> {
//  static script::Value wrap(script::FunctionCall *c) {
//    ClassType & self = *value_cast<ClassType*>(c->arg(0));
//    return c->engine()->newPtr(make_type<Proxy<typename std::decay<R>::type>>(), &((self).*(f))());
//  }
//};
//
//template<typename R, typename ClassType, R(ClassType::*f)()>
//struct ref_member_wrapper_t<R(ClassType::*)(), f> {
//  static script::Value wrap(script::FunctionCall *c) {
//    ClassType & self = *value_cast<ClassType*>(c->arg(0));
//    return c->engine()->newPtr(make_type<Proxy<typename std::decay<R>::type>>(), &((self).*(f))());
//  }
//};
//
//template<typename R, typename ClassType, typename A1, R(ClassType::*f)(A1)const>
//struct ref_member_wrapper_t<R(ClassType::*)(A1)const, f> {
//  static script::Value wrap(script::FunctionCall *c) {
//    ClassType & self = *value_cast<ClassType*>(c->arg(0));
//    return c->engine()->newPtr(make_type<Proxy<typename std::decay<R>::type>>(), &((self).*(f))(value_cast<A1>(c->arg(1))));
//  }
//};
//
//template<typename R, typename ClassType, typename A1, R(ClassType::*f)(A1)>
//struct ref_member_wrapper_t<R(ClassType::*)(A1), f> {
//  static script::Value wrap(script::FunctionCall *c) {
//    ClassType & self = *value_cast<ClassType*>(c->arg(0));
//    return c->engine()->newPtr(make_type<Proxy<typename std::decay<R>::type>>(), &((self).*(f))(value_cast<A1>(c->arg(1))));
//  }
//};
//
//template<typename R, typename ClassType, typename A1, typename A2, R(ClassType::*f)(A1, A2)const>
//struct ref_member_wrapper_t<R(ClassType::*)(A1, A2)const, f> {
//  static script::Value wrap(script::FunctionCall *c) {
//    ClassType & self = *value_cast<ClassType*>(c->arg(0));
//    return c->engine()->newPtr(make_type<Proxy<typename std::decay<R>::type>>(), &((self).*(f))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2))));
//  }
//};
//
//template<typename R, typename ClassType, typename A1, typename A2, R(ClassType::*f)(A1, A2)>
//struct ref_member_wrapper_t<R(ClassType::*)(A1, A2), f> {
//  static script::Value wrap(script::FunctionCall *c) {
//    ClassType & self = *value_cast<ClassType*>(c->arg(0));
//    return c->engine()->newPtr(make_type<Proxy<typename std::decay<R>::type>>(), &((self).*(f))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2))));
//  }
//};
//
//template<typename R, typename ClassType, typename A1, typename A2, typename A3, R(ClassType::*f)(A1, A2, A3)const>
//struct ref_member_wrapper_t<R(ClassType::*)(A1, A2, A3)const, f> {
//  static script::Value wrap(script::FunctionCall *c) {
//    ClassType & self = *value_cast<ClassType*>(c->arg(0));
//    return c->engine()->newPtr(make_type<Proxy<typename std::decay<R>::type>>(), &((self).*(f))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3))));
//  }
//};
//
//template<typename R, typename ClassType, typename A1, typename A2, typename A3, R(ClassType::*f)(A1, A2, A3)>
//struct ref_member_wrapper_t<R(ClassType::*)(A1, A2, A3), f> {
//  static script::Value wrap(script::FunctionCall *c) {
//    ClassType & self = *value_cast<ClassType*>(c->arg(0));
//    return c->engine()->newPtr(make_type<Proxy<typename std::decay<R>::type>>(), &((self).*(f))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3))));
//  }
//};
//
//template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, R(ClassType::*f)(A1, A2, A3, A4)const>
//struct ref_member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4)const, f> {
//  static script::Value wrap(script::FunctionCall *c) {
//    ClassType & self = *value_cast<ClassType*>(c->arg(0));
//    return c->engine()->newPtr(make_type<Proxy<typename std::decay<R>::type>>(), &((self).*(f))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4))));
//  }
//};
//
//template<typename R, typename ClassType, typename A1, typename A2, typename A3, typename A4, R(ClassType::*f)(A1, A2, A3, A4)>
//struct ref_member_wrapper_t<R(ClassType::*)(A1, A2, A3, A4), f> {
//  static script::Value wrap(script::FunctionCall *c) {
//    ClassType & self = *value_cast<ClassType*>(c->arg(0));
//    return c->engine()->newPtr(make_type<Proxy<typename std::decay<R>::type>>(), &((self).*(f))(value_cast<A1>(c->arg(1)), value_cast<A2>(c->arg(2)), value_cast<A3>(c->arg(3)), value_cast<A4>(c->arg(4))));
//  }
//};

} // namespace wrapper

} // namespace script

#endif // GONK_WRAPPERS_REF_MEM_FUN_WRAPPER_H
