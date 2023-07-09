cmake_minimum_required (VERSION 3.8)
include(CMakePrintHelpers)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project ("HelloGL")

add_executable (HelloGL "HelloGL.cpp" "HelloGL.h")

list(APPEND CMAKE_PREFIX_PATH "out/")

find_package(ASSIMP CONFIG REQUIRED HINTS "out")
find_package(GLFW3 CONFIG REQUIRED HINTS "out")
find_package(GLM CONFIG REQUIRED HINTS "out")
find_package(GLAD CONFIG REQUIRED HINTS "out")

target_link_libraries(HelloGL PRIVATE
					 assimp::assimp 
					 glfw
					 glm::glm
					 glad::glad)


add_subdirectory(src)
add_subdirectory(Shaders)
add_subdirectory(include/imgui)

file(COPY assets/ DESTINATION assets)
file(COPY Shaders/ DESTINATION shaders)


target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_include_directories(HelloGL PUBLIC ${OPENGL_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})
target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/)
target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Resources/)
target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/)