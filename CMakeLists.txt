cmake_minimum_required(VERSION 3.27) #TODO: TEST OLDER CMAKE
project(EZLUAENGINE)

set(CMAKE_CXX_STANDARD 20)

set(EMSCRIPTEN_LINK_PRELOAD_FLAGS
"SHELL:--preload-file ${CMAKE_SOURCE_DIR}/src/shaders@/shaders/"
"SHELL:--preload-file ${CMAKE_SOURCE_DIR}/default-assets@/")
set(EMSCRIPTEN_USE_FLAGS -sUSE_SDL=2 -sUSE_SDL_MIXER=2)
set(EMSCRIPTEN_EXCEPTION_FLAG -fwasm-exceptions)
set(EMSCRIPTEN_SHELL_FLAG --shell-file ${CMAKE_SOURCE_DIR}/src/shell.html)
set(EMSCRIPTEN_LINK_OPENGL_FLAGS
-sMIN_WEBGL_VERSION=2
-sMAX_WEBGL_VERSION=2
-sFETCH)

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
    src/gfx.cpp
    src/input.cpp
    src/shader.cpp
    src/stbi.cpp
    src/fetch.cpp
    src/sound.cpp
)

target_include_directories(engine PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/sol2/include
    ${PROJECT_SOURCE_DIR}/lua
    ${PROJECT_SOURCE_DIR}/spdlog/include
)

add_subdirectory(spdlog)
target_link_libraries(engine PUBLIC lua)
target_link_libraries(engine PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

target_compile_definitions(engine PUBLIC "$<$<CONFIG:DEBUG>:DEBUG>")

if(EMSCRIPTEN)
    message("Using emscripten!")
    target_compile_options(lua PUBLIC ${EMSCRIPTEN_EXCEPTION_FLAG} -O3)
    target_compile_options(engine PUBLIC ${EMSCRIPTEN_EXCEPTION_FLAG} ${EMSCRIPTEN_USE_FLAGS} -O3)
    target_link_options(engine PUBLIC
        --use-preload-plugins 
        #--emrun
        ${EMSCRIPTEN_LINK_OPENGL_FLAGS}
        ${EMSCRIPTEN_EXCEPTION_FLAG}
        ${EMSCRIPTEN_LINK_PRELOAD_FLAGS}
        ${EMSCRIPTEN_USE_FLAGS}
        ${EMSCRIPTEN_SHELL_FLAG}
    )
    set_target_properties(engine PROPERTIES SUFFIX .html)
    configure_file(src/favicon.ico favicon.ico COPYONLY)
    configure_file(default-assets/ezle.png ezle.png COPYONLY)
else()
    message(FATAL_ERROR "Emscripten is required, run with emcmake") # TODO: Add native build support
endif()