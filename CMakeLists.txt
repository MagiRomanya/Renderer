CMAKE_MINIMUM_REQUIRED(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
# compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

project(renderer VERSION 0.1)

set(SOURCE_FILES
  src/glad.c
  src/shader.cpp
  src/object.cpp
  src/camera.cpp
  src/mesh.cpp
  src/edge.cpp
  src/renderer.cpp
  src/object_manager.cpp
  src/intersection.cpp
  src/little_axis.cpp
)

set(SHADER_PATH "${PROJECT_SOURCE_DIR}/shaders")
set(TEXTURE_PATH "${PROJECT_SOURCE_DIR}/img")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/shader_path.h.in ${CMAKE_CURRENT_SOURCE_DIR}/../include/shader_path.h)

# Creates a library if added as a subdirectory or compiles it as
# an executable if run as a top level CMakeLists.txt
if(PROJECT_IS_TOP_LEVEL)
  message("${PROJECT_NAME} compiled as a standalone")
  add_executable(${PROJECT_NAME} ${SOURCE_FILES} src/main.cpp)
else()
  message("${PROJECT_NAME} added as subdirectory")
  add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
endif()


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/imgui)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glfw)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/glm)

set(BUILD_SHARED_LIBS ON)
set(ASSIMP_BUILD_ZLIB ON)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)
set(ASSIMP_NO_EXPORT ON)
set(ASSIMP_WARNINGS_AS_ERRORS OFF)
message("---------------- ASSIMP ---------------")
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/assimp)
# find_package(assimp REQUIRED)
message("---------------------------------------")


set(CMAKE_BUILD_TYPE Debug)

set(OpenGL_GL_PREFERENCE GLVND)
find_package(OpenGL REQUIRED)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/external/glfw/include/
  ${CMAKE_CURRENT_SOURCE_DIR}/external/glm/
  ${CMAKE_CURRENT_SOURCE_DIR}/external/assimp/include/
  )

target_link_libraries(${PROJECT_NAME} PUBLIC
  imgui
  glfw
  OpenGL::GL
  assimp
  )
