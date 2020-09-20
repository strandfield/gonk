// Copyright (C) 2020 Vincent Chambrin
// This file is part of the 'gonk' project
// For conditions of distribution and use, see copyright notice in LICENSE

#include "math.h"

#include "gonk/common/binding/function.h"

#include <script/namespace.h>

#include <cmath>

namespace gonk
{

namespace std_math
{

namespace callbacks
{

} // namespace callbacks

} // namespace std_math

} // namespace gonk

void register_math_file(script::Namespace ns)
{
  using namespace script;

  gonk::bind::free_function<float, float, std::abs>(ns, "abs").create();
  gonk::bind::free_function<double, double, std::abs>(ns, "abs").create();

  gonk::bind::free_function<float, float, float, std::fmod>(ns, "fmod").create();
  gonk::bind::free_function<double, double, double, std::fmod>(ns, "fmod").create();

  gonk::bind::free_function<float, float, float, std::remainder>(ns, "remainder").create();
  gonk::bind::free_function<double, double, double, std::remainder>(ns, "remainder").create();

  gonk::bind::free_function<float, float, float, float, std::fma>(ns, "fma").create();
  gonk::bind::free_function<double, double, double, double, std::fma>(ns, "fma").create();

  gonk::bind::free_function<float, float, float, std::fmax>(ns, "fmax").create();
  gonk::bind::free_function<double, double, double, std::fmax>(ns, "fmax").create();

  gonk::bind::free_function<float, float, float, std::fmin>(ns, "fmin").create();
  gonk::bind::free_function<double, double, double, std::fmin>(ns, "fmin").create();

  gonk::bind::free_function<float, float, std::exp>(ns, "exp").create();
  gonk::bind::free_function<double, double, std::exp>(ns, "exp").create();

  gonk::bind::free_function<float, float, std::exp2>(ns, "exp2").create();
  gonk::bind::free_function<double, double, std::exp2>(ns, "exp2").create();

  gonk::bind::free_function<float, float, std::log>(ns, "log").create();
  gonk::bind::free_function<double, double, std::log>(ns, "log").create();

  gonk::bind::free_function<float, float, std::log10>(ns, "log10").create();
  gonk::bind::free_function<double, double, std::log10>(ns, "log10").create();

  gonk::bind::free_function<float, float, std::log2>(ns, "log2").create();
  gonk::bind::free_function<double, double, std::log2>(ns, "log2").create();

  gonk::bind::free_function<float, float, float, std::pow>(ns, "pow").create();
  gonk::bind::free_function<double, double, double, std::pow>(ns, "pow").create();
  gonk::bind::free_function<double, float, int, std::pow<float, int>>(ns, "pow").create();
  gonk::bind::free_function<double, double, int, std::pow<double, int>>(ns, "pow").create();

  gonk::bind::free_function<float, float, std::sqrt>(ns, "sqrt").create();
  gonk::bind::free_function<double, double, std::sqrt>(ns, "sqrt").create();

  gonk::bind::free_function<float, float, std::cbrt>(ns, "cbrt").create();
  gonk::bind::free_function<double, double, std::cbrt>(ns, "cbrt").create();

  gonk::bind::free_function<float, float, float, std::hypot>(ns, "hypot").create();
  gonk::bind::free_function<double, double, double, std::hypot>(ns, "hypot").create();

  gonk::bind::free_function<float, float, std::sin>(ns, "sin").create();
  gonk::bind::free_function<double, double, std::sin>(ns, "sin").create();

  gonk::bind::free_function<float, float, std::cos>(ns, "cos").create();
  gonk::bind::free_function<double, double, std::cos>(ns, "cos").create();

  gonk::bind::free_function<float, float, std::tan>(ns, "tan").create();
  gonk::bind::free_function<double, double, std::tan>(ns, "tan").create();

  gonk::bind::free_function<float, float, std::asin>(ns, "asin").create();
  gonk::bind::free_function<double, double, std::asin>(ns, "asin").create();

  gonk::bind::free_function<float, float, std::acos>(ns, "acos").create();
  gonk::bind::free_function<double, double, std::acos>(ns, "acos").create();

  gonk::bind::free_function<float, float, std::atan>(ns, "atan").create();
  gonk::bind::free_function<double, double, std::atan>(ns, "atan").create();

  gonk::bind::free_function<float, float, float, std::atan2>(ns, "atan2").create();
  gonk::bind::free_function<double, double, double, std::atan2>(ns, "atan2").create();

  gonk::bind::free_function<float, float, std::sinh>(ns, "sinh").create();
  gonk::bind::free_function<double, double, std::sinh>(ns, "sinh").create();

  gonk::bind::free_function<float, float, std::cosh>(ns, "cosh").create();
  gonk::bind::free_function<double, double, std::cosh>(ns, "cosh").create();

  gonk::bind::free_function<float, float, std::tanh>(ns, "tanh").create();
  gonk::bind::free_function<double, double, std::tanh>(ns, "tanh").create();

  gonk::bind::free_function<float, float, std::asinh>(ns, "asinh").create();
  gonk::bind::free_function<double, double, std::asinh>(ns, "asinh").create();

  gonk::bind::free_function<float, float, std::acosh>(ns, "acosh").create();
  gonk::bind::free_function<double, double, std::acosh>(ns, "acosh").create();

  gonk::bind::free_function<float, float, std::atanh>(ns, "atanh").create();
  gonk::bind::free_function<double, double, std::atanh>(ns, "atanh").create();

  gonk::bind::free_function<float, float, std::erf>(ns, "erf").create();
  gonk::bind::free_function<double, double, std::erf>(ns, "erf").create();

  gonk::bind::free_function<float, float, std::erfc>(ns, "erfc").create();
  gonk::bind::free_function<double, double, std::erfc>(ns, "erfc").create();

  gonk::bind::free_function<float, float, std::tgamma>(ns, "tgamma").create();
  gonk::bind::free_function<double, double, std::tgamma>(ns, "tgamma").create();

  gonk::bind::free_function<float, float, std::lgamma>(ns, "lgamma").create();
  gonk::bind::free_function<double, double, std::lgamma>(ns, "lgamma").create();

  gonk::bind::free_function<float, float, std::ceil>(ns, "ceil").create();
  gonk::bind::free_function<double, double, std::ceil>(ns, "ceil").create();

  gonk::bind::free_function<float, float, std::floor>(ns, "floor").create();
  gonk::bind::free_function<double, double, std::floor>(ns, "floor").create();

  gonk::bind::free_function<float, float, std::trunc>(ns, "trunc").create();
  gonk::bind::free_function<double, double, std::trunc>(ns, "trunc").create();

  gonk::bind::free_function<float, float, std::round>(ns, "round").create();
  gonk::bind::free_function<double, double, std::round>(ns, "round").create();
}
