add_library(engine STATIC)

target_sources(engine
	PRIVATE
		Angle.cpp
		Date.cpp
		Engine.cpp
		Latitude.cpp
		Longitude.cpp
		MMSI.cpp
		msg.cpp
	)

target_compile_options(engine
	PRIVATE
		-Wall
		-Wextra
	)

target_include_directories(engine
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}/..
	PRIVATE
		${Qt5Core_INCLUDE_DIRS}
	)

target_link_libraries(engine
	PRIVATE
		lua
	)
