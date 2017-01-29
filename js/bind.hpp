#ifndef JS_BIND_HPP
#define JS_BIND_HPP

#include <iostream>

#include <utility>
#include <functional>

#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <boost/mpl/assert.hpp>

#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <pre/type_traits/function_traits.hpp>
#include <pre/functional/to_std_function.hpp>

#ifndef JS_BIND_MAX_ARITY
  /**
   * \brief JS_BIND_MAX_ARITY Defines how many arguments the javascript
   *        exported C++  std::function can have. 
   *        It defaults to 16, simply override it if you need more.
   */
  #define JS_BIND_MAX_ARITY 16
#endif

#define JS_BIND_DETAIL_ARGS_EACH(z, n, unused)                                 \
  BOOST_PP_COMMA_IF(n) val 

#define JS_BIND_DETAIL_ARGS(COUNT)                                             \
  BOOST_PP_REPEAT(COUNT, JS_BIND_DETAIL_ARGS_EACH, unused)

#define JS_BIND_DETAIL_EACH(z, n, return_type)                                 \
  emscripten::class_<                                                          \
    std::function<                                                             \
      return_type(JS_BIND_DETAIL_ARGS(n))                                      \
    >                                                                          \
  >(BOOST_PP_STRINGIZE(return_type) BOOST_PP_STRINGIZE(n) "ArgsFunctor")       \
    .constructor<>()                                                           \
    .function("opcall", &std::function<                                        \
      return_type(JS_BIND_DETAIL_ARGS(n))                                      \
    >::operator());                                                            \




#define JS_BIND_DETAIL_GENERATE_BINDINGS()                                     \
  BOOST_PP_REPEAT(JS_BIND_MAX_ARITY, JS_BIND_DETAIL_EACH, void)                \
  BOOST_PP_REPEAT(JS_BIND_MAX_ARITY, JS_BIND_DETAIL_EACH, val)                 \


#define JS_BIND_FUNCTION_TYPES_EACH(z, n, return_type)                         \
  template<>                                                                   \
  struct functor_t<std::is_same<return_type, void>::value, n> {                \
    typedef ::std::function<                                                   \
      return_type(JS_BIND_DETAIL_ARGS(n))                                      \
    > type;                                                                    \
  };                                                                           \

#define JS_BIND_FUNCTION_TYPES()                                               \
  BOOST_PP_REPEAT(JS_BIND_MAX_ARITY, JS_BIND_FUNCTION_TYPES_EACH, void)        \
  BOOST_PP_REPEAT(JS_BIND_MAX_ARITY, JS_BIND_FUNCTION_TYPES_EACH, val)         \


#define JS_BIND_DETAIL_FWD(...) ::std::forward<decltype(__VA_ARGS__)>(__VA_ARGS__)

namespace js {
  using emscripten::val;

  template<const bool returns_void, const size_t arity>
  struct functor_t;

  JS_BIND_FUNCTION_TYPES();

  EMSCRIPTEN_BINDINGS(jsbindhpp) {
    JS_BIND_DETAIL_GENERATE_BINDINGS()
  }

  namespace detail {
    template<typename... Args> struct count_placeholders;

    template<>
    struct count_placeholders<> {
        static constexpr size_t value = 0;
    };

    template <class T>
    struct placeholder_increment {
      static constexpr size_t value = (std::is_placeholder<typename std::decay<T>::type>::value) ? 1 : 0;
    };

    template<typename Arg, typename... Args>
    struct count_placeholders<Arg, Args...> {
        static constexpr size_t value = 
         placeholder_increment<Arg>::value + count_placeholders<Args...>::value;
    }; 
  }


  template< class F, class... Args >
  emscripten::val bind( F&& f, Args&&... args ) {
    using emscripten::val;
    using pre::type_traits::function_traits;

    using result_type = typename function_traits<std::decay_t<F>>::result_type;
    auto bind_result = std::bind( std::forward<decltype(f)>(f), JS_BIND_DETAIL_FWD(args)... );

    //count_placeholders<Args...> a; a.banaa();
      using callback_t = typename functor_t<
        std::is_same< result_type, void>::value,
        detail::count_placeholders<Args...>::value
      >::type;

      callback_t functor = bind_result;

      // We ensure the correct object will always be bound to the this of the function
      auto functor_adapter = val(functor)["opcall"].call<val>("bind", val(functor)); 
      return functor_adapter;
    }

}

#endif // JS_BIND_HPP
