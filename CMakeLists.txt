cmake_minimum_required(VERSION 3.25)

project(GAME VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS off)

file(
  GLOB_RECURSE
  SOURCES
  CONFIGURE_DEPENDS
  "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  ${SOURCES}
)

target_include_directories(
  ${PROJECT_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(
  ${PROJECT_NAME}
  SDL2
  SDL2main
)