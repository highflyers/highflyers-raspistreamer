add_executable(RaspiStreamer
	raspividwrapper.cpp
	gstudpserver.cpp
	main.cpp
)

target_link_libraries(RaspiStreamer ${PKG_LIBRARIES})