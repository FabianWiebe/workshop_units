#pragma once

#include <chrono>
#include <ratio>
#include "base_dimensions.h"
#include "quantity.h"
#include "unit.h"


namespace units {

  // put time related definitions here
  using dimension_time = make_dimension<exp<base_dim_time, 1>>;
  using milisecond = unit<dimension_time, std::chrono::milliseconds::period>;
  using second = unit<dimension_time, std::chrono::seconds::period>;
  using minute = unit<dimension_time, std::chrono::minutes::period>;
  using hour = unit<dimension_time, std::chrono::hours::period>;


  namespace literals {
    constexpr auto operator""_ms(unsigned long long l) { return quantity<milisecond, unsigned long long>(l); }
    constexpr auto operator""_ms(long double l) { return quantity<milisecond, long double>(l); }
    constexpr auto operator""_s(unsigned long long l) { return quantity<second, unsigned long long>(l); }
    constexpr auto operator""_s(long double l) { return quantity<second, long double>(l); }
    constexpr auto operator""_h(unsigned long long l) { return quantity<hour, unsigned long long>(l); }
    constexpr auto operator""_h(long double l) { return quantity<hour, long double>(l); }
  }

  using namespace literals;

}  // namespace units
