﻿# CMakeList.txt : CMake project for HellloGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Technicolor.frag
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Technicolor.vert
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Light.frag
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Light.vert
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
	
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/UberShader.frag
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
	
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/UberShader.vert
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/common.glsl
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/MaterialCommon.glsl
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/LightData.glsl
    ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)	
	
target_sources(HelloGL 
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/Technicolor.vert
	${CMAKE_CURRENT_LIST_DIR}/Technicolor.frag
	${CMAKE_CURRENT_LIST_DIR}/Light.vert
	${CMAKE_CURRENT_LIST_DIR}/Light.frag
	${CMAKE_CURRENT_LIST_DIR}/UberShader.vert
	${CMAKE_CURRENT_LIST_DIR}/UberShader.frag
	${CMAKE_CURRENT_LIST_DIR}/common.glsl
	${CMAKE_CURRENT_LIST_DIR}/MaterialCommon.glsl
	${CMAKE_CURRENT_LIST_DIR}/LightData.glsl
)