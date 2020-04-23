cmake_minimum_required ( VERSION 3.9 )

include(CheckIncludeFile)

project ( cliutils C )

# Set the project source dir (just convention)
set( CLIUTILS_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR} )
set( CLIUTILS_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR} )

# It's useful to setup the current version of our code in the build system
# using a `semver` style
set ( CLIUTILS_VERSION_MAJOR 0 )
set ( CLIUTILS_VERSION_MINOR 1 )
set ( CLIUTILS_VERSION_PATCH 0 )

# Include Directories
# In GCC, this will invoke the "-I" command
include_directories( src )

# Lists
# Setup the list of source files
set( CLIUTILS_SOURCES
  src/cliutils.c
  src/cli_common.c

  src/cliutils.h
  src/cli_common.h
)

include(FindLua)
if (LUA_FOUND)
  message("-- lua found at ${LUA_LIBRARIES} ${LUA_INCLUDE_DIR}")
  message("-- lua version is ${LUA_VERSION_STRING}")
  add_compile_definitions(LUA_ENABLED)
else ()
  message( FATAL_ERROR " -- ERROR: lua not found.")
endif (LUA_FOUND)

add_executable( ${PROJECT_NAME} ${CLIUTILS_SOURCES} )
target_include_directories(${PROJECT_NAME} PRIVATE ${YOUR_DIRECTORY})

# see mustach documentation for this
add_definitions(-DNO_OPEN_MEMSTREAM)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 11)

set(CMAKE_FIND_USE_PACKAGE_REGISTRY ON)

find_package(coll CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PUBLIC coll::coll)

check_include_file("getopt.h" HAVE_GETOPT)

if(NOT HAVE_GETOPT)
	# To find and use linenoise, as the vcpkg build does not have cmake config
	find_path(GETOPT_INCLUDE_DIR getopt.h)
	find_library(GETOPT_LIBRARY getopt)
	include_directories(${GETOPT_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} PUBLIC ${GETOPT_LIBRARY})
endif (NOT HAVE_GETOPT)
