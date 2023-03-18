project(imgui)

file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

message(${CMAKE_CURRENT_SOURCE_DIR}/../include/imgui/)

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/imgui/
  ${CMAKE_CURRENT_SOURCE_DIR}/../external/glfw/include/
  )
