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
int a = 1;

void init_lua() {
    L = luaL_newstate(); // Perhaps make my own allocater later?
    luaL_openlibs(L);
    luaL_dostring(L, "function _update()\n\tlogMsg(\"Test!\")\nend");
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

    // Set a color for drawing matching the earlier `ctx.fillStyle = "green"`.
    SDL_SetRenderDrawColor(renderer, /* RGBA: green */ 0x00, 0x80, 0x00, 0xFF);
    // Create and draw a rectangle like in the earlier `ctx.fillRect()`.
    SDL_Rect rect = {.x = 10+a, .y = 10, .w = 150, .h = 100};
    SDL_RenderFillRect(renderer, &rect);
    a++;

    // Render everything from a buffer to the actual screen.
    SDL_RenderPresent(renderer);
}

int main() {
    std::cout << "Hello, world!\n";
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(600,300,0,&window, &renderer);
    init_lua();
    init_api();
    emscripten_set_main_loop(mainloop, 0, 1);

    SDL_Quit();
    return 0;
}