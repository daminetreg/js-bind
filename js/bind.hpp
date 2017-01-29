#ifndef JS_BIND_HPP
#define JS_BIND_HPP

#include <utility>
#include <functional>

#include <emscripten/bind.h>
#include <emscripten/val.h>

#include <boost/mpl/assert.hpp>

#include <boost/preprocessor/repetition/repeat.hpp>
#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/stringize.hpp>

#include <boost/type_traits.hpp>


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
  struct functor_t {
    //typedef static_assert(false, "You have tried to use a too high arity, increase " 
    // "#define JS_BIND_MAX_ARITY") type;

    //BOOST_MPL_ASSERT_MSG(
    //      is_integral<T>::value
    //    , NON_INTEGRAL_TYPES_ARE_NOT_ALLOWED
    //    , (T)
    //    );
  };

  JS_BIND_FUNCTION_TYPES();

  EMSCRIPTEN_BINDINGS(jsbindhpp) {
    JS_BIND_DETAIL_GENERATE_BINDINGS()
  }



  template<class FunctionType, class BindExpr>
  val deduce_functor_adapter(BindExpr&& b) {
    using func_t = typename ::js::functor_t<
      std::is_same<void, typename boost::function_traits<FunctionType>::result_type>::value, 
      boost::function_traits<FunctionType>::arity
    >::type;
    func_t functor = b;

    auto functor_adapter = val(functor)["opcall"].call<val>("bind", val(functor)); 
    return functor_adapter;
  }

  /**
   * \brief 
   */
  template< class FunctionType >
  struct function {
    typedef FunctionType type;
    
    template<class BindExpr, typename std::enable_if<std::is_bind_expression<BindExpr>::value>::type* = nullptr>
    function(BindExpr&& b) : functor_adapter(deduce_functor_adapter<FunctionType>(b)) {std::cout << "CONSTRUCTING js::function" << std::endl; }

    template<class BindExpr, typename std::enable_if<std::is_bind_expression<BindExpr>::value>::type* = nullptr>
    function(const BindExpr& b) : functor_adapter(deduce_functor_adapter<FunctionType>(std::move(b))) {}

    template<class... Args, typename std::enable_if< std::is_same<void, typename boost::function_traits<FunctionType>::result_type>::value >::type* = nullptr>
    auto operator()(Args&&... args) const { return functor_adapter(JS_BIND_DETAIL_FWD(args)...); }

    operator emscripten::val&() { return functor_adapter; }
    operator emscripten::val() const { return functor_adapter; }

    private:
    emscripten::val functor_adapter;
  };

  //template< class F, class... Args >
  //emscripten::val bind( F&& f, Args&&... args ) {
  //  using emscripten::val;
  //  auto bind_result = std::bind( std::forward<decltype(f)>(f), JS_BIND_DETAIL_FWD(args)... );
  //  std::function<void(val)> functor = bind_result;

  //  std::cout << boost::function_traits<decltype(&decltype(bind_result)::operator())>::arity << std::endl;
////decltype(bind_result::result_type)
  //  
  //  //TODO: if result_type is non-void, THEN select val.
  //  //TODO: if F is member function ref, then take sizeof(args - 1) as arity.
  //  //TODO: if F is not member function ref, then take sizeof(args - 1) as arity.

  //  // We ensure the correct object will always be bound to the this of the function
  //  auto functor_adapter = val(functor)["opcall"].call<val>("bind", val(functor)); 
  //  return functor_adapter;
  //};

}

#endif // JS_BIND_HPP
