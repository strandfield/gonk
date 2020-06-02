// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "gonk/common/binding/constructor.h"
#include "gonk/common/binding/destructor.h"
#include "gonk/common/binding/function.h"
#include "gonk/common/binding/memberfunction.h"
#include "gonk/common/binding/fn-memfn.h"
#include "gonk/common/binding/chainable-memfn.h"
#include "gonk/common/binding/getter.h"
#include "gonk/common/binding/staticmemberfunction.h"
#include "gonk/common/binding/operators.h"

#include <script/engine.h>
#include <script/class.h>
#include <script/classbuilder.h>
#include <script/namespace.h>
#include <script/operator.h>

#include <script/private/value_p.h>

#include <cassert>
#include <iostream>
#include <type_traits>

void Assert(const char* expr, bool val, int line)
{
  if (val)
    return;

  std::string file = __FILE__;
  std::string message = file + ":" + std::to_string(line) + ": " + std::string(expr);
  throw std::runtime_error{ message.c_str() };
}

#define ASSERT(x) Assert(#x, (x), __LINE__)
#define ASSERT_EQ(a, b) Assert(#a "=" #b, ((a) == (b)), __LINE__)

int class_type_id_offset = -1;
int enum_type_id_offset = -1;

enum class ClassTypeIds
{
  FirstTypeId,
  Point,
  LastTypeId,
};

enum class EnumTypeIds
{
  FirstTypeId,
  LastTypeId,
};


int add(int a, int b)
{
  return a + b;
}

struct Point {
  int x_;
  int y_;

  Point() = default;
  Point(const Point &) = default;
  Point(int a, int b = 0) : x_(a), y_(b) { }

  int x() const { return x_; }
  int y() const { return y_; }

  int& rx() { return x_; }
  int& ry() { return y_; }

  int get2X() const { return 2 * x_; }

  Point& incrX(int n) { x_ += n; return *this; }

  void invert() { std::swap(x_, y_); }

  static Point max(const Point & a, const Point & b)
  {
    /* dummy impl */
    return a.x() > b.x() ? a : b;
  }

  static void print(const Point & pt) 
  { 
    std::cout << "Point(" << pt.x() << ", " << pt.y() << ")" << std::endl;
  }

  Point & operator=(const Point &) = default;
};

int point_y(const Point & pt)
{
  return pt.y();
}

namespace script {
template<> struct make_type_helper<Point> { inline static script::Type get() { return (class_type_id_offset + static_cast<int>(ClassTypeIds::Point)) | script::Type::ObjectFlag; } };
} // namespace script

void test_simple_bindind(script::Engine& e)
{
  using namespace script;

  Namespace ns = e.rootNamespace();
  
  Function add_func = bind::function<int, int, int, &add>(ns, "add").get();
  ASSERT(add_func.returnType() == Type::Int);
  ASSERT(add_func.prototype().size() == 2);
  ASSERT(add_func.parameter(0) == Type::Int);
  ASSERT(add_func.parameter(1) == Type::Int);

  Class pt = ns.newClass("Point").setId(script::make_type<Point>().data()).get();

  Function ctor = bind::default_constructor<Point>(pt).get();
  ASSERT(ctor.isConstructor());
  ASSERT(ctor.memberOf() == pt);
  ASSERT(ctor.prototype().size() == 1);

  ctor = bind::constructor<Point, int, int>(pt).get();
  ASSERT(ctor.isConstructor());
  ASSERT(ctor.memberOf() == pt);
  ASSERT(ctor.prototype().size() == 3);
  ASSERT(ctor.parameter(1) == Type::Int);
  ASSERT(ctor.parameter(2) == Type::Int);

  Function dtor = bind::destructor<Point>(pt).get();
  ASSERT(!dtor.isNull());
  ASSERT(dtor.isDestructor());
  ASSERT(dtor.memberOf() == pt);


  Function x = bind::member_function<Point, int, &Point::x>(pt, "x").get();
  ASSERT(x.isMemberFunction());
  ASSERT(x.memberOf() == pt);
  ASSERT(x.name() == "x");
  ASSERT(x.returnType() == Type::Int);
  ASSERT(x.prototype().size() == 1);
  ASSERT(x.isConst());

  Function y = bind::fn_as_memfn<Point, int, &point_y>(pt, "y").get();
  ASSERT(y.isMemberFunction());
  ASSERT(y.memberOf() == pt);
  ASSERT(y.name() == "y");
  ASSERT(y.returnType() == Type::Int);
  ASSERT(y.prototype().size() == 1);
  ASSERT(y.isConst());

  Function incr_x = bind::chainable_memfn<Point, int, &Point::incrX>(pt, "incrX").get();
  ASSERT(incr_x.isMemberFunction());
  ASSERT_EQ(incr_x.memberOf(), pt);
  ASSERT_EQ(incr_x.name(), "incrX");
  ASSERT_EQ(incr_x.returnType(), Type::ref(pt.id()));
  ASSERT_EQ(incr_x.prototype().size(), 2);
  ASSERT_EQ(incr_x.parameter(1), Type::Int);
  ASSERT(!incr_x.isConst());

  Function invert = bind::void_member_function<Point, &Point::invert>(pt, "invert").get();
  ASSERT(invert.isMemberFunction());
  ASSERT_EQ(invert.memberOf(), pt);
  ASSERT_EQ(invert.name(), "invert");
  ASSERT_EQ(invert.returnType(), Type::Void);
  ASSERT_EQ(invert.prototype().size(), 1);
  ASSERT(!invert.isConst());

  //Function rx = bind::non_const_getter<Point, int&, &Point::rx>(pt, "rx").get();
  //ASSERT(rx.isMemberFunction());
  //ASSERT_EQ(rx.memberOf(), pt);
  //ASSERT_EQ(rx.name(), "rx");
  //ASSERT_EQ(rx.returnType(), Type::Proxyint);
  //ASSERT_EQ(rx.prototype().size(), 1);
  //ASSERT(!rx.isConst());


  Function max = bind::static_member_function<Point, Point, const Point &, const Point &, &Point::max>(pt, "max").get();
  ASSERT(max.isMemberFunction());
  ASSERT(max.isStatic());
  ASSERT_EQ(max.memberOf(), pt);
  ASSERT_EQ(max.name(), "max");
  ASSERT_EQ(max.returnType(), pt.id());
  ASSERT_EQ(max.prototype().size(), 2);
  ASSERT_EQ(max.parameter(0), Type::cref(pt.id()));
  ASSERT(!max.isConst());

  Function print = bind::static_void_member_function<Point, const Point &, &Point::print>(pt, "print").get();
  ASSERT(print.isMemberFunction());
  ASSERT(print.isStatic());
  ASSERT_EQ(print.memberOf(), pt);
  ASSERT_EQ(print.name(), "print");
  ASSERT_EQ(print.returnType(), Type::Void);
  ASSERT_EQ(print.prototype().size(), 1);
  ASSERT_EQ(print.parameter(0), Type::cref(pt.id()));
  ASSERT(!print.isConst());

  Function assign = bind::memop_assign<Point, const Point &>(pt);
  ASSERT(assign.isOperator());
  ASSERT_EQ(assign.toOperator().operatorId(), AssignmentOperator);
  ASSERT_EQ(assign.returnType(), Type::ref(pt.id()));
  ASSERT_EQ(assign.parameter(0), Type::ref(pt.id()));
  ASSERT_EQ(assign.parameter(1), Type::cref(pt.id()));
  ASSERT(assign.isMemberFunction());
  ASSERT(!assign.isStatic());
  ASSERT(!assign.isConst());
  ASSERT_EQ(assign.memberOf(), pt);
}


int main(int argc, char* argv[])
{
  script::Engine engine;
  engine.setup();

  class_type_id_offset = engine.typeSystem()->reserve(script::Type::ObjectFlag, 256);
  enum_type_id_offset = engine.typeSystem()->reserve(script::Type::EnumFlag, 256);

  test_simple_bindind(engine);

  return 0;
}