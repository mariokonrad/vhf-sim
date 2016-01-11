cmake_minimum_required(VERSION 3.1)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include(GitInformation)

project(radio-sim VERSION 0.0.0 LANGUAGES CXX)

### compiler check
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message(STATUS "Compiler: GNU")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	message(STATUS "Compiler: Clang")
else()
	message(FATAL_ERROR "Unsupported compiler detected: ${CMAKE_CXX_COMPILER_ID}")
endif()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(NOT $ENV{QTHOME})
	if(WIN32)
		set(ENV{QTHOME} "c:\\Qt\\5.5\\mingw492_32")
	else()
		set(ENV{QTHOME} "$ENV{HOME}/local/Qt/5.5/gcc_64")
	endif()
endif()
message(STATUS "QTHOME: $ENV{QTHOME}")
set(CMAKE_PREFIX_PATH "$ENV{QTHOME}")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Script REQUIRED)

add_subdirectory(src)

### Testing

# TODO: testing

### packaging

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_NAME "radio-sim")
set(CPACK_PACKAGE_VENDOR "Mario Konrad")
set(CPACK_PACKAGE_CONTACT "Mario Konrad <mario.konrad@gmx.net>")
set(CPACK_PACAKGE_DESCRIPTION "Radio Simulator")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Radio Simulator for Practicing")
set(CPACK_MONOLITHIC_INSTALL 1)
set(CPACK_PACKAGE_EXECUTABLES "radio-sim" "radio-sim")
set(CPACK_STRIP_FILES true)

# TODO: winddows packaging

# TODO: debian packaging

include(CPack)
