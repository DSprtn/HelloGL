cmake_minimum_required (VERSION 3.8)

target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(Rendering)

target_sources(HelloGL 
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/Engine.cpp
	${CMAKE_CURRENT_LIST_DIR}/Entity.cpp
	${CMAKE_CURRENT_LIST_DIR}/World.cpp
	${CMAKE_CURRENT_LIST_DIR}/Input.cpp
	${CMAKE_CURRENT_LIST_DIR}/Timer.cpp

	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/Engine.h
	${CMAKE_CURRENT_LIST_DIR}/Entity.h
	${CMAKE_CURRENT_LIST_DIR}/Component.h
	${CMAKE_CURRENT_LIST_DIR}/World.h
	${CMAKE_CURRENT_LIST_DIR}/Input.h
	${CMAKE_CURRENT_LIST_DIR}/Timer.h
)