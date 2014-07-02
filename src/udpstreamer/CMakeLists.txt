add_library(
	udpstreamer
	raspividwrapper.cpp
	gstudpserver.cpp
)

include_directories(include/udpstreamer)

target_link_libraries(udpstreamer ${PKG_LIBRARIES})
