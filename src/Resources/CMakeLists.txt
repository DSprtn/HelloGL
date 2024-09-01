cmake_minimum_required (VERSION 3.8)

target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(HelloGL 
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/AssetCache.cpp
	${CMAKE_CURRENT_LIST_DIR}/Model.cpp
	${CMAKE_CURRENT_LIST_DIR}/Mesh.cpp

	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/AssetCache.h
	${CMAKE_CURRENT_LIST_DIR}/Model.h
	${CMAKE_CURRENT_LIST_DIR}/Mesh.h
)