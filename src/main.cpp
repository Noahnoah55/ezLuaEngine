#include<iostream>
#include<emscripten.h>
#include<SDL2/SDL.h>

#include<sol/sol.hpp> 

#include"api.hpp"
#include"singletons.hpp"

SDL_Window *window;
SDL_Renderer *renderer;
EZLUA_AssetStore ASSET_STORE;
sol::state lua;
sol::protected_function lua_update;


bool init_lua() {
    lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::math, sol::lib::package);
    lua.script(
        R"(
            function __handler (message)
                print("[LUA-ERR]" .. message)
            end
        )");
    auto pfr = lua.safe_script_file("game-files/src/main.lua", sol::script_pass_on_error);
    if (!pfr.valid()) {
        sol::error err = pfr;
        std::cout << err.what() << "\n";
        return false;
    }
    init_api(lua);
    lua_update = sol::protected_function(lua["_update"], lua["__handler"]);
    return true;
}

void main_loop() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    // Poll events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        // Maybe useful later? At least this will still pump events
    }

    lua_update();

    // Render everything from a buffer to the actual screen.
    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(600,300,0,&window, &renderer);
    if (init_lua()) {
        emscripten_set_main_loop(main_loop, 0, 1);
    }

    SDL_Quit();
    return 0;
}