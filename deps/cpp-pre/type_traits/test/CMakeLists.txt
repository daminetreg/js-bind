# \brief add_unit_test(NAME <test name> LIBS <libraries to link>
# A file <test name>.cpp has to exists for the test to be valid. 
# The main method <test name>.cpp should return non-zero on failure.
function(add_unit_test)
  set(options)
  set(oneValueArgs NAME)
  set(multiValueArgs LIBS)
  cmake_parse_arguments(add_unit_test "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  add_executable(test_${add_unit_test_NAME} 
    ${add_unit_test_NAME}.cpp)

  target_link_libraries(test_${add_unit_test_NAME} 
    ${add_unit_test_LIBS})

  add_test(NAME test_${add_unit_test_NAME}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMAND ${CMAKE_CURRENT_BINARY_DIR}/test_${add_unit_test_NAME} )

endfunction(add_unit_test) 


# Boost
find_package(Boost 1.60 REQUIRED)
include_directories(AFTER ${Boost_INCLUDE_DIRS})
link_directories ( ${Boost_LIBRARY_DIRS} )

include_directories(BEFORE ${PROJECT_SOURCE_DIR})

# function_traits_test
add_unit_test(NAME function_traits_test)
