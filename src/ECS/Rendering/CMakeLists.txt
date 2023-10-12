# CMakeList.txt : CMake project for HellloGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(HelloGL 
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/Material.cpp
	${CMAKE_CURRENT_LIST_DIR}/Renderer.cpp
	${CMAKE_CURRENT_LIST_DIR}/RenderTexture.cpp

	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/Renderer.h
	${CMAKE_CURRENT_LIST_DIR}/Material.h
	${CMAKE_CURRENT_LIST_DIR}/RenderTexture.h
	)
	
# TODO: Add tests and install targets if needed.
