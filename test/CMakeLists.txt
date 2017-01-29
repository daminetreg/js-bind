cmake_minimum_required(VERSION 3.0.0)
project (jssupport-test)

add_definitions(-std=c++14)
set (CMAKE_CXX_STANDARD 14)

if (DEFINED EMSCRIPTEN)
  if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-g4) # Generates emscripten C++ sourcemaps
  endif()
endif()

# Boost
include_directories (AFTER /opt/libs/boost_1_61_0/)

include_directories(BEFORE ${CMAKE_CURRENT_LIST_DIR}/..)

# bind
add_executable(bind bind.cpp)
set_target_properties(bind PROPERTIES SUFFIX ".html" LINK_FLAGS " --bind -s  NO_EXIT_RUNTIME=1 ")
em_link_js_library(bind ${libraryJsFiles})

