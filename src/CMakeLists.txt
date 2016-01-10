cmake_minimum_required(VERSION 3.1)

message(STATUS "compile flags: ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

add_executable(radio-sim
	main.cpp
	)

target_include_directories(radio-sim
	PRIVATE
		${Qt5Core_INCLUDE_DIRS}
	)

target_link_libraries(radio-sim
	Qt5::Core
	Qt5::Widgets
	Qt5::Network
	Qt5::Script
	)

install(
	TARGETS radio-sim
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	COMPONENT applications
	)

