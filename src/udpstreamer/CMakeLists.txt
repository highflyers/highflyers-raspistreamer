pkg_check_modules(GSTREAMERMM REQUIRED gstreamermm-1.0)
include_directories(${GSTREAMERMM_INCLUDE_DIRS})

ADD_DEFINITIONS(-D_MYDEFINE=${_MYDEFINE})

add_library(
	udpstreamer
	raspividwrapper.cpp
	gstudpserver.cpp
)

include_directories(include/udpstreamer)

target_link_libraries(udpstreamer ${GSTREAMERMM_LIBRARIES})
