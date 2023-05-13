cmake_minimum_required (VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project ("OpenGL")

add_executable (HelloGL "HelloGL.cpp" "HelloGL.h")

add_subdirectory(src)
add_subdirectory(Shaders)
add_subdirectory(include/imgui)

file(COPY img/ DESTINATION img)

include_directories( ${OPENGL_INCLUDE_DIRS} ${GLFW3_INCLUDE_DIRS})

target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/glm/glm/)
target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/)

target_link_libraries(HelloGL "opengl32.lib")
target_link_libraries(HelloGL ${CMAKE_CURRENT_SOURCE_DIR}/include/lib/glfw3.lib)