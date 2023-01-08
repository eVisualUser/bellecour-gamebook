cmake_minimum_required(VERSION 3.10)

set(CMAKE_GENERATOR "Unix Makefiles")

set(PROJECT_VERSION 0.1.0)

project(client VERSION ${PROJECT_VERSION})

set(WORKING_DIRECTORY "cmake")

option(client -G)

file(GLOB sources "source/*.cpp" "source/filesystem/*.cpp" "source/logicstr/*.cpp")
message("Sources fetched successfully")

add_executable(client ${sources})

message("Executable copied to build directory")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s SIDE_MODULE=1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -s MODULARIZE=1")

add_custom_command(
  OUTPUT index.html
  COMMAND emcc index.wasm -o index.html
  DEPENDS index.wasm
  COMMENT "Generating HTML file"
)

add_custom_target(make_html DEPENDS index.html)
