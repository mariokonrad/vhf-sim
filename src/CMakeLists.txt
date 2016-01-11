cmake_minimum_required(VERSION 3.1)

add_subdirectory(lua)
add_subdirectory(engine)
add_subdirectory(nmea)
add_subdirectory(util)
add_subdirectory(vhf)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in
	${CMAKE_CURRENT_BINARY_DIR}/version.hpp
	)

