
# This is an #inglued <> generated CMakeLists.txt (https://github.com/header-only/inglued)
# To modify it edit CMakeLists.txt.tpl ( i.e. Generate it with `inglued cmaketpl` )


cmake_minimum_required(VERSION 3.7.2)

project(js-bind VERSION "0.0.1")
enable_testing()

set(CMAKE_CXX_STANDARD 14)

# Compile with shipped-with headers or without 
option(INGLUED "Enable use of #inglued shipped with dependencies." ON)

# Compile unit tests
option(UNIT_TESTS "Enable Unit Testing" OFF)


# Warning as errors to ensure js-bind quality
string(TOUPPER "${CMAKE_CXX_COMPILER_ID}" COMPILER_IN_USE)
if ("${COMPILER_IN_USE}" STREQUAL "GNU" OR "${COMPILER_IN_USE}" MATCHES "CLANG")
	add_definitions(
    -Wall
		-Werror
		-Wno-unused-local-typedefs
		-Wno-unused-variable
  )
endif()



# preprocessor
if (INGLUED)
  include_directories(AFTER deps/boostorg/preprocessor/include/)
else ()
  find_package(Boost REQUIRED)
endif()

# type_traits
if (INGLUED)
  include_directories(AFTER deps/cpp-pre/type_traits/)
else ()
  find_package(cpp-pre REQUIRED)
endif()


# Define header only library
include_directories(BEFORE ${CMAKE_CURRENT_LIST_DIR})
add_library(js-bind INTERFACE)
add_library(cpp-js::js-bind ALIAS js-bind)

set(include_install_dir "include")

if (INGLUED)

  target_include_directories(js-bind INTERFACE 
    # Transitive libraries location once installed
    $<INSTALL_INTERFACE:${include_install_dir}/js/deps> 
  )


else()

  target_link_libraries(js-bind INTERFACE 
      Boost::boost
      cpp-pre::type_traits
  )

endif()


if( UNIT_TESTS )
 add_subdirectory(test)
endif()

# IDE Supports (XCode, VS, QtCreator they don't list headers)
file (GLOB_RECURSE HPP_FILES js/*.h*)
add_custom_target(sources SOURCES ${HPP_FILES})

# Installing

# Layout. This works for all platforms:
#   * <prefix>/lib/cmake/<PROJECT-NAME>
#   * <prefix>/lib/
#   * <prefix>/include/
set(config_install_dir "lib/cmake/${PROJECT_NAME}")

set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

# Configuration
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "${PROJECT_NAME}::")

# Include module with fuction 'write_basic_package_version_file'
include(CMakePackageConfigHelpers)

# Configure '<PROJECT-NAME>ConfigVersion.cmake'
# Note: PROJECT_VERSION is used as a VERSION
write_basic_package_version_file(
    "${version_config}" COMPATIBILITY SameMajorVersion
)

# Configure '<PROJECT-NAME>Config.cmake'
# Use variables:
#   * targets_export_name
#   * PROJECT_NAME
configure_package_config_file(
    "cmake/modules/Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${config_install_dir}"
)


# Targets:
install(
    TARGETS js-bind
    EXPORT "${targets_export_name}"
    LIBRARY DESTINATION "lib"
    ARCHIVE DESTINATION "lib"
    RUNTIME DESTINATION "bin"
    INCLUDES DESTINATION "${include_install_dir}"
)

# Headers:
install(
    DIRECTORY js
    DESTINATION "${include_install_dir}"
    FILES_MATCHING PATTERN "*.[ih]*"
)

# Install the deps when run in INGLUED mode
if (INGLUED)


  install(
      DIRECTORY deps/boostorg/preprocessor/include/
      DESTINATION ${include_install_dir}/js/deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/boostorg/preprocessor/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


  install(
      DIRECTORY deps/cpp-pre/type_traits/
      DESTINATION ${include_install_dir}/js/deps
      FILES_MATCHING PATTERN "*.[ih]*"
      PATTERN deps/cpp-pre/type_traits/deps EXCLUDE # Transitive deps are hikedup on `glue seal`.
      )


endif(INGLUED)

# Config
#   * <prefix>/lib/cmake/js-bind/js-bindConfig.cmake
#   * <prefix>/lib/cmake/js-bind/js-bindConfigVersion.cmake
#   * <prefix>/lib/cmake/js-bind/js-bindTargets.cmake
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)
install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "${config_install_dir}"
)

