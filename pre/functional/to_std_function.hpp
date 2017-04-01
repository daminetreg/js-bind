#ifndef PRE_FUNCTIONAL_TO_STD_FUNCTION_HPP
#define PRE_FUNCTIONAL_TO_STD_FUNCTION_HPP

#include <functional>
#include <pre/type_traits/function_traits.hpp>

namespace pre { namespace functional {


  /* Creates an std::function object from the given lambda, member function, free standing function
   * @t the lambda/[member]function to transform into std::function.
   * @return An [std::function](http://en.cppreference.com/w/cpp/utility/functional/function) of the given functor/function.
   */
  template <class T>
  auto to_std_function (T t) -> typename type_traits::function_traits<T>::function_type  {
    return typename type_traits::function_traits<T>::function_type(t);
  }

}}

#endif /*PRE_FUNCTIONAL_TO_STD_FUNCTION_HPP*/
