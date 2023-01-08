cmake_minimum_required(VERSION 3.10)

set(PROJECT_VERSION 0.1.0)

project(client VERSION ${PROJECT_VERSION})

set(WORKING_DIRECTORY "cmake")

option(client -G)

file(GLOB sources "source/*.cpp" "source/filesystem/*.cpp" "source/logicstr/*.cpp")
message("Sources fetched successfully")

add_executable(client ${sources})

message("Executable copied to build directory")
