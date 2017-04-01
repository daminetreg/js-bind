#define BOOST_TEST_MODULE function_traits_test
#include <boost/test/included/unit_test.hpp>


#include <iostream>
#include <type_traits>
#include <pre/type_traits/function_traits.hpp>
#include <pre/functional/to_std_function.hpp>


BOOST_AUTO_TEST_CASE (function_traits_test_lambda) {

    auto my_lambda = [](bool arg_0, int arg_1, double arg_2, std::string arg_3) {
                            return int{23} ;
                        };

    typedef pre::type_traits::function_traits<decltype(my_lambda)> my_lambda_types;

    auto is_same =  std::is_same< my_lambda_types::result_type, int>::value;

    BOOST_ASSERT_MSG(my_lambda_types::arity == 4 , " lambda arity is wrong");
    BOOST_ASSERT_MSG(is_same, " return type wrong");

    is_same =  std::is_same<my_lambda_types::arg<0>, bool>::value;
    BOOST_ASSERT_MSG(is_same, " argument ist not as expected");

    is_same =  std::is_same<my_lambda_types::arg<1>, int>::value;
    BOOST_ASSERT_MSG(is_same, " argument ist not as expected");

    is_same =  std::is_same<my_lambda_types::arg<2>, double>::value;
    BOOST_ASSERT_MSG(is_same, " argument ist not as expected");

    is_same =  std::is_same<my_lambda_types::arg<3>, std::string>::value;
    BOOST_ASSERT_MSG(is_same, " argument ist not as expected");

    typedef std::function<int(bool, int, double, std::string)> expected_function_type;
    is_same = std::is_same<my_lambda_types::function_type , expected_function_type>::value;
    BOOST_ASSERT_MSG(is_same, " function type  ist not as expected");

    auto std_function = pre::functional::to_std_function(my_lambda);

    BOOST_ASSERT_MSG(std_function(true, int{42}, double{3.14}, std::string{"Hello World"}) == 23 , " call to std::funtion is wrong");
}


BOOST_AUTO_TEST_CASE (function_traits_test_lambda_copy_captures) {

    bool capture_bool     = true;
    int capture_int       = 42;
    double capture_double = 12.3;

    auto my_lambda_copy_capture = [=](bool arg_0, int arg_1, double arg_2, std::string arg_3) {
      return capture_int + capture_double;
    };

    typedef pre::type_traits::function_traits<decltype(my_lambda_copy_capture)> my_lambda_types;

    auto is_same =  std::is_same< my_lambda_types::result_type, double>::value;

    BOOST_ASSERT_MSG(my_lambda_types::arity == 4 , " lambda arity is wrong");
    BOOST_ASSERT_MSG(is_same, " return type wrong");

    is_same =  std::is_same<my_lambda_types::arg<0>, bool>::value;
    BOOST_ASSERT_MSG(is_same, " argument 0 ist not as expected " );

    is_same =  std::is_same<my_lambda_types::arg<1>, int>::value;
    BOOST_ASSERT_MSG(is_same, " argument 1 ist not as expected");

    is_same =  std::is_same<my_lambda_types::arg<2>, double>::value;
    BOOST_ASSERT_MSG(is_same, " argument 2 ist not as expected");

    is_same =  std::is_same<my_lambda_types::arg<3>, std::string>::value;
    BOOST_ASSERT_MSG(is_same, " argument 3 ist not as expected");

    typedef std::function<int(bool, int, double, std::string)> expected_function_type;
    is_same = std::is_same<my_lambda_types::function_type , expected_function_type>::value;
    BOOST_ASSERT_MSG(!is_same, " function type  ist not as expected with capture it is different");
    auto std_function = pre::functional::to_std_function(my_lambda_copy_capture);

    BOOST_ASSERT_MSG(std_function(true, int{42}, double{3.14}, std::string{"Hello World"}) == (42+12.3) , " call to std::funtion is wrong");
}

BOOST_AUTO_TEST_CASE (function_traits_test_lambda_ref_captures) {

    bool capture_bool     = true;
    int capture_int       = 42;
    double capture_double = 12.3;

    auto my_lambda_ref_capture = [&](bool arg_0, int arg_1, double arg_2, std::string arg_3) {
      return capture_int + capture_double;
    };

    typedef pre::type_traits::function_traits<decltype(my_lambda_ref_capture)> my_lambda_types;

    auto is_same =  std::is_same< my_lambda_types::result_type, double>::value;

    BOOST_ASSERT_MSG(my_lambda_types::arity == 4 , " lambda arity is wrong");
    BOOST_ASSERT_MSG(is_same, " return type wrong");

    is_same =  std::is_same<my_lambda_types::arg<0>, bool>::value;
    BOOST_ASSERT_MSG(is_same, " argument 0 ist not as expected");

    is_same =  std::is_same<my_lambda_types::arg<1>, int>::value;
    BOOST_ASSERT_MSG(is_same, " argument 1 ist not as expected");

    is_same =  std::is_same<my_lambda_types::arg<2>, double>::value;
    BOOST_ASSERT_MSG(is_same, " argument 2 ist not as expected");

    is_same =  std::is_same<my_lambda_types::arg<3>, std::string>::value;
    BOOST_ASSERT_MSG(is_same, " argument 3 ist not as expected");

    typedef std::function<int(bool, int, double, std::string)> expected_function_type;
    is_same = std::is_same<my_lambda_types::function_type , expected_function_type>::value;
    BOOST_ASSERT_MSG(!is_same, " function type  ist not as expected with capture it is different");

    auto std_function = pre::functional::to_std_function(my_lambda_ref_capture);

    BOOST_ASSERT_MSG(std_function(true, int{42}, double{3.14}, std::string{"Hello World"}) == (42+12.3) , " call to std::funtion is wrong");
}


int function_to_instrospect(const std::string& name, double score) {
  return score * 2;
}

BOOST_AUTO_TEST_CASE (function_traits_test_freestandingfunctions) {
  
  typedef pre::type_traits::function_traits<decltype(function_to_instrospect)> introspector;

    BOOST_ASSERT_MSG(introspector::arity == 2 , " function arity is wrong");
    BOOST_ASSERT_MSG(
      (std::is_same<introspector::result_type, int>::value),
      " return type wasn't correclty extracted");

    BOOST_ASSERT_MSG(
      (std::is_same<introspector::arg<0>, const std::string&>::value),
      " 1st argument should be detected as const std::string&");

    BOOST_ASSERT_MSG(
      (std::is_same<introspector::arg<1>, double>::value),
      " 2nd argument should be detected as double");

    auto std_function = pre::functional::to_std_function(function_to_instrospect);

    BOOST_ASSERT_MSG(std_function(std::string{"Hello World"}, 21.00) == 42 , " call to std::funtion is wrong");
}

class some_class {
  public:
  int member_to_introspect(const std::string& name, double score) const {
    return score * 2;
  }

  int nonconst_member_to_introspect(const std::string& name, double score) {
    return score * 2;
  }
};

BOOST_AUTO_TEST_CASE (function_traits_test_memberfunctions) {

  {
    typedef pre::type_traits::function_traits<decltype(&some_class::member_to_introspect)> introspector;

      BOOST_ASSERT_MSG(introspector::arity == 3 , " function arity is wrong");
      BOOST_ASSERT_MSG(
        (std::is_same<introspector::result_type, int>::value),
        " return type wasn't correclty extracted");

     BOOST_ASSERT_MSG(
        (std::is_same<introspector::arg<0>, some_class>::value),
        " 1st argument should be detected as const std::string&");

      BOOST_ASSERT_MSG(
        (std::is_same<introspector::arg<1>, const std::string&>::value),
        " 1st argument should be detected as const std::string&");

      BOOST_ASSERT_MSG(
        (std::is_same<introspector::arg<2>, double>::value),
        " 2nd argument should be detected as double");

      auto std_function = pre::functional::to_std_function(&some_class::member_to_introspect);
      
      const some_class instance{};
      BOOST_ASSERT_MSG(std_function(instance, std::string{"Hello World"}, 21.00) == 42 , " call to std::funtion is wrong");
  }


  {
    typedef pre::type_traits::function_traits<decltype(&some_class::nonconst_member_to_introspect)> introspector;

      BOOST_ASSERT_MSG(introspector::arity == 3 , " function arity is wrong");
      BOOST_ASSERT_MSG(
        (std::is_same<introspector::result_type, int>::value),
        " return type wasn't correclty extracted");

     BOOST_ASSERT_MSG(
        (std::is_same<introspector::arg<0>, some_class>::value),
        " 1st argument should be detected as const std::string&");

      BOOST_ASSERT_MSG(
        (std::is_same<introspector::arg<1>, const std::string&>::value),
        " 1st argument should be detected as const std::string&");

      BOOST_ASSERT_MSG(
        (std::is_same<introspector::arg<2>, double>::value),
        " 2nd argument should be detected as double");

      auto std_function = pre::functional::to_std_function(&some_class::nonconst_member_to_introspect);
      
      some_class instance{};
      BOOST_ASSERT_MSG(std_function(instance, std::string{"Hello World"}, 21.00) == 42 , " call to std::funtion is wrong");
  }
}
