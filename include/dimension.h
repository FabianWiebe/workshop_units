#pragma once

#include "type_list.h"
#include <type_traits>

namespace units {

  template<int value>
  using dim_id = std::integral_constant<int, value>;

  template<typename... Exponents>
  struct dimension;

  template<typename BaseDimension, int Value>
  struct exp {
    using dimension = BaseDimension;
    static constexpr int value = Value;
  };

  template<typename D1, typename D2>
  struct exp_less : std::bool_constant<typename D1::dimension() < typename D2::dimension() || typename D1::dimension() == typename D2::dimension() && D1::value < D2::value> {};


  namespace details {
    template <typename Exp>
    struct exp_invert_impl;

    template <template<typename, int> typename ExpClass, typename BaseDimension, int Value>
    struct exp_invert_impl<ExpClass<BaseDimension, Value>>
    {
      using type = ExpClass<BaseDimension, -Value>;
    };
  }

  template <typename Exp>
  using exp_invert = typename details::exp_invert_impl<Exp>::type;

}  // namespace units
