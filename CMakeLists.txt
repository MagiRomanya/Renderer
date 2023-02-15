CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
# compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project(renderer)

set(SOURCE_FILES
  src/glad.c
  src/shader.cpp
  src/object.cpp
  src/camera.cpp
  src/mesh.cpp
  src/main2.cpp
  src/renderer.cpp
  src/intersection.cpp
)
# Creates a library if added as a subdirectory or compiles it as
# an executable if run as a top level CMakeLists.txt
if(PROJECT_IS_TOP_LEVEL)
  message("${PROJECT_NAME} compiled as a standalone")

  # set(SOURCE_FILES ${SOURCE_FILES} src/main2.cpp)
  add_executable(${PROJECT_NAME} ${SOURCE_FILES})
else()
  message("${PROJECT_NAME} added as subdirectory")
  add_library(${PROJECT_NAME} ${SOURCE_FILES})
endif()


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/imgui)


set(CMAKE_BUILD_TYPE Debug)

find_package(glfw3 3.3 REQUIRED)
set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
  ${GLM_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
  ${ASSIMP_INCLUDE_DIR}
  )

target_link_libraries(${PROJECT_NAME} PUBLIC
  imgui
  glfw
  OpenGL::GL
  ${ASSIMP_LIBRARIES}
  )
