add_subdirectory(udpstreamer)
add_subdirectory(protocol)

pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)

IF (NOT DEFINED TEST_APP)
	SET(TEST_APP 0)
ENDIF()
ADD_DEFINITIONS(-DTEST_APP=${TEST_APP})

include_directories(
	udpstreamer/include
	protocol
	${GLIBMM_INCLUDE_DIRS}
)

add_executable(
	raspistreamer
	streamer.cpp
	config.cpp
	main.cpp
)

target_link_libraries(
	raspistreamer
	udpstreamer
	raspistream-protocol-lib
	${GLIBMM_LIBRARIES}
)
