#pragma once

#include "base_dimensions.h"
#include "quantity.h"

namespace units {

  // put length definitions and its units here
  using dimension_length = make_dimension<exp<base_dim_length, 1>>;
  using metre = unit<dimension_length, std::ratio<1>>;
  using meter = metre;
  using millimetre = unit<dimension_length, std::milli>;
  using kilometre = unit<dimension_length, std::kilo>;


namespace literals {
  constexpr auto operator ""_m(unsigned long long l) {
    return quantity<metre, unsigned long long>(l);
  }
  constexpr auto operator ""_m(long double l) {
    return quantity<metre, long double>(l);
  }
  constexpr auto operator ""_mm(unsigned long long l) {
    return quantity<millimetre, unsigned long long>(l);
  }
  constexpr auto operator ""_mm(long double l) {
    return quantity<millimetre, long double>(l);
  }
  constexpr auto operator ""_km(unsigned long long l) {
    return quantity<kilometre, unsigned long long>(l);
  }
  constexpr auto operator ""_km(long double l) {
    return quantity<kilometre, long double>(l);
  }
}

}  // namespace units
