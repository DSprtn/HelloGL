# CMakeList.txt : CMake project for HellloGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

target_sources(HelloGL 
	PRIVATE
	${CMAKE_CURRENT_LIST_DIR}/imgui.cpp
	${CMAKE_CURRENT_LIST_DIR}/imgui_draw.cpp
	${CMAKE_CURRENT_LIST_DIR}/imgui_impl_glfw.cpp
	${CMAKE_CURRENT_LIST_DIR}/imgui_impl_opengl3.cpp
	${CMAKE_CURRENT_LIST_DIR}/imgui_widgets.cpp
	${CMAKE_CURRENT_LIST_DIR}/imgui_tables.cpp
	PUBLIC
	${CMAKE_CURRENT_LIST_DIR}/imgui.h
	${CMAKE_CURRENT_LIST_DIR}/imgui_impl_glfw.h
	${CMAKE_CURRENT_LIST_DIR}/imgui_impl_opengl3.h

	)


# TODO: Add tests and install targets if needed.
