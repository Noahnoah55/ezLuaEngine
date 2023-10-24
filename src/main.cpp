#include<iostream>
#include<emscripten.h>
#include<emscripten/fetch.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>

#include<sol/sol.hpp> 

#include"api.hpp"
#include"singletons.hpp"
#include"fetch.hpp"

int download_done = 0;

SDL_Window *window;
SDL_Renderer *renderer;
EZLUA_AssetStore ASSET_STORE;
sol::state lua;
sol::protected_function lua_update;

Uint8 lastKeystate[SDL_NUM_SCANCODES] = {0};

bool init_lua() {
    lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::math, sol::lib::package);
    lua.script(
        R"(
            function __handler (message)
                print("[LUA-ERR]" .. message)
            end
        )");
    auto pfr = lua.safe_script_file("/main.lua", sol::script_pass_on_error);
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
    while (SDL_PollEvent(&e))
    {
        // Maybe useful later? At least this will still pump events
    }

    lua_update();

    memcpy(lastKeystate, SDL_GetKeyboardState(NULL), sizeof(lastKeystate));

    // Render everything from a buffer to the actual screen.
    SDL_RenderPresent(renderer);
}

void download_loop() {
    if (download_done == 1) {
        init_lua();
        printf("done?\n");
        download_done = 2;
        return;
    }
    if (download_done == 2) {
        main_loop();
        return;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Event e;
    while (SDL_PollEvent(&e)) {

    }
    SDL_RenderPresent(renderer);
}

int main() {
    init_filesystem();
    download_file_to_FS("src/main.lua");
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

    if (SDL_CreateWindowAndRenderer(600,300,0,&window, &renderer) != 0) {
        std::cout << "SDL failed to create a window, error: " << SDL_GetError() << "\n";
    }

    emscripten_set_main_loop(download_loop, 0, 1);

    SDL_Quit();
    return 0;
}