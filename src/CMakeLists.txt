# CMakeList.txt : CMake project for HellloGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

target_sources(HelloGL 
	PRIVATE
	#${CMAKE_CURRENT_LIST_DIR}/glad.c
	${CMAKE_CURRENT_LIST_DIR}/Camera.cpp
	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/Camera.h
	)

add_subdirectory(Resources)
add_subdirectory(Shaders)
add_subdirectory(Core)

target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/)
target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Resources/)