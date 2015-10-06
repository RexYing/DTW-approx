# Created by the script cgal_create_cmake_script_with_options
# This is the CMake script for compiling a set of CGAL applications.

project( executable )


cmake_minimum_required(VERSION 2.6.2)
if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" VERSION_GREATER 2.6)
  if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION}" VERSION_GREATER 2.8.3)
    cmake_policy(VERSION 2.8.4)
  else()
    cmake_policy(VERSION 2.6)
  endif()
endif()

set( CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true )
set(CMAKE_CXX_FLAGS "-lboost_program_options")

if ( COMMAND cmake_policy )

  cmake_policy( SET CMP0003 NEW )

endif()

# CGAL and its components
find_package( CGAL QUIET COMPONENTS  )

if ( NOT CGAL_FOUND )

  message(STATUS "This project requires the CGAL library, and will not be compiled.")
  return()

endif()

# include helper file
include( ${CGAL_USE_FILE} )


# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()

endif()

# include for local directory

# include for local package

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please
		use a different C++ compiler.")
endif()


# Creating entries for target: executable
# ############################

include_directories ("${PROJECT_SOURCE_DIR}")
INCLUDE_DIRECTORIES("${Boost_INCLUDE_DIR}")
link_directories("${Boost_LIBRARY_DIR}")

add_library(
  WSPD
  QuadTree.cpp QuadTree.h
  QuadTreeTwoClasses.cpp QuadTreeTwoClasses.h
  WSPD.cpp WSPD.h
)
add_library( Frechet FrechetDecider.cpp FrechetDecider.h )

add_library( Util util/joiner.h)
set_target_properties( Util PROPERTIES LINKER_LANGUAGE CXX)

add_executable(
  executable
	grid.cpp grid.h
	rect_cluster.cpp rect_cluster.h
  main.cpp
)
add_executable(
  gen_tests
  gen_tests.cpp
)
# target_compile_features(executable PRIVATE cxx_range_for)

add_to_cached_list( CGAL_EXECUTABLE_TARGETS executable )

# Link the executable to CGAL and third-party libraries
target_link_libraries(
	executable   ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES}
	WSPD
	Frechet
	Util
)

target_link_libraries(
	gen_tests   ${Boost_LIBRARIES} ${CGAL_LIBRARIES} ${CGAL_3RD_PARTY_LIBRARIES}
)

