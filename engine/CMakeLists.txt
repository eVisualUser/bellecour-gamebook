cmake_minimum_required(VERSION 3.10)

project(engine)

include("deps.cmake")

set(CMAKE_CXX_STANDARD 23)

include_directories("include")

file(GLOB sources "source/*.cpp")

add_library(engine STATIC ${sources} ${headers})
target_link_libraries(engine toml)
