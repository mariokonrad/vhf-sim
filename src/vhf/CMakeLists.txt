cmake_minimum_required(VERSION 3.2)

qt5_add_translation(QM_FILES ${TRANSLATION_FILES})

add_executable(simrad-rd68
	main.cpp
	MainWindow.cpp
	Widget.cpp
	System.cpp
	MsgSenderSocket.cpp
	MsgSenderControlCenter.cpp
	vhfpreferences.cpp
	connectionpreferences.cpp
	gpswindow.cpp
	controlcenter.cpp
	PeerModel.cpp
	etherwindow.cpp
	${UI_HEADERS}
	${QM_FILES}
	)

add_custom_command(TARGET simrad-rd68 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E echo "Copy language files"
	COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/lang/
	)

add_custom_command(TARGET simrad-rd68 POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E echo "Copy Simrad-RD68 files to: ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/simrad-rd68/"
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data/simrad-rd68 ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/simrad-rd68/
	)

target_compile_options(simrad-rd68
	PUBLIC
		-Wall
		-Wextra
		-std=c++11
		${Qt5Core_EXECUTABLE_COMPILE_FLAGS}
		${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}
		${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}
		${Qt5Multimedia_EXECUTABLE_COMPILE_FLAGS}
	)

target_include_directories(simrad-rd68
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_BINARY_DIR}/..
		${CMAKE_CURRENT_SOURCE_DIR}/..
		${Qt5Core_INCLUDE_DIRS}
		${Qt5Gui_INCLUDE_DIRS}
		${Qt5Widgets_INCLUDE_DIRS}
		${Qt5Multimedia_INCLUDE_DIRS}
	)

target_link_libraries(simrad-rd68
	util
	nmea
	engine
	lua
	Qt5::Core
	Qt5::Widgets
	Qt5::Gui
	Qt5::Multimedia
	Qt5::Network
	)

install(
	TARGETS simrad-rd68
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
	add_custom_command(TARGET simrad-rd68 POST_BUILD
		COMMAND ${WINDEPLOYQT} --release $<TARGET_FILE:simrad-rd68>
		)
endif()
