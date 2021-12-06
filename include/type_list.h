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

template<typename TypeList>
struct type_list_split_half;

template<template<typename...> typename List, typename... Types>
struct type_list_split_half<List<Types...>> {
  using split = detail::split_impl<List, 0, (sizeof...(Types) + 1) / 2, Types...>;
  using first_list = typename split::first_list;
  using second_list = typename split::second_list;
};

namespace detail {

  template<typename SortedList1, typename SortedList2, template<typename, typename> typename Pred>
  struct type_list_merge_sorted_impl;

  template<template<typename...> typename List, typename... Lhs, template<typename, typename> typename Pred>
  struct type_list_merge_sorted_impl<List<Lhs...>, List<>, Pred> {
    using type = List<Lhs...>;
  };

  template<template<typename...> typename List, typename... Rhs, template<typename, typename> typename Pred>
  struct type_list_merge_sorted_impl<List<>, List<Rhs...>, Pred> {
    using type = List<Rhs...>;
  };

  template<template<typename...> typename List, typename Lhs1, typename... LhsRest, typename Rhs1, typename... RhsRest,
           template<typename, typename> typename Pred>
  struct type_list_merge_sorted_impl<List<Lhs1, LhsRest...>, List<Rhs1, RhsRest...>, Pred> {
    using type = std::conditional_t<
        Pred<Lhs1, Rhs1>::value,
        type_list_push_front<typename type_list_merge_sorted_impl<List<LhsRest...>, List<Rhs1, RhsRest...>, Pred>::type, Lhs1>,
        type_list_push_front<typename type_list_merge_sorted_impl<List<Lhs1, LhsRest...>, List<RhsRest...>, Pred>::type, Rhs1>>;
  };
}

template<typename SortedList1, typename SortedList2, template<typename, typename> typename Pred>
using type_list_merge_sorted = typename detail::type_list_merge_sorted_impl<SortedList1, SortedList2, Pred>::type;

namespace detail {

  template<typename List, template<typename, typename> typename Pred>
  struct type_list_sort_impl;

  template<template<typename...> typename List, template<typename, typename> typename Pred>
  struct type_list_sort_impl<List<>, Pred> {
    using type = List<>;
  };

  template<template<typename...> typename List, typename Type, template<typename, typename> typename Pred>
  struct type_list_sort_impl<List<Type>, Pred> {
    using type = List<Type>;
  };

  template<template<typename...> typename List, typename... Types, template<typename, typename> typename Pred>
  struct type_list_sort_impl<List<Types...>, Pred> {
    using type = type_list_merge_sorted<
        typename type_list_sort_impl<typename type_list_split_half<List<Types...>>::first_list, Pred>::type,
        typename type_list_sort_impl<typename type_list_split_half<List<Types...>>::second_list, Pred>::type,
        Pred>;
  };
}

template<typename List, template<typename, typename> typename Pred>
using type_list_sort = typename detail::type_list_sort_impl<List, Pred>::type;

}  // namespace units
