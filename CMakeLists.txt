cmake_minimum_required ( VERSION 3.9 )

project ( cliutils C )

# Export all symbols for shared linking to work on windows
# see https://stackoverflow.com/questions/37128555/getting-cmake-to-build-shared-library-for-msvc
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

# Set the project source dir (just convention)
set( ZCLK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( ZCLK_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )

# It's useful to setup the current version of our code in the build system
# using a `semver` style
set ( ZCLK_VERSION_MAJOR 0 )
set ( ZCLK_VERSION_MINOR 1 )
set ( ZCLK_VERSION_PATCH 0 )

# https://stackoverflow.com/a/47260387/9483968
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include Directories
# In GCC, this will invoke the "-I" command
include_directories( src )

# Lists
# Setup the list of source files
set( ZCLK_SOURCES
  src/zclk.c
  src/cli_common.c
  src/cli_table.c
  src/cli_dict.c
  src/cli_progress.c

  src/zclk.h
  src/cli_common.h
  src/cli_table.h
  src/cli_dict.h
  src/cli_progress.h
)

add_library( ${PROJECT_NAME} SHARED ${ZCLK_SOURCES} )
target_include_directories(${PROJECT_NAME} PUBLIC src)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)

include(FindLua)
if (LUA_FOUND)
  message("-- lua found at ${LUA_LIBRARIES} ${LUA_INCLUDE_DIR}")
  message("-- lua version is ${LUA_VERSION_STRING}")
  add_compile_definitions(LUA_ENABLED)
  target_include_directories(${PROJECT_NAME} PUBLIC ${LUA_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${LUA_LIBRARIES})
else ()
  message( FATAL_ERROR " -- ERROR: lua not found.")
endif (LUA_FOUND)

set(CMAKE_FIND_USE_PACKAGE_REGISTRY ON)

find_package(coll CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC coll::coll)

# Package Configuration
export(TARGETS cliutils NAMESPACE cliutils:: FILE cliutilsConfig.cmake)
set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(PACKAGE cliutils)

#-------------------- ZCLK SAMPLES --------------------
add_executable(        s1_basic_usage_arg_option   samples/s1_basic_usage_arg_option.c )
target_link_libraries( s1_basic_usage_arg_option   ${PROJECT_NAME} )
