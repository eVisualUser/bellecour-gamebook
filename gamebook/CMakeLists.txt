cmake_minimum_required(VERSION 3.10)

set(PROJECT_VERSION 0.1.0)

project(client VERSION ${PROJECT_VERSION})

option(client -G)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 17)

file(GLOB sources "source/*.cpp" "source/filesystem/*.cpp" "source/logicstr/*.cpp" "source/web/*.cpp")

message("\nCode sources found: ${sources}\n")

add_executable(client ${sources})

if (EMSCRIPTEN)
    file(GLOB assets RELATIVE "${CMAKE_CURRENT_LIST_DIR}/web" "web/assets/*.toml")

    message("Preloaded files list: [${assets}]\n")
    string(REPLACE ";" " --preload-file " assets "${assets}")

    set(emcc_flags "-s ALLOW_MEMORY_GROWTH=1 --preload-file ${assets}")

    set_target_properties(client PROPERTIES LINK_FLAGS ${emcc_flags})
endif ()
