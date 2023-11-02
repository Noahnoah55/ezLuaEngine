#include"engine.hpp"
#include"shader.hpp"
#include"gfx.hpp"

#include<iostream>
#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_opengl.h>

#include<GLES3/gl3.h>
#include<GL/glew.h>

float verts[] = {
    0.5f, 0.5f, 0.0f,
    0.5f, -0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    -0.5f, 0.5f, 0.0f
};

unsigned int indices[] = {
    0, 1, 3,
    1, 2, 3
};

int ezlua::engine::initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cout << "SDL failed to initialize, error: " << SDL_GetError() << "\n";
        return -1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) != 0) {
        std::cout << "SDL_mixer failed to initialize, error: " << Mix_GetError() << "\n";
        return -1;
    }

    if (TTF_Init() != 0) {
        std::cout << "SDL_ttf failed to initialize, error: " << TTF_GetError() << "\n";
        return -1;
    }

    ezlua::gfx::init_gfx();


    int success = 0;

    success = success | init_lua();

    return success;
}

void ezlua::engine::init_primatives(){
}

int ezlua::engine::init_lua() {
    lua_state.open_libraries(sol::lib::base, sol::lib::table, sol::lib::math, sol::lib::package);
    lua_state.script(
        R"(
            function __handler (message)
                print("[LUA-ERROR]" .. message)
            end
        )");
    auto pfr = lua_state.safe_script_file("/src/main.lua", sol::script_pass_on_error);
    if (!pfr.valid()) {
        sol::error err = pfr;
        std::cout << err.what() << "\n";
        return -1;
    }
    lua_ontick = sol::protected_function(lua_state["_update"], lua_state["__handler"]);

    lua_state.set_function("drawRect", &ezlua::engine::draw_rect, this);
    return 0;
}

void ezlua::engine::tick() {
    lua_ontick();
}

void ezlua::engine::draw_rect(float x, float y, float width, float height, float rot, sol::table color) {
    ezlua::gfx::transform trans{x, y, width, height, rot};
    ezlua::gfx::color col{color[1], color[2], color[3], 255};
    ezlua::gfx::draw_rect(trans, col);
}