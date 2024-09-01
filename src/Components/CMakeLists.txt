cmake_minimum_required (VERSION 3.8)

target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(HelloGL 
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/Transform.cpp
	${CMAKE_CURRENT_LIST_DIR}/MeshRenderer.cpp
	${CMAKE_CURRENT_LIST_DIR}/Light.cpp
	${CMAKE_CURRENT_LIST_DIR}/Camera.cpp

	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/Transform.h
	${CMAKE_CURRENT_LIST_DIR}/MeshRenderer.h
	${CMAKE_CURRENT_LIST_DIR}/Light.h
	${CMAKE_CURRENT_LIST_DIR}/Camera.h
)