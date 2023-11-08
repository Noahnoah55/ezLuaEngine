cmake_minimum_required(VERSION 3.27) #TODO: TEST OLDER CMAKE
project(EZLUAENGINE)

set(CMAKE_CXX_STANDARD 20)
set(EMSCRIPTEN_LINK_PRELOAD_FLAGS
"SHELL:--preload-file ${CMAKE_SOURCE_DIR}/examples/opengl-simple@/"
"SHELL:--preload-file ${CMAKE_SOURCE_DIR}/src/shaders@/shaders/"
"SHELL:--preload-file ${CMAKE_SOURCE_DIR}/default-assets@/")
set(EMSCRIPTEN_LINK_LIBRARY_FLAGS
-sUSE_SDL=2
-sMIN_WEBGL_VERSION=2 
-sMAX_WEBGL_VERSION=2)
set(EMSCRIPTEN_EXCEPTION_FLAG -fwasm-exceptions)
set(EMSCRIPTEN_SHELL_FLAG --shell-file ${CMAKE_SOURCE_DIR}/src/shell.html)

add_library(lua
    lua/lapi.c
    lua/lauxlib.c
    lua/lbaselib.c
    lua/lcode.c
    lua/lcorolib.c
    lua/lctype.c
    lua/ldblib.c
    lua/ldebug.c
    lua/ldo.c
    lua/ldump.c
    lua/lfunc.c
    lua/lgc.c
    lua/linit.c
    lua/liolib.c
    lua/llex.c
    lua/lmathlib.c
    lua/lmem.c
    lua/loadlib.c
    lua/lobject.c
    lua/lopcodes.c
    lua/loslib.c
    lua/lparser.c
    lua/lstate.c
    lua/lstring.c
    lua/lstrlib.c
    lua/ltable.c
    lua/ltablib.c
    lua/ltests.c
    lua/ltm.c
    lua/lundump.c
    lua/lutf8lib.c
    lua/lvm.c
    lua/lzio.c
)

add_executable(engine 
    src/main.cpp
    src/engine.cpp
    src/fetch.cpp
    src/gfx.cpp
    src/input.cpp
    src/shader.cpp
    src/stbi.cpp
)

target_include_directories(engine PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/sol2/include
    ${PROJECT_SOURCE_DIR}/lua
)

target_link_libraries(engine PUBLIC lua)

if(EMSCRIPTEN)
    message("Using emscripten!")
    target_compile_options(lua PUBLIC ${EMSCRIPTEN_EXCEPTION_FLAG})
    target_compile_options(engine PUBLIC ${EMSCRIPTEN_EXCEPTION_FLAG})
    target_link_options(engine PUBLIC
        --use-preload-plugins --emrun
        ${EMSCRIPTEN_EXCEPTION_FLAG}
        ${EMSCRIPTEN_LINK_PRELOAD_FLAGS}
        ${EMSCRIPTEN_LINK_LIBRARY_FLAGS}
        ${EMSCRIPTEN_SHELL_FLAG}
    )
    set_target_properties(engine PROPERTIES SUFFIX .html)
    configure_file(src/favicon.ico favicon.ico COPYONLY)
else()
    message(FATAL_ERROR "Emscripten is required, run with emcmake") # TODO: Add native build support
endif()