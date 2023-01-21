cmake_minimum_required(VERSION 3.10)

set(PROJECT_VERSION 0.1.0)

project(client VERSION ${PROJECT_VERSION})

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB sources "source/*.cpp" "source/filesystem/*.cpp" "source/logicstr/*.cpp" "source/io/*.cpp" "source/render/*.cpp" "source/pages/*.cpp" "source/core/*.cpp" "source/debug/*.cpp" "source/noise/*.cpp" "source/save/*.cpp" "source/splashframe/*.cpp")

message("\nCode sources found: ${sources}\n")

add_executable(client ${sources})

if (EMSCRIPTEN)
    file(GLOB assets RELATIVE "${CMAKE_CURRENT_LIST_DIR}/web" "web/pages/*.toml" "web/*.toml")

    message("Preloaded files list: [${assets}]\n")
    string(REPLACE ";" " --preload-file " assets "${assets}")

    set(emcc_flags "-s ALLOW_MEMORY_GROWTH=1 --preload-file ${assets}")

    set_target_properties(client PROPERTIES LINK_FLAGS ${emcc_flags})
endif ()
