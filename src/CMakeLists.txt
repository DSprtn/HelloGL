# CMakeList.txt : CMake project for HellloGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

target_sources(HelloGL 
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/glad.c
	${CMAKE_CURRENT_LIST_DIR}/Camera.cpp
	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/Camera.h
	)
	
target_sources(HelloGL PRIVATE "src/Shaders/Shader.h")

# TODO: Add tests and install targets if needed.
