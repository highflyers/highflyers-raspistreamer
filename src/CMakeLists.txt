add_subdirectory(udpstreamer)
add_subdirectory(protocol)

IF (NOT DEFINED TEST_APP)
	SET(TEST_APP 0)
ENDIF()
ADD_DEFINITIONS(-DTEST_APP=${TEST_APP})

include_directories(
	udpstreamer/include
	protocol
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
)

add_executable(
	test-data-generator
	test-data-generator.cpp
)

target_link_libraries(
	test-data-generator
	raspistream-protocol-lib
)