cmake_minimum_required(VERSION 3.2)	 

project(stereo)			# Название проекта

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_EXE 
	sources/stereo.cpp
	sources/ComputeDistance.cpp
	sources/SectionsDistanceTest.cpp
	sources/TrivialTests.cpp
)

add_executable(stereo ${SOURCE_EXE})

