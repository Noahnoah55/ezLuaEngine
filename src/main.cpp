#include<iostream>
#include<emscripten.h>
#include<SDL2/SDL.h>

#include<lua.h>
#include<lualib.h>
#include<lauxlib.h>
#include<sol/sol.hpp> // idk why but sol doesn't like to be included without lua already included

SDL_Window *window;
SDL_Renderer *renderer;
lua_State *L;
sol::state lua;

void init_lua() {
    lua.open_libraries(sol::lib::base);
    lua.script_file("game-code/main.lua", [](lua_State *, sol::protected_function_result pfr) {
        printf("Failed to load soltest!\n");
        return pfr;
    });
}

void lua_update() {
}

void mainloop() {
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
    std::cout << "Hello, world!\n";
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(600,300,0,&window, &renderer);
    init_lua();
    emscripten_set_main_loop(mainloop, 0, 1);

    SDL_Quit();
    return 0;
}