qt5_add_translation(QM_FILES ${TRANSLATION_FILES})

add_executable(vhf-sim WIN32 MACOSX_BUNDLE)

if(APPLE)
	set(OSX_ICON_DIR ${CMAKE_BINARY_DIR}/vhf-main.iconset)
	file(MAKE_DIRECTORY ${OSX_ICON_DIR})

	add_custom_command(OUTPUT ${CMAKE_BINARY_DIR}/vhf-main.icns
		COMMAND cp icons/vhf-main_x16.png   ${OSX_ICON_DIR}/icon_16x16.png
		COMMAND cp icons/vhf-main_x32.png   ${OSX_ICON_DIR}/icon_16x16@2x.png
		COMMAND cp icons/vhf-main_x32.png   ${OSX_ICON_DIR}/icon_32x32.png
		COMMAND cp icons/vhf-main_x64.png   ${OSX_ICON_DIR}/icon_32x32@2x.png
		COMMAND cp icons/vhf-main_x64.png   ${OSX_ICON_DIR}/icon_64x64.png
		COMMAND cp icons/vhf-main_x128.png  ${OSX_ICON_DIR}/icon_64x64@2x.png
		COMMAND cp icons/vhf-main_x128.png  ${OSX_ICON_DIR}/icon_128x128.png
		COMMAND cp icons/vhf-main_x256.png  ${OSX_ICON_DIR}/icon_128x128@2x.png
		COMMAND cp icons/vhf-main_x256.png  ${OSX_ICON_DIR}/icon_256x256.png
		COMMAND cp icons/vhf-main_x512.png  ${OSX_ICON_DIR}/icon_256x256@2x.png
		COMMAND cp icons/vhf-main_x512.png  ${OSX_ICON_DIR}/icon_512x512.png
		COMMAND cp icons/vhf-main_x1024.png ${OSX_ICON_DIR}/icon_512x512@2x.png
		COMMAND iconutil --convert icns --output ${CMAKE_BINARY_DIR}/vhf-main.icns ${OSX_ICON_DIR}
		WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
		)

	set_target_properties(vhf-sim PROPERTIES
		INSTALL_RPATH @executable_path/../Frameworks

		MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"
		MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
		MACOSX_BUNDLE_LONG_VERSION_STRING "${PROJECT_VERSION}"
		MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
		MACOSX_BUNDLE_COPYRIGHT "Copyright (c) 2021 Mario Konrad. All rights reserved."
		MACOSX_BUNDLE_GUI_IDENTIFIER "vhf-sim"
		MACOSX_BUNDLE_ICON_FILE "vhf-main" # intentionally no file suffix
		MACOSX_BUNDLE_INFO_STRING "VHF Simulator"
		)
endif()

target_sources(vhf-sim
	PRIVATE
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
		icons/vhf-main.ico
		icons/vhf-main.bmp
		icons/vhf-main.xpm
		$<$<PLATFORM_ID:Darwin>:${CMAKE_BINARY_DIR}/vhf-main.icns>
		$<$<PLATFORM_ID:Darwin>:platform_osx.cpp>
		$<$<PLATFORM_ID:Windows>:platform_win.cpp>
		$<$<PLATFORM_ID:Linux>:platform_linux.cpp>
	)

if(APPLE)
	set_source_files_properties(${QM_FILES} PROPERTIES
		MACOSX_PACKAGE_LOCATION "translations"
		)

	set_source_files_properties(
			icons/vhf-main.ico
			icons/vhf-main.bmp
			icons/vhf-main.xpm
			${CMAKE_BINARY_DIR}/vhf-main.icns
		PROPERTIES
			MACOSX_PACKAGE_LOCATION Resources
		)
endif()

target_compile_options(vhf-sim
	PRIVATE
		-Wall
		-Wextra
		${Qt5Core_EXECUTABLE_COMPILE_FLAGS}
		${Qt5Gui_EXECUTABLE_COMPILE_FLAGS}
		${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}
		${Qt5Multimedia_EXECUTABLE_COMPILE_FLAGS}
	)

target_include_directories(vhf-sim
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
		${CMAKE_CURRENT_BINARY_DIR}/..
		${Qt5Core_INCLUDE_DIRS}
		${Qt5Gui_INCLUDE_DIRS}
		${Qt5Widgets_INCLUDE_DIRS}
		${Qt5Multimedia_INCLUDE_DIRS}
	)

target_link_libraries(vhf-sim
	PRIVATE
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
	BUNDLE  DESTINATION .
	COMPONENT applications
	)

if(APPLE)
	install(
		FILES
			${CMAKE_CURRENT_LIST_DIR}/icons/vhf-main.ico
			${CMAKE_CURRENT_LIST_DIR}/icons/vhf-main.bmp
			${CMAKE_CURRENT_LIST_DIR}/icons/vhf-main.xpm
			${CMAKE_BINARY_DIR}/vhf-main.icns
		DESTINATION "vhf-sim.app/Contents/Resources/"
		)
endif()

### language files
if(NOT APPLE)
	add_custom_command(TARGET vhf-sim POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E echo "Copy language files"
		COMMAND ${CMAKE_COMMAND} -E copy ${QM_FILES} ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/lang/
		)

	install(
		FILES ${QM_FILES}
		DESTINATION share/${PROJECT_NAME}/lang
		)
endif()

### simrad-rd68 files
add_custom_command(TARGET vhf-sim POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E echo "Copy Simrad-RD68 files to: ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/simrad-rd68/"
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/data/simrad-rd68 ${CMAKE_BINARY_DIR}/share/${PROJECT_NAME}/simrad-rd68/
	)
if(APPLE)
	install(
		DIRECTORY "${CMAKE_SOURCE_DIR}/data/simrad-rd68"
		DESTINATION "vhf-sim.app/Contents/Resources/"
		)
else()
	install(
		DIRECTORY "${CMAKE_SOURCE_DIR}/data/simrad-rd68"
		DESTINATION "share/${PROJECT_NAME}"
		)
endif()

### deployment
if(WIN32)
	find_program(DEPLOYQT_BIN windeployqt
		PATHS
			$ENV{QTHOME}
		PATH_SUFFIXES
			bin
		)
	if(NOT DEPLOYQT_BIN)
		message(FATAL_ERROR "NOT FOUND: windeployqt")
	endif()
	message(STATUS "Found: ${DEPLOYQT_BIN}")

	# install necessary libraries in binary folder
	add_custom_command(TARGET vhf-sim POST_BUILD
		COMMAND ${DEPLOYQT_BIN}
			--release
			--dir ${CMAKE_BINARY_DIR}/bin
			--verbose 0
			$<TARGET_FILE:vhf-sim>
		)

	# install necessary libraries
	add_custom_command(TARGET vhf-sim POST_BUILD
		COMMAND ${DEPLOYQT_BIN}
			--release
			--dir ${CMAKE_BINARY_DIR}/qt
			--verbose 0
			$<TARGET_FILE:vhf-sim>
		)
	install(DIRECTORY "${CMAKE_BINARY_DIR}/qt/" DESTINATION bin)
elseif(APPLE)
	find_program(DEPLOYQT_BIN macdeployqt
		PATHS
			$ENV{QTHOME}
		PATH_SUFFIXES
			bin
		)
	if(NOT DEPLOYQT_BIN)
		message(FATAL_ERROR "NOT FOUND: macdeployqt")
	endif()
	message(STATUS "Found: ${DEPLOYQT_BIN}")

	set(DEPLOY_OPTIONS [[vhf-sim.app -verbose=2]])
	set(TRANSLATION_DIR vhf-sim.app/Contents/translations)

	configure_file(
		${CMAKE_SOURCE_DIR}/osx-deploy.cmake.in
		${CMAKE_BINARY_DIR}/osx-deploy.cmake
		@ONLY)

	install(SCRIPT ${CMAKE_BINARY_DIR}/osx-deploy.cmake)
elseif(UNIX)
	# intentionally left blank
endif()

