cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project ("OpenGL")


set(GLAD_PATH "" CACHE FILEPATH "Path to GLAD")
set(GLFW_PATH "" CACHE FILEPATH "Path to GLFW")
set(KHR_PATH "" CACHE FILEPATH "Path to KHR")
set(GLFW_LIB_PATH "" CACHE FILEPATH "Path to GLFW Lib")

cmake_path(GET KHR_PATH PARENT_PATH LIB_DIR)

add_executable (HelloGL "HelloGL.cpp" "HelloGL.h")

add_subdirectory(src)

include_directories( ${OPENGL_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})

target_include_directories(HelloGL PUBLIC ${GLAD_PATH})
target_include_directories(HelloGL PUBLIC ${GLFW_PATH})
target_include_directories(HelloGL PUBLIC ${KHR_PATH})
target_include_directories(HelloGL PUBLIC ${LIB_DIR})
 

target_link_libraries(HelloGL "opengl32.lib")
target_link_libraries(HelloGL ${GLFW_LIB_PATH})