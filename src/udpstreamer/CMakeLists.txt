pkg_check_modules(GSTREAMERMM REQUIRED gstreamermm-1.0)
include_directories(${GSTREAMERMM_INCLUDE_DIRS})

IF (NOT DEFINED TEST_APP)
	SET(TEST_APP 0)
ENDIF()
ADD_DEFINITIONS(-DTEST_APP=${TEST_APP})

add_library(
	udpstreamer
	raspividwrapper.cpp
	gstudpserver.cpp
)

include_directories(include/udpstreamer)

target_link_libraries(udpstreamer ${GSTREAMERMM_LIBRARIES})
