cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project ("OpenGL")

set(INCLUDE_PATH CACHE FILEPATH "Include dir path")

set(GLAD_PATH "" CACHE FILEPATH "Path to GLAD")
set(GLFW_PATH "" CACHE FILEPATH "Path to GLFW")
set(KHR_PATH "" CACHE FILEPATH "Path to KHR")
set(GLFW_LIB_PATH "" CACHE FILEPATH "Path to GLFW Lib")
set(STB_PATH "" CACHE FILEPATH "Path to STB")
set(GLM_PATH "" CACHE FILEPATH "Path to GLM")

cmake_path(GET KHR_PATH PARENT_PATH LIB_DIR)

add_executable (HelloGL "HelloGL.cpp" "HelloGL.h")

add_subdirectory(src)
add_subdirectory(Shaders)

file(COPY img/ DESTINATION img)

include_directories( ${OPENGL_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})

target_include_directories(HelloGL PUBLIC ${INCLUDE_PATH})
target_include_directories(HelloGL PUBLIC ${GLM_PATH})
target_include_directories(HelloGL PUBLIC ${GLAD_PATH})
target_include_directories(HelloGL PUBLIC ${KHR_PATH})
target_include_directories(HelloGL PUBLIC ${GLFW_PATH})
target_include_directories(HelloGL PUBLIC ${STB_PATH})
target_include_directories(HelloGL PUBLIC ${LIB_DIR})

target_link_libraries(HelloGL "opengl32.lib")
target_link_libraries(HelloGL ${GLFW_LIB_PATH})


