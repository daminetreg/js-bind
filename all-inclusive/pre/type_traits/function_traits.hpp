/**
 * Copyright 2014, Joel FALCOU <joel.falcou@lri.fr>
 * Copyright 2016, Daniel Friedrich <daniel@duerrenbuehl.de> 
 * Copyright 2016, Damien BUHL <damien.buhl@lecbna.org>
 *
 */
#ifndef PRE_TYPE_TRAITS_FUNCTION_TRAITS_HPP
#define PRE_TYPE_TRAITS_FUNCTION_TRAITS_HPP

#include <pre/type_traits/detail/function_traits_impl.hpp>

namespace pre { namespace type_traits {
    
 /**
  * \brief Provides access to lambda and functions arity, return type, arguments type and access as std::function type:
  * \param F The function, function pointer or lambda type to introspect, pass it as decltype(your_lambda).
  *
  * \code{.cpp}
  * #include <iostream>
  * #include <typeinfo>
  * #include <pre/traits/function_traits.hpp>
  * #include <pre/functional/to_std_function.hpp>
  *
  * int main() {
  *
  *     auto my_lambda = [](bool arg_0, int arg_1, double arg_2, std::string arg_3) {
  *                             return int{0} ;
  *                         };
  *
  *     typedef pre::type_traits::function_traits<decltype(my_lambda)> my_lambda_types;
  *
  *     std::cout << "my_lambda number of arguments " << my_lambda_types::arity                        << std::endl;
  *     std::cout << "my_lambda return type is "      << typeid(my_lambda_types::result_type).name()   << std::endl;
  *     std::cout << "my_lambda argument 0 type is "  << typeid(my_lambda_types::arg<0>).name()        << std::endl;
  *     std::cout << "my_lambda argument 1 type is "  << typeid(my_lambda_types::arg<1>).name()        << std::endl;
  *     std::cout << "my_lambda argument 2 type is "  << typeid(my_lambda_types::arg<2>).name()        << std::endl;
  *     std::cout << "my_lambda argument 3 type is "  << typeid(my_lambda_types::arg<3>).name()        << std::endl;
  *     std::cout << "my_lambda function type is "    << typeid(my_lambda_types::function_type).name() << std::endl;
  *
  *     auto std_function = pre::functional::to_std_function(my_lambda);
  *
  *     std::cout << "my_lambda called as std::function "  << std_function(true, int{42}, double{3.14}, std::string{"Hello World"}) << std::endl;
  *
  *     return 0;
  * }
  * \endcode
  * 
  */
  template<typename F, typename = void>
  struct function_traits;

  template<typename F> 
  struct function_traits<F, detail::enable_if_is_lambda_t<F> > 
    : public detail::function_traits_impl<decltype(&F::operator())> {};

  template<typename F>
  struct function_traits<F, detail::enable_if_is_function_t<F> >
    : public detail::function_traits_impl<typename std::remove_pointer<F>::type> {};

  template<typename F>
  struct function_traits<F, detail::enable_if_is_function_member_pointer_t<F> >
    : public detail::function_traits_impl_member<F> {};
  

}}


#endif /*PRE_TYPE_TRAITS_FUNCTION_TRAITS_HPP*/
