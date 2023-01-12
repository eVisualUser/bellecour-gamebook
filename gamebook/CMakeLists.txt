cmake_minimum_required(VERSION 3.10)

project(client)

option(client -G)

set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD 23)

file(GLOB sources "source/*.cpp" "source/filesystem/*.cpp" "source/logicstr/*.cpp" "source/io/*.cpp" "source/render/*.cpp")

message("\nCode sources found: ${sources}\n")

add_executable(client ${sources})

set(MINOR_CACHE_FILE "version_minor.txt")
set(MAJOR_CACHE_FILE "version_major.txt")

set(MAJOR_VERSION "0")

IF(EXISTS ${MAJOR_CACHE_FILE})
    file(READ ${MAJOR_CACHE_FILE} MAJOR_VERSION)
ENDIF()

IF(EXISTS ${MINOR_CACHE_FILE})
    file(READ ${MINOR_CACHE_FILE} MINOR_INCREMENTED_VALUE)
    math(EXPR MINOR_INCREMENTED_VALUE "${MINOR_INCREMENTED_VALUE}+1")
ELSE()
    set(MINOR_INCREMENTED_VALUE "1")
ENDIF()

file(WRITE ${MINOR_CACHE_FILE} "${MINOR_INCREMENTED_VALUE}")

set_target_properties(client PROPERTIES OUTPUT_NAME "clientV${MAJOR_VERSION}.${MINOR_INCREMENTED_VALUE}")

if (EMSCRIPTEN)
    file(GLOB assets RELATIVE "${CMAKE_CURRENT_LIST_DIR}/web" "web/assets/*.toml")

    message("Preloaded files list: [${assets}]\n")
    string(REPLACE ";" " --preload-file " assets "${assets}")

    set(emcc_flags "-s ALLOW_MEMORY_GROWTH=1 --preload-file ${assets}")

    set_target_properties(client PROPERTIES LINK_FLAGS ${emcc_flags})
endif ()
