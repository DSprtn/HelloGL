# CMakeList.txt : CMake project for HellloGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

target_sources(HelloGL 
	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/Shader.h
	)

# TODO: Add tests and install targets if needed.
