#include<iostream>
#include<emscripten.h>
#include<SDL2/SDL.h>

#include<lua.h>
#include<lualib.h>
#include<lauxlib.h>
#include<sol/sol.hpp> // idk why but sol doesn't like to be included without lua already included

SDL_Window *window;
SDL_Renderer *renderer;
sol::state lua;

bool getKey(std::string keyname) {
    auto scan  = SDL_GetScancodeFromName(keyname.c_str());
    return SDL_GetKeyboardState(nullptr)[scan];
}

void drawSquare(int x, int y, int w, int h) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect r = {.x=x, .y=y, .w=w, .h=h};
    SDL_RenderFillRect(renderer, &r);
}

void init_lua() {
    lua.open_libraries(sol::lib::base, sol::lib::table, sol::lib::math);
    lua.script_file("game-code/main.lua");
    lua.set_function("drawSquare", drawSquare);
    lua.set_function("getKey", getKey);
}

void lua_update() {
    lua.script("_update()\n");
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