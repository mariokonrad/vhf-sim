cmake_minimum_required(VERSION 3.2)

qt5_add_translation(QM_FILES ${TRANSLATION_FILES})

add_executable(vhf-sim ${GUI_TYPE}
	main.cpp
	Circle.cpp
	Rectangle.cpp
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
	communicationshubwindow.cpp
	${UI_HEADERS}
	${QM_FILES}
	resources.qrc
	resources.rc
	)

target_compile_options(vhf-sim
	PUBLIC
		-Wall
		-Wextra
		-std=c++11
		${Qt5Core_EXECUTABLE_COMPILE_FLAGS}
		${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}
		${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}
		${Qt5Multimedia_EXECUTABLE_COMPILE_FLAGS}
	)

target_include_directories(vhf-sim
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_BINARY_DIR}/..
		${CMAKE_CURRENT_SOURCE_DIR}/..
		${Qt5Core_INCLUDE_DIRS}
		${Qt5Gui_INCLUDE_DIRS}
		${Qt5Widgets_INCLUDE_DIRS}
		${Qt5Multimedia_INCLUDE_DIRS}
	)

target_link_libraries(vhf-sim
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
	TARGETS vhf-sim
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	COMPONENT applications
	)

# language files
add_custom_command(TARGET vhf-sim POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E echo "Copy language files"
	COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/lang/
	)

install(
	FILES ${QM_FILES}
	DESTINATION share/${PROJECT_NAME}/lang
	)

# simrad-rd68 files
add_custom_command(TARGET vhf-sim POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E echo "Copy Simrad-RD68 files to: ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/simrad-rd68/"
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data/simrad-rd68 ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/simrad-rd68/
	)

install(
	DIRECTORY "${CMAKE_SOURCE_DIR}/data/simrad-rd68"
	DESTINATION "share/${PROJECT_NAME}"
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

	# install necessary libraries in binary folder
	add_custom_command(TARGET vhf-sim POST_BUILD
		COMMAND ${WINDEPLOYQT}
			--release
			--dir ${CMAKE_BINARY_DIR}/bin
			--verbose 0
			$<TARGET_FILE:vhf-sim>
		)

	# install necessary libraries
	add_custom_command(TARGET vhf-sim POST_BUILD
		COMMAND ${WINDEPLOYQT}
			--release
			--dir ${CMAKE_BINARY_DIR}/qt
			--verbose 0
			$<TARGET_FILE:vhf-sim>
		)
	install(DIRECTORY "${CMAKE_BINARY_DIR}/qt/" DESTINATION bin)
endif()
