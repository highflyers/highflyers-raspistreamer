cmake_minimum_required(VERSION 2.8.9)

project(raspistreamer)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")

find_package(PkgConfig)

add_subdirectory(src)