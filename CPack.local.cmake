include(InstallRequiredSystemLibraries)

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_NAME "${PROJECT_NAME}")
set(CPACK_PACKAGE_VENDOR "Mario Konrad")
set(CPACK_PACKAGE_CONTACT "Mario Konrad <mario.konrad@gmx.net>")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://mariokonrad.github.io/vhf-sim/")
set(CPACK_PACAKGE_DESCRIPTION "Martitime Radio Simulator")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Martitime Radio Simulator for Practicing")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_MONOLITHIC_INSTALL 1)
set(CPACK_PACKAGE_EXECUTABLES "vhf-sim" "vhf-sim")
set(CPACK_STRIP_FILES true)

if(WIN32)
	set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/src/vhf/icons\\\\vhf-main.bmp")
	set(CPACK_NSIS_DISPLAY_NAME "${PROJECT_NAME} ${PROJECT_VERSION}")
	set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\vhf-sim.exe")
	set(CPACK_NSIS_CONTACT "${CPACK_PACKAGE_CONTACT}")
	set(CPACK_NSIS_MUI_ICON "${CMAKE_SOURCE_DIR}/src/vhf/icons\\\\vhf-main.ico")
	#set(CPACK_NSIS_MODIFY_PATH ON)
	#set(CPACK_NSIS_URL_INFO_ABOUT "")
	#set(CPACK_NSIS_HELP_LINK "")
	message(STATUS "Packaging available: NSIS")
elseif(APPLE)
	set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/src/vhf/icons/vhf-main.bmp")
	set(CPACK_BUNDLE_NAME "${CPACK_PACKAGE_NAME}")
	set(CPACK_BUNDLE_ICON "${CMAKE_SOURCE_DIR}/src/vhf/icons/vhf-main.ico")

	if(DEFINED PACKAGE_SYSTEM_DISTRIBUTION)
		message(STATUS "System Distribution: ${PACKAGE_SYSTEM_DISTRIBUTION}")
		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${PACKAGE_SYSTEM_DISTRIBUTION}")
	endif()

	message(STATUS "Packaging available: DragNDrop")
elseif(UNIX)
	set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/src/vhf/icons/vhf-main.bmp")
	set(CPACK_PACKAGING_INSTALL_PREFIX "/usr/local/")
	set(CPACK_DEBIAN_PACKAGE_NAME ${CPACK_DEBIAN_PACKAGE_NAME})
	set(CPACK_DEBIAN_PACKAGE_MAINTAINER ${CPACK_PACKAGE_CONTACT})
	set(CPACK_DEBIAN_PACKAGE_DESCRIPTION ${CPACK_PACAKGE_DESCRIPTION})
	set(CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5core5a, libqt5widgets5, libqt5gui5, libqt5multimedia5, libqt5network5")
	set(CPACK_DEBIAN_PACKAGE_SECTION "misc")
	if(CMAKE_SIZEOF_VOID_P MATCHES "8")
		set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "amd64")
	else()
		set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "i386")
	endif()

	if(DEFINED PACKAGE_SYSTEM_DISTRIBUTION)
		message(STATUS "System Distribution: ${PACKAGE_SYSTEM_DISTRIBUTION}")
		set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${PACKAGE_SYSTEM_DISTRIBUTION}")
	endif()
	message(STATUS "Packaging available: DEB")
endif()

include(CPack)
