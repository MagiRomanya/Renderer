project(imgui)

file(GLOB_RECURSE SOURCE_FILES *.cpp)

add_library(${PROJECT_NAME} ${SOURCE_FILES})

message(${CMAKE_CURRENT_SOURCE_DIR}/../include/imgui/)
message("hello")
target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/../include/imgui/
  )
