add_subdirectory(udpstreamer)
include_directories(udpstreamer/include)

add_executable(
	raspistreamer 
	main.cpp)

target_link_libraries(raspistreamer udpstreamer)
