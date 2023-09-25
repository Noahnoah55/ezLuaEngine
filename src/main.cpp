#include<iostream>
#include<emscripten.h>
#include<SDL2/SDL.h>
#include"../lua/lua.h"
#include"../lua/lauxlib.h"
#include"../lua/lualib.h"
#include"api.hpp"
#include"singletons.hpp"

SDL_Window *window;
SDL_Renderer *renderer;
lua_State *L;

void init_lua() {
    L = luaL_newstate(); // Perhaps make my own allocater later?
    luaL_openlibs(L);
    init_api();
    if (luaL_dofile(L, "game-code/main.lua") != 0) {
        printf("Lua error: %s\n", lua_tostring(L, -1));
    }
    else {
        printf("Lua initialized successfully\n");
    }
}

void lua_update() {
    if (lua_getglobal(L, "_update") && lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, 0, 0) != 0) {
            std::cout << "Error running _update()\n";
        }
    }
    else {
        std::cout << "Could not find function _update()\n";
    }
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