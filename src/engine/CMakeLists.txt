cmake_minimum_required(VERSION 3.1)

add_library(engine STATIC
	Angle.cpp
	Date.cpp
	Engine.cpp
	Latitude.cpp
	Longitude.cpp
	MMSI.cpp
	msg.cpp
	)

target_compile_options(engine
	PUBLIC
		-Wall
		-Wextra
		-std=c++11
	)

target_include_directories(engine
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}/../lua/include
		${Qt5Core_INCLUDE_DIRS}
	)
