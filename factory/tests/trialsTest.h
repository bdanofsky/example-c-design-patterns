/*
 * trialsTest.h
 * sandbox for trying things out
 *
 *
 *
 *
 *
 */
#pragma once

#include "gtest/gtest.h"

#include <iostream>
#include <utility>
#include <vector>
#include <cassert>

namespace {

  namespace util {
    template <typename ReturnType, typename... Args>
    struct function_traits_defs {
      static constexpr size_t arity = sizeof...(Args);

      using result_type = ReturnType;

      template <size_t i>
      struct arg {
	using type = typename std::tuple_element<i, std::tuple<Args...>>::type;
      };
    };

    template <typename T>
    struct function_traits_impl;

    template <typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(Args...)>
      : function_traits_defs<ReturnType, Args...> {};

    template <typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(*)(Args...)>
      : function_traits_defs<ReturnType, Args...> {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...)>
      : function_traits_defs<ReturnType, Args...> {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const>
      : function_traits_defs<ReturnType, Args...> {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const&>
      : function_traits_defs<ReturnType, Args...> {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const&&>
      : function_traits_defs<ReturnType, Args...> {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) volatile>
      : function_traits_defs<ReturnType, Args...> {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) volatile&>
      : function_traits_defs<ReturnType, Args...> {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) volatile&&>
      : function_traits_defs<ReturnType, Args...> {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const volatile>
      : function_traits_defs<ReturnType, Args...> {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const volatile&>
      : function_traits_defs<ReturnType, Args...> {};

    template <typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const volatile&&>
      : function_traits_defs<ReturnType, Args...> {};

    template <typename T, typename V = void>
    struct function_traits
      : function_traits_impl<T> {};

    template <typename T>
    struct function_traits<T, decltype((void)&T::operator())>
      : function_traits_impl<decltype(&T::operator())> {};

    template <size_t... Indices>
    struct indices {
      using next = indices<Indices..., sizeof...(Indices)>;
    };
    template <size_t N>
    struct build_indices {
      using type = typename build_indices<N - 1>::type::next;
    };
    template <>
    struct build_indices<0> {
      using type = indices<>;
    };
    template <size_t N>
    using BuildIndices = typename build_indices<N>::type;

    namespace details {
      template <typename FuncType,
		typename VecType,
		size_t... I,
		typename Traits = function_traits<FuncType>,
		typename ReturnT = typename Traits::result_type>
      ReturnT do_call(FuncType& func,
		      VecType& args,
		      indices<I...> ) {
	assert(args.size() >= Traits::arity);
	return func(args[I]...);
      }
    }  // namespace details

    template <typename FuncType,
	      typename VecType,
	      typename Traits = function_traits<FuncType>,
	      typename ReturnT = typename Traits::result_type>
    ReturnT unpack_caller(FuncType& func,
			  VecType& args) {
      return details::do_call(func, args, BuildIndices<Traits::arity>());
    }
  }  // namespace util

  int func3(int a, int b, int c) {
    return a + b + c;
  }

  int func4(int a, int b, int c, int d) {
    return a + b + c + d;
  }

  
  TEST(SimpleTest, trials) {
    // play around with pack/unpack of variadic args
    // the issue here is that the args are all the same type
    // stupid std::any requires cast and you have to know the type
    std::any foo = 5.0;
    double useless = std::any_cast<double>(foo);
    std::cout << foo.type().name() << ": " << useless << std::endl;
    std::vector<int> args = {1, 2, 3, 5};
    int j = util::unpack_caller(func3, args);
    std::cout << j << std::endl;
    j = util::unpack_caller(func4, args);
    std::cout << j << std::endl;

  }


}  // namespace
