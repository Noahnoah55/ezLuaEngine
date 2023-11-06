#include"engine.hpp"
#include"shader.hpp"
#include"gfx.hpp"
#include"input.hpp"

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
        return -1;
    }

    this->modules.push_back(new input());
    this->modules.push_back(new gfx());

    for (module *m : this->modules) {
        if (m->initialize(&this->lua_state) != 0) {
            std::cout << m->get_error(NULL) << "\n";
            return -1;
        }
    }

    return 0;
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

    return 0;
}

void ezlua::engine::tick() {
    lua_ontick();

    for (module *m : this->modules) {
        if (m->on_tick(&this->lua_state)) {
            std::cout << m->get_error(NULL) << "\n";
        }
    }
}