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
set(CMAKE_AUTOUIC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/lang)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Multimedia REQUIRED)
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

### ctags / cscope
find_program(CTAGS_PATH ctags)
find_program(CSCOPE_PATH cscope)
if(CTAGS_PATH AND CSCOPE_PATH)
	message(STATUS "Found ctags: ${CTAGS_PATH}")
	message(STATUS "Found cscope: ${CSCOPE_PATH}")

	set_source_files_properties(cscope.files PROPERTIES GENERATED true)
	set_source_files_properties(cscope.out PROPERTIES GENERATED true)
	set_source_files_properties(tags PROPERTIES GENERATED true)

	file(GLOB_RECURSE SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.?pp")
	string(REPLACE ";" "\n" SOURCE_FILES "${SOURCE_FILES}")
	file(WRITE "${CMAKE_BINARY_DIR}/cscope.files.in" "${SOURCE_FILES}")
	configure_file("${CMAKE_BINARY_DIR}/cscope.files.in" "${CMAKE_BINARY_DIR}/cscope.files" COPYONLY)

	add_custom_target(tags
		COMMAND ${CTAGS_PATH} --c++-kinds=+p --fields=+iaS --extra=+q -L ${CMAKE_BINARY_DIR}/cscope.files
		COMMAND ${CSCOPE_PATH} -b
		WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
		)
endif()

### packaging
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Mario Konrad")
set(CPACK_PACKAGE_CONTACT "Mario Konrad <mario.konrad@gmx.net>")
set(CPACK_PACAKGE_DESCRIPTION "Radio Simulator - Simrad-RD68")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Radio Simulator for Practicing")
set(CPACK_MONOLITHIC_INSTALL 1)
set(CPACK_PACKAGE_EXECUTABLES "vhf" "vhf")
set(CPACK_STRIP_FILES true)

# TODO: winddows packaging

# TODO: debian packaging

include(CPack)
