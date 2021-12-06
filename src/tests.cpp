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

#include "../include/quantity.h"
#include "../include/type_list.h"

namespace {

using namespace units;

using metre = unit<std::ratio<1>>;

namespace test1 {
constexpr quantity<metre> d1(1), d2(2);
constexpr quantity d3 = d1 + d2;
static_assert(d3.count() == 3);
}

namespace test2 {
constexpr quantity<metre, int> d1(1), d2(2);
constexpr quantity<metre, int> d3 = d1 + d2;
static_assert(d3.count() == 3);

constexpr quantity<metre> d4(3.0);
constexpr quantity<metre> d5 = d4 + d1;
static_assert(d5.count() == 4.0);
}


namespace test3 {
constexpr quantity<metre, int> kilometre(1000);
constexpr quantity<metre> d(kilometre);
static_assert(d.count() == 1000);

static_assert((++quantity<metre, int>(kilometre)).count() == 1001);
static_assert((quantity<metre, int>(kilometre)++).count() == 1000);
static_assert((--quantity<metre, int>(kilometre)).count() == 999);
static_assert((quantity<metre, int>(kilometre)--).count() == 1000);

static_assert(quantity<metre, int>(kilometre) % 10 == quantity<metre, int>(0));
}

namespace test6
{
  using dm = unit<std::deci>;
  using cm = unit<std::centi>;
  static_assert(quantity_cast<quantity<cm, int>>(quantity<dm, int>(2)).count() == 20);
}

namespace test9 {
  template<typename... Types>
  struct type_list;

  using split = type_list_split<type_list<int, long, double, float, size_t>, 2>;
  static_assert(std::is_same_v<split::first_list, type_list<int, long>>);
  static_assert(std::is_same_v<split::second_list, type_list<double, float, size_t>>);
}

  // put additional unit tests (if needed) here


}  // namespace
