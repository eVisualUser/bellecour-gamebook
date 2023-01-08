cmake -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_RUNTIME_OUTPUT_DIRECTORY=D:/Code/bellecour-gamebook/gamebook -DCMAKE_TOOLCHAIN_FILE=D:\Tools\emsdk\upstream\emscripten\cmake\Modules\Platform\Emscripten.cmake -DCMAKE_CROSSCOMPILING_EMULATOR="D:/Tools/emsdk/node/14.18.2_64bit/bin/node.exe;--experimental-wasm-bulk-memory;--experimental-wasm-threads" D:/Code/bellecour-gamebook/gamebook

ninja
ninja -v make_html
