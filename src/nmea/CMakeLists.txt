cmake_minimum_required(VERSION 3.1)

add_library(nmea STATIC
	nmea.cpp
	nmea_sentence.cpp
	)

target_compile_options(nmea
	PUBLIC
		-Wall
		-Wextra
		-std=c++11
	)

target_include_directories(nmea PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_dependencies(nmea engine)
