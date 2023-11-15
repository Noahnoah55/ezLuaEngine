#include"engine.hpp"
#include"shader.hpp"
#include"gfx.hpp"
#include"input.hpp"
#include"sound.hpp"

#include<spdlog/spdlog.h>

#include<iostream>
#include<SDL2/SDL.h>

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
    if (init_lua()){
        spdlog::error("Failed to initialize LUA");
        return -1;
    }
    spdlog::info("Initialized Lua");

    this->modules.push_back(new input());
    this->modules.push_back(new gfx());
    this->modules.push_back(new sound());

    for (module *m : this->modules) {
        int ret = m->initialize(&this->lua_state);
        if (ret != 0) {
            spdlog::error("Failed to initialize module {}, error code ({})", m->get_name(NULL), ret);
            return -1;
        }
        spdlog::info("Initialized module {}", m->get_name(NULL));
    }

    return 0;
}

std::string error_handler(std::string msg) {
    spdlog::error("LUA Error: {}", msg);
    return msg;
}

int ezlua::engine::init_lua() {
    lua_state.open_libraries(sol::lib::base, sol::lib::table, sol::lib::math, sol::lib::package);
    lua_state["__handler"] = error_handler;
    auto lr = lua_state.load_file("/src/main.lua");
    if (!lr.valid()) {
        sol::error err = lr;
        spdlog::error("Lua error on loading /src/main.lua:\n{}",err.what());
        return -1;
    }
    sol::protected_function pf = lr;
    auto pfr = pf();
    if (!pfr.valid()) {
        sol::error err = pfr;
        spdlog::error("Lua error on running /src/main.lua:\n{}",err.what());
        return -1;
    }
    lua_ontick = sol::protected_function(lua_state["_update"], lua_state["__handler"]);

    return 0;
}

void ezlua::engine::tick() {
    lua_ontick();

    for (module *m : this->modules) {
        int ret = m->on_tick(&this->lua_state);
        if (ret) {
            spdlog::error("Faile to tick module {}, error code ({})", m->get_name(NULL), ret);
        }
    }
}