# CMakeList.txt : CMake project for HellloGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(HelloGL 
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/Entity.cpp
	${CMAKE_CURRENT_LIST_DIR}/World.cpp
	${CMAKE_CURRENT_LIST_DIR}/Renderer.cpp

	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/Entity.h
	${CMAKE_CURRENT_LIST_DIR}/Component.h
	${CMAKE_CURRENT_LIST_DIR}/World.h
	${CMAKE_CURRENT_LIST_DIR}/Renderer.h
	)
	
# TODO: Add tests and install targets if needed.
