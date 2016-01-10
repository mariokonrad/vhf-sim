cmake_minimum_required(VERSION 3.1)

message(STATUS "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
message(STATUS "CMAKE_MODULE_PATH=${CMAKE_MODULE_PATH}")
include(GitInformation)

project(radio-sim VERSION 0.0.0 LANGUAGES CXX)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

if(WIN32)
	if(NOT $ENV{QTHOME})
		set(ENV{QTHOME} "c:\\Qt\\5.5\\mingw492_32")
	endif()
else()
	if(NOT $ENV{QTHOME})
		set(ENV{QTHOME} "$ENV{HOME}/local/Qt/5.5/gcc_64")
	endif()
endif()
message(STATUS "QTHOME: $ENV{QTHOME}")
set(CMAKE_PREFIX_PATH "$ENV{QTHOME}")

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)

add_subdirectory(src)
