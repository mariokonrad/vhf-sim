cmake_minimum_required(VERSION 3.12)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

### module and prefix path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
if(DEFINED ENV{QTHOME})
	message(STATUS "QTHOME: $ENV{QTHOME}")
	set(CMAKE_PREFIX_PATH "$ENV{QTHOME}")
endif()

### information about repository and project
include(GitInformation)
project(vhf-sim VERSION 2.0.0 LANGUAGES CXX C)
set(PROJECT_LICENSE "GPLv3")

### build type
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

### compiler check
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID}")
	message(STATUS "C Compiler  : ${CMAKE_C_COMPILER_ID}")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER_ID}")
	message(STATUS "C Compiler  : ${CMAKE_C_COMPILER_ID}")
else()
	message(FATAL_ERROR "Unsupported compiler detected: ${CMAKE_CXX_COMPILER_ID}")
endif()

# C++11, no compiler extensions
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

### GUI options
if(WIN32)
	set(GUI_TYPE "WIN32")
elseif(APPLE)
	set(GUI_TYPE "MACOSX_BUNDLE")
else()
	set(GUI_TYPE "")
endif()

### package handling
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/lang)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5LinguistTools REQUIRED)

### internationalization
set(TRANSLATION_FILES
	${CMAKE_CURRENT_SOURCE_DIR}/lang/de_DE.ts
	)

add_custom_target(language-update)
foreach(filename ${TRANSLATION_FILES})
	add_custom_command(TARGET language-update PRE_BUILD
		COMMAND ${Qt5_LUPDATE_EXECUTABLE} -no-obsolete -recursive ${CMAKE_CURRENT_SOURCE_DIR}/src -ts ${filename}
		)
endforeach()

### sources
add_subdirectory(src)

### Testing

# TODO: testing

### ctags / cscope
include(CTags)

### packaging
include(CPack.local)

