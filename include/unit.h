#pragma once

#include <type_traits>
#include <ratio>


namespace units {

template<typename Ratio>
struct unit {
  using ratio = Ratio;
  static_assert(std::is_same_v<Ratio, std::ratio<Ratio::num, Ratio::den>>);

  static_assert(Ratio::num >= 0);
};

}  // namespace units
