# package: debian

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

set(CPACK_GENERATOR "DEB")

if(DEFINED PACKAGE_SYSTEM_DISTRIBUTION)
	message(STATUS "System Distribution: ${PACKAGE_SYSTEM_DISTRIBUTION}")
	set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${PACKAGE_SYSTEM_DISTRIBUTION}")
endif()