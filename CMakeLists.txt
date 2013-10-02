## DFT2LNT Library project
project (libfrugi)
set (libfrugi_VERSION_MAJOR 0)
set (libfrugi_VERSION_MINOR 1)

cmake_minimum_required (VERSION 2.6)

## Not sure if needed
add_definitions( -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS )
#add_definitions(-std=c++0x -D_GNU_SOURCE)
add_definitions(-std=gnu++0x -Wall)

## Include source directory and the build directory (for generated files)
include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

## By default the header is not cleaned up (nor built)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${CMAKE_CURRENT_BINARY_DIR}/lexer.l.h)

## Specify the library and its sources
add_library(libfrugi SHARED
	src/FileWriter.cpp
	src/ConsoleWriter.cpp
	src/MessageFormatter.cpp
	src/Shell.cpp
	src/FileSystem.cpp
	src/System.cpp
)
set_target_properties(libfrugi PROPERTIES OUTPUT_NAME "frugi")

# For clock_gettime() (to measure elapsed time), we need this
if(UNIX AND NOT APPLE AND NOT WIN32)
	target_link_libraries(libfrugi rt)
endif()

## Installation
install (TARGETS libfrugi DESTINATION lib)
install (DIRECTORY
	"${CMAKE_CURRENT_SOURCE_DIR}/include/libfrugi"
	DESTINATION include/libfrugi
)