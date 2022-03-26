// Copyright (C) 2020-2022 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "gonk/common/binding/chainable-memfn.h"
#include "gonk/common/binding/constructor.h"
#include "gonk/common/binding/destructor.h"
#include "gonk/common/binding/enum.h"
#include "gonk/common/binding/fn-memfn.h"
#include "gonk/common/binding/function.h"
#include "gonk/common/binding/memberfunction.h"
#include "gonk/common/binding/staticmemberfunction.h"
#include "gonk/common/binding/operators.h"
#include "gonk/common/binding/pointer.h"

#include "gonk/templates/pointer-template.h"

#include <script/class.h>
#include <script/classbuilder.h>
#include <script/engine.h>
#include <script/enumbuilder.h>
#include <script/locals.h>
#include <script/namespace.h>
#include <script/operator.h>

#include <script/private/value_p.h>

#include <cassert>
#include <iostream>
#include <type_traits>

int guaranteed_random()
{
  return 6;
}

void assign_random(int& x)
{
  x = 6;
}

int add(int a, int b)
{
  return a + b;
}

enum CoordinateSystem
{
  Cartesian,
  Polar,
};

CoordinateSystem favoriteCoordinateSystem()
{
  return Cartesian;
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

  Point* address_if_true(bool b) { return b ? this : nullptr; }

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

TEST_CASE("Test simple binding", "[binding]")
{
  using namespace script;

  script::Engine e;
  e.setup();

  gonk::register_pointer_template(e.rootNamespace());

  Namespace ns = e.rootNamespace();

  {
    Function f = gonk::bind::function(ns, "guaranteed_random", &guaranteed_random);
    script::Value x = f.invoke({});
    REQUIRE(x.isInt());
    REQUIRE(x.toInt() == 6);
  }

  {
    Function f = gonk::bind::function(ns, "assign_random", &assign_random);
    int x = 0;
    script::Value xx = e.expose(x);
    f.invoke({ xx });
    REQUIRE(x == 6);
  }
  
  Function add_func = gonk::bind::free_function<int, int, int, &add>(ns, "add").get();
  REQUIRE(add_func.returnType() == Type::Int);
  REQUIRE(add_func.prototype().size() == 2);
  REQUIRE(add_func.parameter(0) == Type::Int);
  REQUIRE(add_func.parameter(1) == Type::Int);

  {
    script::Locals locals;
    locals.push(e.newInt(6));
    locals.push(e.newInt(7));

    script::Value val = add_func.invoke(locals.data());
    locals.push(val);

    REQUIRE(val.type() == Type::Int);
    REQUIRE(val.toInt() == 13);
  }

  Class pt = ns.newClass("Point").setId(e.registerType<Point>().data()).get();
  gonk::bind::pointer<Point>(&e);

  Function ctor = gonk::bind::default_constructor<Point>(pt).get();
  REQUIRE(ctor.isConstructor());
  REQUIRE(ctor.memberOf() == pt);
  REQUIRE(ctor.prototype().size() == 1);

  ctor = gonk::bind::constructor<Point, int, int>(pt).get();
  REQUIRE(ctor.isConstructor());
  REQUIRE(ctor.memberOf() == pt);
  REQUIRE(ctor.prototype().size() == 3);
  REQUIRE(ctor.parameter(1) == Type::Int);
  REQUIRE(ctor.parameter(2) == Type::Int);

  Function dtor = gonk::bind::destructor<Point>(pt).get();
  REQUIRE(!dtor.isNull());
  REQUIRE(dtor.isDestructor());
  REQUIRE(dtor.memberOf() == pt);


  Function x = gonk::bind::member_function<Point, int, &Point::x>(pt, "x").get();
  REQUIRE(x.isMemberFunction());
  REQUIRE(x.memberOf() == pt);
  REQUIRE(x.name() == "x");
  REQUIRE(x.returnType() == Type::Int);
  REQUIRE(x.prototype().size() == 1);
  REQUIRE(x.isConst());

  {
    Point pt{ 4, 8 };
    script::Value val = e.expose(pt);

    script::Value val_x = x.invoke({ val });

    REQUIRE(val_x.type() == Type::Int);
    REQUIRE(val_x.toInt() == 4);
  }

  Function y = gonk::bind::fn_as_memfn<Point, int, point_y>(pt, "y").get();
  REQUIRE(y.isMemberFunction());
  REQUIRE(y.memberOf() == pt);
  REQUIRE(y.name() == "y");
  REQUIRE(y.returnType() == Type::Int);
  REQUIRE(y.prototype().size() == 1);
  REQUIRE(y.isConst());

  Function address_if_true = gonk::bind::member_function<Point, Point*, bool, &Point::address_if_true>(pt, "address_if_true").get();
  REQUIRE(address_if_true.isMemberFunction());
  REQUIRE(address_if_true.memberOf() == pt);
  REQUIRE(address_if_true.name() == "address_if_true");
  REQUIRE(address_if_true.returnType() == e.makeType<Point*>());
  REQUIRE(address_if_true.prototype().size() == 2);
  REQUIRE(!address_if_true.isConst());

  {
    Point pt{ 6, 6 };
    script::Value self = e.expose(pt);

    script::Locals locals;
    locals.push(self);
    locals.push(e.newBool(false));

    script::Value result = address_if_true.invoke(locals.data());

    REQUIRE(result.type() == e.makeType<Point*>());
    REQUIRE(script::get<gonk::Pointer<Point>>(result) == nullptr);
    e.destroy(result);

    script::get<bool>(locals.at(1)) = true;

    result = address_if_true.invoke(locals.data());
    REQUIRE(script::get<gonk::Pointer<Point>>(result) == &pt);
    e.destroy(result);
  }

  {
    Function mem = gonk::bind::method(pt, "get2X", &Point::get2X);
    REQUIRE(mem.isMemberFunction());
    REQUIRE(mem.memberOf() == pt);
    REQUIRE(mem.name() == "get2X");
    REQUIRE(mem.isConst());

    Point pt{ 6, 6 };
    script::Value self = e.expose(pt);
    script::Value x = mem.invoke({ self });
    REQUIRE(x.type() == Type::Int);
    REQUIRE(x.toInt() == 12);
  }

  Function incr_x = gonk::bind::chainable_memfn<Point, int, &Point::incrX>(pt, "incrX").get();
  REQUIRE(incr_x.isMemberFunction());
  REQUIRE(incr_x.memberOf() == pt);
  REQUIRE(incr_x.name() == "incrX");
  REQUIRE(incr_x.returnType() == Type::ref(pt.id()));
  REQUIRE(incr_x.prototype().size() == 2);
  REQUIRE(incr_x.parameter(1) == Type::Int);
  REQUIRE(!incr_x.isConst());

  {
    Point pt{ 4, 8 };
    script::Value self = e.expose(pt);

    script::Locals locals;
    locals.push(self);
    locals.push(e.newInt(6));

    script::Value other_self = incr_x.invoke(locals.data());
    REQUIRE(other_self.type() == e.makeType<Point>());
    REQUIRE(std::addressof(pt) == std::addressof(script::get<Point>(other_self)));
  }

  Function invert = gonk::bind::void_member_function<Point, &Point::invert>(pt, "invert").get();
  REQUIRE(invert.isMemberFunction());
  REQUIRE(invert.memberOf() == pt);
  REQUIRE(invert.name() == "invert");
  REQUIRE(invert.returnType() == Type::Void);
  REQUIRE(invert.prototype().size() == 1);
  REQUIRE(!invert.isConst());

  Function rx = gonk::bind::member_function<Point, int&, &Point::rx>(pt, "rx").get();
  REQUIRE(rx.isMemberFunction());
  REQUIRE(rx.memberOf() == pt);
  REQUIRE(rx.name() == "rx");
  REQUIRE(rx.returnType() == Type::ref(Type::Int));
  REQUIRE(rx.prototype().size() == 1);
  REQUIRE(!rx.isConst());

  {
    Point pt{ 7, 13 };
    script::Value self = e.expose(pt);

    script::Locals locals;
    locals.push(self);
    
    script::Value x = rx.invoke(locals.data());
    REQUIRE(x.type() == Type::Int);
    REQUIRE(x.isReference());
    REQUIRE(script::get<int>(x) == 7);

    script::get<int>(x) = 11;
    REQUIRE(pt.x() == 11);
  }

  Function max = gonk::bind::static_member_function<Point, Point, const Point &, const Point &, &Point::max>(pt, "max").get();
  REQUIRE(max.isMemberFunction());
  REQUIRE(max.isStatic());
  REQUIRE(max.memberOf() == pt);
  REQUIRE(max.name() == "max");
  REQUIRE(max.returnType() == pt.id());
  REQUIRE(max.prototype().size() == 2);
  REQUIRE(max.parameter(0) == Type::cref(pt.id()));
  REQUIRE(!max.isConst());

  Function print = gonk::bind::static_void_member_function<Point, const Point &, &Point::print>(pt, "print").get();
  REQUIRE(print.isMemberFunction());
  REQUIRE(print.isStatic());
  REQUIRE(print.memberOf() == pt);
  REQUIRE(print.name() == "print");
  REQUIRE(print.returnType() == Type::Void);
  REQUIRE(print.prototype().size() == 1);
  REQUIRE(print.parameter(0) == Type::cref(pt.id()));
  REQUIRE(!print.isConst());

  Function assign = gonk::bind::memop_assign<Point, const Point &>(pt);
  REQUIRE(assign.isOperator());
  REQUIRE(assign.toOperator().operatorId() == AssignmentOperator);
  REQUIRE(assign.returnType() == Type::ref(pt.id()));
  REQUIRE(assign.parameter(0) == Type::ref(pt.id()));
  REQUIRE(assign.parameter(1) == Type::cref(pt.id()));
  REQUIRE(assign.isMemberFunction());
  REQUIRE(!assign.isStatic());
  REQUIRE(!assign.isConst());
  REQUIRE(assign.memberOf() == pt);
}

TEST_CASE("Test enum binding", "[binding]")
{
  using namespace script;

  script::Engine e;
  e.setup();

  Namespace ns = e.rootNamespace();

  Enum cs = gonk::bind::enumeration<CoordinateSystem>(ns, "CoordinateSystem").get();
  cs.addValue("Cartesian", Cartesian);
  cs.addValue("Polar", Polar);

  script::Value val = gonk::make_value<CoordinateSystem>(Polar, &e);

  REQUIRE(val.type() == e.makeType<CoordinateSystem>());
  REQUIRE(gonk::value_cast<CoordinateSystem>(val) == CoordinateSystem::Polar);

  gonk::value_cast<CoordinateSystem&>(val) = Cartesian;
  REQUIRE(gonk::value_cast<CoordinateSystem>(val) == CoordinateSystem::Cartesian);

  script::Function favcoord = gonk::bind::free_function<CoordinateSystem, &favoriteCoordinateSystem>(ns, "favoriteCoordinateSystem").get();

  val = favcoord.invoke({});
  REQUIRE(gonk::value_cast<CoordinateSystem>(val) == CoordinateSystem::Cartesian);
}
