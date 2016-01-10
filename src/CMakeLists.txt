cmake_minimum_required(VERSION 3.1)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in
	${CMAKE_CURRENT_BINARY_DIR}/version.hpp
	)

add_executable(radio-sim
	main.cpp
	MainWindow.cpp
	action/About.cpp
	action/AboutQt.cpp
	action/ExitApplication.cpp
	)

target_compile_options(radio-sim
	PUBLIC
		-Wall
		-Wextra
		-std=c++11
		${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}
	)

target_include_directories(radio-sim
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
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

