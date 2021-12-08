#pragma once

#include <ratio>
#include "base_dimensions.h"
#include "my_time.h"
#include "unit.h"
#include "quantity.h"

namespace units {

  // put frequency related definitions here
  using dimension_frequency = make_dimension<exp<base_dim_time, -1>>;

  using hertz = unit<dimension_frequency, std::ratio<1>>;
  using kilohertz = unit<dimension_frequency, std::kilo>;

  namespace literals {
    constexpr auto operator""_Hz(unsigned long long l) { return quantity<hertz, unsigned long long>(l); }
    constexpr auto operator""_Hz(long double l) { return quantity<hertz, long double>(l); }
    constexpr auto operator""_kHz(unsigned long long l) { return quantity<kilohertz, unsigned long long>(l); }
    constexpr auto operator""_kHz(long double l) { return quantity<kilohertz, long double>(l); }
  }

  using namespace literals;

}  // namespace units
