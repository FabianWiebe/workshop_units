#pragma once

#include "type_list.h"
#include <type_traits>

namespace units {

  template<int value>
  using dim_id = std::integral_constant<int, value>;

  template<typename... Exponents>
  struct dimension;

  // is_dimension

  template<typename T>
  inline constexpr bool is_dimension = false;

  template<typename... Exponents>
  inline constexpr bool is_dimension<units::dimension<Exponents...>> = true;

  // make dimension

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


  namespace detail {

    template<typename D>
    struct dim_consolidate;

    template<>
    struct dim_consolidate<dimension<>> {
      using type = dimension<>;
    };

    template<template<typename... > typename D, typename First>
    struct dim_consolidate<D<First>> {
      using type = std::conditional_t<First::value == 0, D<>, D<First>>;
    };

    template<template<typename... > typename D, typename First, typename Second, typename ...Rest>
    struct dim_consolidate<D<First, Second, Rest...>> {
      using type = std::conditional_t<
          First::value == 0,
          typename dim_consolidate<D<Second, Rest...>>::type,
          std::conditional_t<
              typename First::dimension() == typename Second::dimension(),
              typename dim_consolidate<D<exp<typename First::dimension, First::value + Second::value>, Rest...>>::type,
              type_list_push_front<typename dim_consolidate<D<Second, Rest...>>::type, First>>>;
    };

  }  // namespace detail

  template<typename... Es>
  using make_dimension = typename detail::dim_consolidate<type_list_sort<dimension<Es...>, exp_less>>::type;

}  // namespace units
