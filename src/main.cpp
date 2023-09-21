#include<iostream>
#include<emscripten.h>
#include<SDL2/SDL.h>
#include"api.hpp"
#include"singletons.hpp"

SDL_Window *window;
SDL_Renderer *renderer;
int a = 1;

EM_JS(void, init_lua, (), {
    lualib.luaL_openlibs(L);
    lauxlib.luaL_dofile(L,fengari.to_luastring("main.lua"));
});

EM_JS(void, lua_update, (), {
    lua.lua_getglobal(L, fengari.to_luastring("_update"));
    if (lua.lua_isfunction(L, -1)) {
        lua.lua_call(L, 0, 0);
    }
    else {
        console.log("_update is not a function");
    }
})

void mainloop() {
    // Poll events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYUP) {
            if (e.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                //std::cout << "Enter pressed\n";
            }
            if (e.key.keysym.scancode == SDL_SCANCODE_Q) {
                //std::cout << "Q pressed\n";
            }
        }
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