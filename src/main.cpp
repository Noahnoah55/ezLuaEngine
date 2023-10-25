#include<iostream>
#include<set>
#include<mutex>
#include<emscripten.h>
#include<emscripten/fetch.h>
#include<SDL2/SDL.h>
#include<SDL2/SDL_mixer.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_image.h>

#include<sol/sol.hpp> 

#include"api.hpp"
#include"singletons.hpp"
#include"fetch.hpp"


int last = 0;

PHASE GAME_PHASE = DOWNLOAD_NOT_STARTED;

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
    auto pfr = lua.safe_script_file("/src/main.lua", sol::script_pass_on_error);
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
    if (GAME_PHASE == DOWNLOAD_FINISHED) {
        init_lua();
        printf("done?\n");
        GAME_PHASE = GAME_RUNNING;
        return;
    }
    if (GAME_PHASE == GAME_RUNNING) {
        main_loop();
        return;
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_Event e;
    while (SDL_PollEvent(&e)) {

    }
    SDL_RenderPresent(renderer);
    if (GAME_PHASE == DOWNLOAD_STARTED) {
        std::lock_guard<std::mutex> l(fset_mut);
        if (fset.size() != last) {
            printf("Remaining files: %d\n", (int)fset.size());
            if (fset.size() == 0) {
                GAME_PHASE = DOWNLOAD_FINISHED;
            }
            last = fset.size();
        }
    }
}

int main() {
    init_filesystem();
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
    
    int img_flags = 0;

    if (IMG_Init(img_flags) != img_flags) {
        std::cout << "SDL_Image failed to initialize, error: " << IMG_GetError() << "\n";
        return -1;
    }

    if (SDL_CreateWindowAndRenderer(600,300,0,&window, &renderer) != 0) {
        std::cout << "SDL failed to create a window, error: " << SDL_GetError() << "\n";
    }

    emscripten_set_main_loop(download_loop, 0, 1);

    SDL_Quit();
    return 0;
}