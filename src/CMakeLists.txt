add_subdirectory(udpstreamer)

pkg_check_modules(GLIBMM REQUIRED glibmm-2.4)

include_directories(
	udpstreamer/include
	${GLIBMM_INCLUDE_DIRS}
)
link_directories(${GLIBMM_LIBRARY_DIRS})

add_executable(
	raspistreamer 
	main.cpp)

target_link_libraries(raspistreamer udpstreamer)
