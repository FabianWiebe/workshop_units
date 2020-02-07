// The MIT License (MIT)
//
// Copyright (c) 2018 Train IT
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "quantity.h"
#include <limits>
#include <utility>

namespace {

  template<typename T>
  class my_value {
    T value_{};
  public:
    my_value() = default;
    constexpr my_value(T v) : value_(std::move(v)) {}
    constexpr my_value& operator+=(const my_value& other) { value_ += other.value_; return *this; }
    constexpr my_value& operator-=(const my_value& other) { value_ -= other.value_; return *this; }
    constexpr my_value& operator*=(const my_value& other) { value_ *= other.value_; return *this; }
    constexpr my_value& operator/=(const my_value& other) { value_ /= other.value_; return *this; }
    constexpr operator const T&() const & { return value_; }
  };

} // namespace

namespace units {

  template<typename T>
  inline constexpr bool treat_as_floating_point<my_value<T>> = std::is_floating_point_v<T>;

  template<typename T>
  struct quantity_values<my_value<T>> {
    static constexpr my_value<T> zero() { return my_value<T>(0); }
    static constexpr my_value<T> max() { return std::numeric_limits<T>::max(); }
    static constexpr my_value<T> min() { return std::numeric_limits<T>::lowest(); }
  };

}  // namespace units

namespace {

  using namespace units;

  // class invariants

//  constexpr quantity<quantity<int>> error(quantity<int>(0));  // should trigger a static_assert

  // member types

  static_assert(std::is_same_v<quantity<int>::rep, int>);
  static_assert(std::is_same_v<quantity<float>::rep, float>);
 
   // constructors

  static_assert(quantity<>().count() == 0);
  static_assert(quantity<int>(1).count() == 1);
  static_assert(quantity<int>(1000).count() == 1000);
  static_assert(quantity<>(quantity<int>(1000)).count() == quantity<int>(1000).count());

  static_assert(quantity<int>(1).count() == 1);
  static_assert(quantity<int>(my_value<int>(1)).count() == 1);
  static_assert(quantity<my_value<int>>(1).count() == 1);
//  static_assert(quantity<int>(1.0).count() == 1);   // should not compile
//  static_assert(quantity<int>(my_value<float>(1.0)).count() == 1); // should not compile
//  static_assert(quantity<my_value<int>>(1.0).count() == 1);   // should not compile
  static_assert(quantity<float>(1.0).count() == 1.0);
  static_assert(quantity<float>(my_value<float>(1.0)).count() == 1.0);
  static_assert(quantity<float>(1).count() == 1.0);
  static_assert(quantity<float>(my_value<int>(1)).count() == 1.0);
  static_assert(quantity<float>(3.14f).count() == 3.14f);
  static_assert(quantity<my_value<float>>(1.0).count() == 1.0);
  static_assert(quantity<my_value<float>>(1).count() == 1.0);
  static_assert(quantity<my_value<float>>(3.14f).count() == 3.14f);

  static_assert(quantity<int>(quantity<int>(1000)).count() == 1000);
//  static_assert(quantity<int>(quantity<float>(1000.0)).count() == 1000);   // should not compile
//  static_assert(quantity<int>(quantity<my_value<float>>(1000.0)).count() == 1000);   // should not compile
//  static_assert(quantity<my_value<int>>(quantity<float>(1000.0)).count() == 1000);   // should not compile
  static_assert(quantity<float>(quantity<float>(1000.0)).count() == 1000.0);
  static_assert(quantity<float>(quantity<my_value<float>>(1000.0)).count() == 1000.0);
  static_assert(quantity<my_value<float>>(quantity<float>(1000.0)).count() == 1000.0);
  static_assert(quantity<float>(quantity<int>(1000)).count() == 1000.0);
  static_assert(quantity<my_value<float>>(quantity<int>(1000)).count() == 1000.0);

  // assignment operator

  auto test_assign = [](auto l1, auto l2) {
    return l1 = l2;
  };
  static_assert(test_assign(quantity<>(2), quantity<>(1)).count() == 1);

  // static member functions

  static_assert(quantity<>::zero().count() == 0);
  static_assert(quantity<>::min().count() == std::numeric_limits<double>::lowest());
  static_assert(quantity<>::max().count() == std::numeric_limits<double>::max());
  static_assert(quantity<int>::zero().count() == 0);
  static_assert(quantity<int>::min().count() == std::numeric_limits<int>::lowest());
  static_assert(quantity<int>::max().count() == std::numeric_limits<int>::max());
  static_assert(quantity<my_value<int>>::zero().count() == 0);
  static_assert(quantity<my_value<int>>::min().count() == std::numeric_limits<int>::lowest());
  static_assert(quantity<my_value<int>>::max().count() == std::numeric_limits<int>::max());
  static_assert(quantity<my_value<float>>::zero().count() == 0.0);
  static_assert(quantity<my_value<float>>::min().count() == std::numeric_limits<float>::lowest());
  static_assert(quantity<my_value<float>>::max().count() == std::numeric_limits<float>::max());

  // unary member operators

  static_assert((+quantity<int>(1000)).count() == 1000);
  static_assert((-quantity<int>(1000)).count() == -1000);
  static_assert((+(-quantity<int>(1000))).count() == -1000);
  static_assert((-(-quantity<int>(1000))).count() == 1000);

  // binary member operators

  auto test_incr = [](auto v, auto func) {
    auto vv = func(v);
    return std::make_pair(v, vv);
  };
  static_assert(test_incr(quantity<int>(1000), [](auto& v) { return v++; }) == std::make_pair(quantity<int>(1001), quantity<int>(1000)));
  static_assert(test_incr(quantity<int>(1000), [](auto& v) { return ++v; }) == std::make_pair(quantity<int>(1001), quantity<int>(1001)));
  static_assert(test_incr(quantity<int>(1000), [](auto& v) { return v--; }) == std::make_pair(quantity<int>(999), quantity<int>(1000)));
  static_assert(test_incr(quantity<int>(1000), [](auto& v) { return --v; }) == std::make_pair(quantity<int>(999), quantity<int>(999)));

  // compound assignment

  static_assert((quantity<>(1) += quantity<>(1)).count() == 2);
  static_assert((quantity<>(2) -= quantity<>(1)).count() == 1);
  static_assert((quantity<>(1) *= 2).count() == 2);
  static_assert((quantity<>(2) /= 2).count() == 1);
  static_assert((quantity<int>(7) %= 2).count() == 1);
  static_assert((quantity<int>(7) %= quantity<int>(2)).count() == 1);
//  static_assert((quantity<>(7.) %= 2).count() == 1);                  // should not compile
//  static_assert((quantity<int>(7) %= 2.).count() == 1);               // should not compile
//  static_assert((quantity<>(7.) %= quantity<int>(2)).count() == 1);   // should not compile
//  static_assert((quantity<int>(7) %= quantity<>(2.)).count() == 1);   // should not compile

  // non-member arithmetic operators

  static_assert(std::is_same_v<decltype(quantity<int>() + quantity<float>()), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<float>() + quantity<int>()), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<float>() - quantity<int>()), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<int>() - quantity<float>()), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<int>() * 1.0f), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<float>() * 1), quantity<float>>);
  static_assert(std::is_same_v<decltype(1.0f * quantity<int>()), quantity<float>>);
  static_assert(std::is_same_v<decltype(1 * quantity<float>()), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<int>() / 1.0f), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<float>() / 1), quantity<float>>);
  static_assert(std::is_same_v<decltype(quantity<int>() / quantity<float>()), float>);
  static_assert(std::is_same_v<decltype(quantity<float>() / quantity<int>()), float>);
  static_assert(std::is_same_v<decltype(quantity<int>() % short(1)), quantity<int>>);
  static_assert(std::is_same_v<decltype(quantity<int>() % quantity<short>(1)), quantity<int>>);
  static_assert((quantity<>(1) + quantity<int>(1000)).count() == 1001);
  static_assert((quantity<int>(1000) - quantity<>(1)).count() == 999);
  static_assert((quantity<>(2) * 2).count() == 4);
  static_assert((3 * quantity<>(3)).count() == 9);
  static_assert((quantity<>(4) / 2).count() == 2);
  static_assert(quantity<>(4) / quantity<>(2) == 2);
  static_assert((quantity<int>(7) % 2).count() == 1);
  static_assert((quantity<int>(7) % quantity<int>(2)).count() == 1);
//  static_assert((quantity<>(7.) % 2).count() == 1);                 // should not compile
//  static_assert((quantity<int>(7) % 2.).count() == 1);              // should not compile
//  static_assert((quantity<>(7.) % quantity<int>(2)).count() == 1);  // should not compile
//  static_assert((quantity<int>(7) % quantity<>(2.)).count() == 1);  // should not compile

  // comparators

  static_assert(quantity<>(2) + quantity<>(1) == quantity<>(3));
  static_assert(!(quantity<>(2) + quantity<>(2) == quantity<>(3)));
  static_assert(quantity<>(2) + quantity<>(2) != quantity<>(3));
  static_assert(!(quantity<>(2) + quantity<>(2) != quantity<>(4)));
  static_assert(quantity<>(2) > quantity<>(1));
  static_assert(!(quantity<>(1) > quantity<>(1)));
  static_assert(quantity<>(1) < quantity<>(2));
  static_assert(!(quantity<>(2) < quantity<>(2)));
  static_assert(quantity<>(2) >= quantity<>(1));
  static_assert(quantity<>(2) >= quantity<>(2));
  static_assert(!(quantity<>(2) >= quantity<>(3)));
  static_assert(quantity<>(1) <= quantity<>(2));
  static_assert(quantity<>(2) <= quantity<>(2));
  static_assert(!(quantity<>(3) <= quantity<>(2)));

  static_assert(quantity<int>(3) == quantity<float>(3.0));
  static_assert(quantity<int>(3) != quantity<float>(3.14));
  static_assert(quantity<int>(2) > quantity<float>(1.0));
  static_assert(quantity<float>(1.0) < quantity<int>(2));
  static_assert(quantity<float>(2.0) >= quantity<int>(1));
  static_assert(quantity<int>(1) <= quantity<float>(2.0));

}  // namespace
