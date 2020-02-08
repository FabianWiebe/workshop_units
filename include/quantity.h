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

#pragma once

#include <limits>

namespace units {

template<typename T>
inline constexpr bool treat_as_floating_point = std::is_floating_point_v<T>;

template<typename Rep>
struct quantity_values {
  static constexpr Rep zero() { return Rep(0); }
  static constexpr Rep max() { return std::numeric_limits<Rep>::max(); }
  static constexpr Rep min() { return std::numeric_limits<Rep>::lowest(); }
};

template <bool B>
using Requires = std::enable_if_t<B, bool>;

template<typename Rep = double>
class quantity {
  Rep value_;

public:
  using rep = Rep;
  constexpr quantity() : value_(0) {}
  template<class Rep2, Requires<treat_as_floating_point<Rep> || !treat_as_floating_point<Rep2>> = true>
  constexpr quantity(const quantity<Rep2>& q)  : value_(q.count()) {};
  template<class Rep2, Requires<treat_as_floating_point<Rep> || !treat_as_floating_point<Rep2>> = true>
  constexpr explicit quantity(const Rep2& v) : value_(v) {};

  template<class Rep2, Requires<treat_as_floating_point<Rep> || !treat_as_floating_point<Rep2>> = true>
  constexpr quantity& operator=(const quantity<Rep2>& other) {
    if (this != &other) {
      value_ = other.value_;
    }
    return *this;
  };

  [[nodiscard]] constexpr Rep count() const noexcept {
    return value_;
  };

  [[nodiscard]] static constexpr quantity zero() {
    return quantity(quantity_values<rep>::zero());
  };

  [[nodiscard]] static constexpr quantity min() {
    return quantity(quantity_values<rep>::min());
  };
  [[nodiscard]] static constexpr quantity max() {
    return quantity(quantity_values<rep>::max());
  };

  [[nodiscard]] constexpr quantity operator+() const {
    return *this;
  }
  [[nodiscard]] constexpr quantity operator-() const {
    return quantity(-value_);
  }

  constexpr quantity& operator+=(const quantity& q) {
    value_ += q.value_;
    return *this;
  }
  constexpr quantity& operator-=(const quantity& q) {
    value_ -= q.value_;
    return *this;
  }
  constexpr quantity& operator*=(const Rep& v) {
    value_ *= v;
    return *this;
  }
  constexpr quantity& operator/=(const Rep& v) {
    value_ /= v;
    return *this;
  }

  template <typename T2, Requires<!treat_as_floating_point<T2>> = true>
  constexpr quantity& operator%=(const T2& v) {
    value_ %= v;
    return *this;
  }

  template <typename T2, Requires<!treat_as_floating_point<T2>> = true>
  constexpr quantity& operator%=(const quantity<T2>& v) {
    value_ %= v.count();
    return *this;
  }

  constexpr quantity operator++(int) {
    auto cpy = *this;
    ++value_;
    return cpy;
  }

  constexpr quantity& operator++() {
    ++value_;
    return *this;
  }

  constexpr quantity operator--(int) {
    auto cpy = *this;
    --value_;
    return cpy;
  }

  constexpr quantity& operator--() {
    --value_;
    return *this;
  }

  template <typename T2, Requires<!treat_as_floating_point<T2>> = true>
  [[nodiscard]] friend constexpr auto operator%(const quantity<Rep>& lhs, const quantity<T2>& rhs) {
    auto result = lhs.count() % rhs.count();
    return quantity<decltype(result)>(result);
  }

  template <typename Rep1, Requires<!treat_as_floating_point<Rep1>> = true>
  [[nodiscard]] friend constexpr auto operator%(const quantity<Rep>& lhs, const Rep1& rhs) {
    auto result = lhs.count() % rhs;
    return quantity<decltype(result)>(result);
  }

  template <typename T2>
  [[nodiscard]] friend constexpr auto operator+(const quantity<Rep>& lhs, const quantity<T2>& rhs) {
    auto result = lhs.count() + rhs.count();
    return quantity<decltype(result)>(result);
  }
  template <typename T2>
  [[nodiscard]] friend constexpr auto operator-(const quantity<Rep>& lhs, const quantity<T2>& rhs) {
    auto result = lhs.count() - rhs.count();
    return quantity<decltype(result)>(result);
  }
  template <typename Rep1>
  [[nodiscard]] friend constexpr auto operator*(const quantity<Rep>& q, const Rep1& v) {
    auto result = q.count() * v;
    return quantity<decltype(result)>(result);
  }
  template <typename Rep1>
  [[nodiscard]] friend constexpr auto operator*(const Rep1& v, const quantity<Rep>& q) {
    return  q * v;
  }
  template <typename Rep1>
  [[nodiscard]] friend constexpr auto operator/(const quantity<Rep>& q, const Rep1& v) {
    auto result = q.count() / v;
    return quantity<decltype(result)>(result);
  }
  template <typename T2>
  [[nodiscard]] friend constexpr auto operator/(const quantity<Rep>& lhs, const quantity<T2>& rhs) {
    return lhs.count() / rhs.count();
  }
  template <typename T2>
  [[nodiscard]] friend constexpr bool operator==(const quantity<Rep>& lhs, const quantity<T2>& rhs) {
    return lhs.count() == rhs.count();
  }
  template <typename T2>
  [[nodiscard]] friend constexpr bool operator!=(const quantity<Rep>& lhs, const quantity<T2>& rhs) {
    return !(lhs == rhs);
  }
  template <typename T2>
  [[nodiscard]] friend constexpr bool operator<(const quantity<Rep>& lhs, const quantity<T2>& rhs) {
    return lhs.count() < rhs.count();
  }
  template <typename T2>
  [[nodiscard]] friend constexpr bool operator<=(const quantity<Rep>& lhs, const quantity<T2>& rhs) {
    return lhs.count() <= rhs.count();
  }
  template <typename T2>
  [[nodiscard]] friend constexpr bool operator>(const quantity<Rep>& lhs, const quantity<T2>& rhs) {
    return !(lhs <= rhs);
  }
  template <typename T2>
  [[nodiscard]] friend constexpr bool operator>=(const quantity<Rep>& lhs, const quantity<T2>& rhs) {
    return !(lhs < rhs);
  }
};

}  // namespace units
