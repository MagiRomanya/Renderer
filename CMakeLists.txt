CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(Renderer)

set(SOURCE_FILES
  src/glad.c
  src/shader.cpp
  src/main.cpp
  src/object.cpp
  src/mesh.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# cempile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
  ${GLFW_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
  glfw
  OpenGL::GL)
