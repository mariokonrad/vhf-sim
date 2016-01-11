cmake_minimum_required(VERSION 3.1)

configure_file(
	${CMAKE_CURRENT_SOURCE_DIR}/version.hpp.in
	${CMAKE_CURRENT_BINARY_DIR}/version.hpp
	)

qt5_add_translation(QM_FILES ${TRANSLATION_FILES})

add_executable(radio-sim
	radio.cpp
	MainWindow.cpp
	action/About.cpp
	action/AboutQt.cpp
	action/ExitApplication.cpp
	action/Fullscreen.cpp
	${QM_FILES}
	)

add_custom_command(TARGET radio-sim POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/lang/
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

install(
	FILES ${QM_FILES}
	DESTINATION share/${PROJECT_NAME}/lang
	)

### windows deployment
if(WIN32)
	find_program(WINDEPLOYQT windeployqt
		PATHS
			$ENV{QTHOME}
		PATH_SUFFIXES
			bin
		)
	if(NOT WINDEPLOYQT)
		message(FATAL_ERROR "NOT FOUND: windeployqt")
	endif()
	message(STATUS "Found: ${WINDEPLOYQT}")
	add_custom_command(TARGET radio-sim POST_BUILD
		COMMAND ${WINDEPLOYQT} --release $<TARGET_FILE:radio-sim>
		)
endif()
