#pragma once

#include <cstring>
namespace units {

// put type_list and all related tools here
namespace detail {
  template<typename List, typename... Types>
  struct type_list_push_front_impl;

  template<template<typename...> typename List, typename... OldTypes, typename... NewTypes>
  struct type_list_push_front_impl<List<OldTypes...>, NewTypes...> {
    using type = List<NewTypes..., OldTypes...>;
  };
}

template<typename List, typename... Types>
using type_list_push_front = typename detail::type_list_push_front_impl<List, Types...>::type;

// put type_list and all related tools here
namespace detail {
  template<typename List, typename... Types>
  struct type_list_push_back;

  template<template<typename...> typename List, typename... OldTypes, typename... NewTypes>
  struct type_list_push_back<List<OldTypes...>, NewTypes...> {
    using type = List<OldTypes..., NewTypes...>;
  };
}

template<typename List, typename... Types>
using type_list_push_back = typename detail::type_list_push_back<List, Types...>::type;


namespace detail {
  template<template<typename...> typename List, std::size_t Idx, std::size_t N, typename... Types>
  struct split_impl;

  template<template<typename...> typename List, std::size_t Idx, std::size_t N>
  struct split_impl<List, Idx, N> {
    using first_list = List<>;
    using second_list = List<>;
  };

  template<template<typename...> typename List, std::size_t Idx, std::size_t N, typename T, typename... Rest>
  struct split_impl<List, Idx, N, T, Rest...> : split_impl<List, Idx + 1, N, Rest...> {
    using Base = split_impl<List, Idx + 1, N, Rest...>;
    using first_list = std::conditional_t<Idx < N,
                                                type_list_push_front<typename Base::first_list, T>,
                                                typename Base::first_list>;
    using second_list = std::conditional_t<Idx < N,
                                                 typename Base::second_list,
                                                 type_list_push_front<typename Base::second_list, T>>;
  };
}

template<typename List, std::size_t N>
struct type_list_split;

template<template<typename...> typename List, std::size_t N, typename... Types>
struct type_list_split<List<Types...>, N> {
  static_assert(N <= sizeof...(Types), "Invalid index provided");
  using split = detail::split_impl<List, 0, N, Types...>;
  using first_list = typename split::first_list;
  using second_list = typename split::second_list;
};


}  // namespace units
