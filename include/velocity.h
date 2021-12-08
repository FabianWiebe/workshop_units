#pragma once

#include "length.h"
#include "my_time.h"
#include "common_ratio.h"

namespace units {

  // put velocity related definitions here
  using dimension_velocity = dimension_divide<dimension_length, dimension_time>;

  using mps = unit<dimension_velocity, std::ratio_divide<metre::ratio, second::ratio>>;
  using kmph = unit<dimension_velocity, std::ratio_divide<kilometre::ratio, hour::ratio>>;

  namespace literals {
    constexpr auto operator""_mps(unsigned long long l) { return quantity<mps, unsigned long long>(l); }
    constexpr auto operator""_mps(long double l) { return quantity<mps, long double>(l); }
    constexpr auto operator""_kmph(unsigned long long l) { return quantity<kmph, unsigned long long>(l); }
    constexpr auto operator""_kmph(long double l) { return quantity<kmph, long double>(l); }
  }

  using namespace literals;

}  // namespace units
