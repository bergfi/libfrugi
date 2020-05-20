cmake_minimum_required (VERSION 3.2)
project (libfrugi)
set(libfrugi_VERSION_MAJOR 0)
set(libfrugi_VERSION_MINOR 2)
set(libfrugi_VERSION_PATCH 0)
set(libfrugi_VERSION "${libfrugi_VERSION_MAJOR}.${libfrugi_VERSION_MINOR}.${libfrugi_VERSION_PATCH}")

include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckCSourceCompiles)

## Include source directory and the build directory (for generated files)
include_directories(
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# For clock_gettime() (to measure elapsed time), we need this
CHECK_LIBRARY_EXISTS(rt clock_gettime "time.h" HAVE_CLOCK_GETTIME)
if (NOT HAVE_CLOCK_GETTIME)
   set(CMAKE_EXTRA_INCLUDE_FILES time.h)
   CHECK_FUNCTION_EXISTS(clock_gettime HAVE_CLOCK_GETTIME)
   SET(CMAKE_EXTRA_INCLUDE_FILES)
endif()

FIND_LIBRARY(HAVE_LIBRT NAMES rt)
if(HAVE_LIBRT)
    set(CMAKE_REQUIRED_LIBRARIES ${CMAKE_REQUIRED_LIBRARIES} rt)
endif()
check_c_source_compiles(
    "
    #define _POSIX_C_SOURCE 199309L
    #include <time.h>
    int main() { return clock_gettime(CLOCK_MONOTONIC, NULL); }
    "
    HAS_POSIX_CLOCK_MONOTONIC
)
check_c_source_compiles(
    "
    #define _POSIX_C_SOURCE 199309L
    #include <time.h>
    int main() { return clock_gettime(CLOCK_MONOTONIC_RAW, NULL); }
    "
    HAS_POSIX_CLOCK_MONOTONIC_RAW
)
set(CMAKE_REQUIRED_LIBRARIES)
if(HAS_POSIX_CLOCK_MONOTONIC)
    add_definitions(-DHAS_POSIX_CLOCK_MONOTONIC)
endif()
if(HAS_POSIX_CLOCK_MONOTONIC_RAW)
    add_definitions(-DHAS_POSIX_CLOCK_MONOTONIC_RAW)
endif()

check_include_file("unistd.h" HAVE_UNISTD_H)

## Specify the library and its sources
add_library(libfrugi
	src/FileWriter.cpp
	src/ConsoleWriter.cpp
	src/MessageFormatter.cpp
	src/Shell.cpp
	src/FileSystem.cpp
	src/System.cpp
)
set_target_properties(libfrugi PROPERTIES OUTPUT_NAME "frugi")
set_property(TARGET libfrugi PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET libfrugi PROPERTY CXX_STANDARD 11)
set_property(TARGET libfrugi PROPERTY CXX_STANDARD_REQUIRED ON)

## Installation
install (TARGETS libfrugi DESTINATION lib)
install (DIRECTORY
	"${CMAKE_CURRENT_SOURCE_DIR}/include/libfrugi"
	DESTINATION include/libfrugi
)

target_include_directories(libfrugi
		INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include"
		)

configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/src/config.h"
)

if(UNIX)
	set(INCLUDE_INSTALL_DIR "include/libfrugi")
	set(LIB_INSTALL_DIR "lib${LIB_SUFFIX}")
	set(PC_FILE ${CMAKE_BINARY_DIR}/libfrugi.pc)
	configure_file("libfrugi.pc.in" ${PC_FILE} @ONLY)
	install(FILES ${PC_FILE} DESTINATION ${LIB_INSTALL_DIR}/pkgconfig)
endif()
