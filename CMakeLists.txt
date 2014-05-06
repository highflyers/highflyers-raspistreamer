cmake_minimum_required(VERSION 2.8.9)

project(RaspiStreamer)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")
SET(PROJECT_SOURCE_DIR "${RaspiStreamer_SOURCE_DIR}/src")

include_directories(${PROJECT_SOURCE_DIR}/include)

find_package(PkgConfig)
pkg_check_modules(PKG REQUIRED glibmm-2.4 gstreamermm-1.0)
include_directories(${PKG_INCLUDE_DIRS})
link_directories(${PKG_LIBRARY_DIRS})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

add_subdirectory(src)