# CMakeList.txt : CMake project for HellloGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Technicolor.frag
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
	
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Technicolor.vert
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
	
	
target_sources(HelloGL 
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/Technicolor.vert
	)
	
target_sources(HelloGL 
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/Technicolor.frag
	)