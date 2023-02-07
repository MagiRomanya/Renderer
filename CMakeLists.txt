CMAKE_MINIMUM_REQUIRED(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)

project(renderer)

set(SOURCE_FILES
  src/glad.c
  src/shader.cpp
  src/object.cpp
  src/camera.cpp
  src/mesh.cpp
  # src/main2.cpp
  src/renderer.cpp
)

file(GLOB_RECURSE SOURCE_FILES
  ${SOURCE_FILES}
  external_src/*.cpp
)

add_library(${PROJECT_NAME} ${SOURCE_FILES})
# add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_BUILD_TYPE Debug)

find_package(glfw3 3.3 REQUIRED)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/imgui
  ${GLM_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
  ${ASSIMP_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  glfw
  OpenGL::GL
  ${ASSIMP_LIBRARIES}
)
