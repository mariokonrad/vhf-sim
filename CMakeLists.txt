cmake_minimum_required(VERSION 3.1)

### module and prefix path
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
if(NOT DEFINED ENV{QTHOME})
	message(FATAL_ERROR "Environment variable QTHOME not set")
endif()
message(STATUS "QTHOME: $ENV{QTHOME}")
set(CMAKE_PREFIX_PATH "$ENV{QTHOME}")

### information about repository and project
include(GitInformation)
project(simrad-rd68 VERSION 0.0.0 LANGUAGES CXX)

### compiler check
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message(STATUS "Compiler: GNU")
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	message(STATUS "Compiler: Clang")
else()
	message(FATAL_ERROR "Unsupported compiler detected: ${CMAKE_CXX_COMPILER_ID}")
endif()

### package handling
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/lang)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Script REQUIRED)
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

### packaging
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Mario Konrad")
set(CPACK_PACKAGE_CONTACT "Mario Konrad <mario.konrad@gmx.net>")
set(CPACK_PACAKGE_DESCRIPTION "Radio Simulator")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Radio Simulator for Practicing")
set(CPACK_MONOLITHIC_INSTALL 1)
set(CPACK_PACKAGE_EXECUTABLES "simrad-rd68" "simrad-rd68")
set(CPACK_STRIP_FILES true)

# TODO: winddows packaging

# TODO: debian packaging

include(CPack)
