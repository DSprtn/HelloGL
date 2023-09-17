cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project ("HelloGL")

add_executable (HelloGL "src/HelloGL.cpp" "src/HelloGL.h")

find_package(ASSIMP CONFIG REQUIRED)
find_package(GLFW3 CONFIG REQUIRED)
find_package(GLM CONFIG REQUIRED)
find_package(GLAD CONFIG REQUIRED)

target_link_libraries(HelloGL PRIVATE
					 assimp::assimp 
					 glfw
					 glm::glm
					 glad::glad)

add_subdirectory(src)
add_subdirectory(Shaders)
add_subdirectory(include/imgui)


# Copy files on build 

add_custom_command(TARGET HelloGL PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:HelloGL>/assets)

add_custom_command(TARGET HelloGL PRE_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:HelloGL>/shaders)

# Copy shaders on config for hot reloading

foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG )
    file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR}/${OUTPUTCONFIG})
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Shaders/)
target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/Resources/)
target_include_directories(HelloGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/imgui/)
