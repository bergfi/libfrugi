cmake_minimum_required (VERSION 3.2)
project (libfrugi)

set(libfrugi_VERSION_MAJOR 0)
set(libfrugi_VERSION_MINOR 2)
set(libfrugi_VERSION_PATCH 0)
set(libfrugi_VERSION "${libfrugi_VERSION_MAJOR}.${libfrugi_VERSION_MINOR}.${libfrugi_VERSION_PATCH}")
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files")
set(INSTALL_SYSCONFIG_DIR etc/foo CACHE PATH "Installation directory for config files")

if(WIN32 AND NOT CYGWIN)
	set(DEF_INSTALL_CMAKE_DIR CMake)
else()
	set(DEF_INSTALL_CMAKE_DIR lib/cmake/libfrugi)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckCSourceCompiles)
include(CheckIncludeFile)
include(CMakePackageConfigHelpers)

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
    HAVE_POSIX_CLOCK_MONOTONIC
)
check_c_source_compiles(
    "
    #define _POSIX_C_SOURCE 199309L
    #include <time.h>
    int main() { return clock_gettime(CLOCK_MONOTONIC_RAW, NULL); }
    "
	HAVE_POSIX_CLOCK_MONOTONIC_RAW
)
set(CMAKE_REQUIRED_LIBRARIES)

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
set_property(TARGET libfrugi PROPERTY CXX_STANDARD 17)
set_property(TARGET libfrugi PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET libfrugi PROPERTY VERSION ${libfrugi_VERSION})
set_property(TARGET libfrugi PROPERTY SOVERSION ${libfrugi_VERSION_MAJOR})
set_property(TARGET libfrugi PROPERTY DEBUG_POSTFIX d)

target_include_directories(libfrugi
		PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include> $<INSTALL_INTERFACE:include>
		)

configure_file (
	"${CMAKE_CURRENT_SOURCE_DIR}/src/config.h.in"
	"${CMAKE_CURRENT_BINARY_DIR}/include/libfrugi/Config.h"
)

configure_package_config_file(libfrugiConfig.cmake.in
		"${CMAKE_BINARY_DIR}/cmake/libfrugiConfig.cmake"
		INSTALL_DESTINATION ${INSTALL_LIB_DIR}/libfrugi/cmake
		PATH_VARS INSTALL_INCLUDE_DIR
		)
write_basic_package_version_file(
		"${CMAKE_BINARY_DIR}/cmake/libfrugiConfigVersion.cmake"
		VERSION "${libfrugi_VERSION}"
		COMPATIBILITY SameMajorVersion
)

if(UNIX)
	set(PC_FILE ${CMAKE_BINARY_DIR}/libfrugi.pc)
	configure_file("libfrugi.pc.in" ${PC_FILE} @ONLY)
	install(FILES ${PC_FILE} DESTINATION ${INSTALL_LIB_DIR}/pkgconfig)
endif()

## Installation
install(TARGETS libfrugi
		EXPORT libfrugiTargets
		RUNTIME DESTINATION "${INSTALL_BIN_DIR}" COMPONENT bin
		LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT shlib
		PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}" COMPONENT dev
		)
install(EXPORT libfrugiTargets
		FILE libfrugiTargets.cmake
		DESTINATION lib/cmake/libfrugi
		)
install(DIRECTORY
		"${CMAKE_CURRENT_SOURCE_DIR}/include/libfrugi"
		DESTINATION include
		)
install(DIRECTORY
		"${CMAKE_CURRENT_BINARY_DIR}/include/libfrugi"
		DESTINATION include
		)
install(FILES
		"${CMAKE_BINARY_DIR}/cmake/libfrugiConfig.cmake"
		"${CMAKE_BINARY_DIR}/cmake/libfrugiConfigVersion.cmake"
		DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
