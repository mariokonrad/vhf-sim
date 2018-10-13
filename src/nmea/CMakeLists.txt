add_library(nmea STATIC
	nmea.cpp
	nmea_sentence.cpp
	)

target_compile_options(nmea
	PRIVATE
		-Wall
		-Wextra
	)

target_link_libraries(nmea
	PRIVATE
		engine
	)

target_include_directories(nmea
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/..
	)
