cmake_minimum_required(VERSION 3.1)

add_library(util STATIC
	Rectangle.cpp
	Circle.cpp
	)

target_compile_options(util
	PUBLIC
		-Wall
		-Wextra
		-std=c++11
		${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}
	)

target_include_directories(util
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
		${Qt5Gui_INCLUDE_DIRS}
	)

