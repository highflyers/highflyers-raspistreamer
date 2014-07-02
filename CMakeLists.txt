cmake_minimum_required(VERSION 2.8.9)

project(raspistreamer)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic -std=c++11")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-as-needed")

execute_process(COMMAND xbuild /p:OutputPath=../../../build ../highflyers-protocol/src/highflyers-protocol-generator/highflyers-protocol-generator.csproj)
execute_process(
	COMMAND 
		cp
		../highflyers-protocol/src/highflyers-protocol-c/frame_parser_helper.h
		../highflyers-protocol/src/highflyers-protocol-c/frame_parser_helper.c
		../highflyers-protocol/src/highflyers-protocol-c/parser.h
		../highflyers-protocol/src/highflyers-protocol-c/parser.c
		../highflyers-protocol/src/highflyers-protocol-c/types.h 
		../src/protocol/
	)
execute_process(
	COMMAND 
		./HFCsGenerator.exe
		../highflyers-protocol/descriptions/imav2014.hfproto
		../src/protocol/frames
		../src/protocol/frame_builder
		--language=C
)

find_package(PkgConfig)

add_subdirectory(src)