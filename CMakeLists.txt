cmake_minimum_required ( VERSION 3.9 )

project ( zclk C )

# Export all symbols for shared linking to work on windows
# see https://stackoverflow.com/questions/37128555/getting-cmake-to-build-shared-library-for-msvc
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS 1)

# Set the project source dir (just convention)
set( ZCLK_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( ZCLK_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )

# version numbers in semver style
# matching the release tag version
set ( ZCLK_VERSION_MAJOR "0" )
set ( ZCLK_VERSION_MINOR "1" )
set ( ZCLK_VERSION_PATCH "0" )
set ( ZCLK_VERSION_PRERELEASE "-alpha.0" )
set ( ZCLK_VERSION_STR ${ZCLK_VERSION_MAJOR}.${ZCLK_VERSION_MINOR}.${ZCLK_VERSION_PATCH}${ZCLK_VERSION_PRERELEASE})

# https://stackoverflow.com/a/47260387/9483968
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include Directories
# In GCC, this will invoke the "-I" command
include_directories( src )

# Lists
# Setup the list of source files
set( ZCLK_SOURCES
  src/zclk.c
  src/zclk_common.c
  src/zclk_table.c
  src/zclk_dict.c
  src/zclk_progress.c

  src/zclk.h
  src/zclk_common.h
  src/zclk_table.h
  src/zclk_dict.h
  src/zclk_progress.h
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
export(TARGETS ${PROJECT_NAME} NAMESPACE ${PROJECT_NAME}:: FILE ${PROJECT_NAME}Config.cmake)
set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
export(PACKAGE ${PROJECT_NAME})

#-------------------- ZCLK SAMPLES --------------------
add_executable(        s1_basic_usage_arg_option   samples/s1_basic_usage_arg_option.c )
target_link_libraries( s1_basic_usage_arg_option   ${PROJECT_NAME} )

add_executable(        s2_sub_commands   samples/s2_sub_commands.c )
target_link_libraries( s2_sub_commands   ${PROJECT_NAME} )
