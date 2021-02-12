add_subdirectory(lua)
add_subdirectory(engine)
add_subdirectory(nmea)
add_subdirectory(vhf)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/version.cpp.in
	${CMAKE_BINARY_DIR}/version.cpp
	)

