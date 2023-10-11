#include<SDL2/SDL.h>
#include<string>
#include"api.hpp"
#include"singletons.hpp"

bool get_key(std::string keyname) {
    auto scan  = SDL_GetScancodeFromName(keyname.c_str());
    return SDL_GetKeyboardState(nullptr)[scan];
}

void draw_rect(int x, int y, int w, int h) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect r = {.x=x, .y=y, .w=w, .h=h};
    SDL_RenderFillRect(renderer, &r);
}

int init_api(sol::state &lua) {
    lua.set_function("drawRect", draw_rect);
    lua.set_function("getKey", get_key);

    return 0;
}