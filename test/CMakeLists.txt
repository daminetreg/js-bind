add_definitions(-std=c++14)

if (CMAKE_BUILD_TYPE MATCHES Debug)
  add_definitions(-g4) # Generates emscripten C++ sourcemaps
endif()

# bind
add_executable(bind bind.cpp)
set_target_properties(bind PROPERTIES SUFFIX ".html" LINK_FLAGS " --bind -s  NO_EXIT_RUNTIME=1 ")
target_link_libraries(bind cpp-js::js-bind)

# example
add_executable(example example.cpp)
set_target_properties(example PROPERTIES SUFFIX ".html" LINK_FLAGS " --bind -s  NO_EXIT_RUNTIME=1 ")
target_link_libraries(example cpp-js::js-bind)
