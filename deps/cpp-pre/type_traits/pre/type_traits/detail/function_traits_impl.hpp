/**
 * Copyright 2014, Joel FALCOU <joel.falcou@lri.fr>
 * Copyright 2016, Daniel Friedrich <daniel@duerrenbuehl.de> 
 * Copyright 2016, Damien BUHL <damien.buhl@lecbna.org>
 *
 */
#ifndef PRE_TYPE_TRAITS_DETAIL_FUNCTION_TRAITS_IMPL_HPP
#define PRE_TYPE_TRAITS_DETAIL_FUNCTION_TRAITS_IMPL_HPP

#include <type_traits>
#include <functional>
#include <tuple>

namespace pre { namespace type_traits { namespace detail {

    template<typename F>
    using enable_if_is_lambda_t = 
      typename std::enable_if< 
        ! std::is_function< typename std::remove_pointer<F>::type >::value
        && ! std::is_member_function_pointer< F >::value
      >::type;

    template<typename F>
    using enable_if_is_function_t = 
      typename std::enable_if< 
        std::is_function< typename std::remove_pointer<F>::type >::value
        && ! std::is_member_function_pointer< F >::value
      >::type;

    template<typename F>
    using enable_if_is_function_member_pointer_t = 
      typename std::enable_if< 
        std::is_member_function_pointer< F >::value
      >::type;


    template<typename ReturnType, typename... Args>
    struct function_traits_api {
      enum {
          arity = sizeof...(Args)
      };

      using result_type = ReturnType;

      using function_type = std::function<ReturnType(Args...)>;

    private:
      template<size_t i>
      struct arg_ {
          typedef typename std::tuple_element<i, std::tuple<Args...>>::type type;
      };

    public:
      template<size_t i>
      using arg = typename arg_<i>::type;
    };

    template<typename... FunctionSignature>
    struct function_traits_impl;
   
    template<typename... FunctionSignature>
    struct function_traits_impl_member;

    template<typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(Args...)>
      : public function_traits_api<ReturnType, Args...>
    {};

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl<ReturnType(ClassType::*)(Args...) const> 
      : public function_traits_api<ReturnType, Args...>
    {};

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl_member<ReturnType(ClassType::*)(Args...)> 
      : public function_traits_api<ReturnType, ClassType, Args...>
    {};

    template<typename ClassType, typename ReturnType, typename... Args>
    struct function_traits_impl_member<ReturnType(ClassType::*)(Args...) const> 
      : public function_traits_api<ReturnType, ClassType, Args...>
    {};

}}}

#endif
