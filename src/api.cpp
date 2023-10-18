#include<SDL2/SDL.h>
#include<string>
#include"api.hpp"
#include"singletons.hpp"

bool get_key(std::string keyname) {
    auto scan  = SDL_GetScancodeFromName(keyname.c_str());
    return SDL_GetKeyboardState(nullptr)[scan];
}

void draw_rect(float x, float y, float w, float h, sol::table color) {
    SDL_SetRenderDrawColor(renderer, color[1], color[2], color[3], 255);
    SDL_FRect r = {.x=x, .y=y, .w=w, .h=h};
    SDL_RenderFillRectF(renderer, &r);
}

void draw_image(std::string path, float x, float y, float wscale, float hscale) {
    auto tex = ASSET_STORE.get_texture(path);
    if (tex == NULL) {
        std::cout << "[ERROR] Could not find texture at path " << path << "\n";
    }
    int w; int h;
    auto res = SDL_QueryTexture(tex, NULL, NULL, &w, &h);
    SDL_FRect rect; rect.x=x; rect.y=y; rect.w=wscale*w; rect.h=hscale*h;
    SDL_RenderCopyF(renderer, tex, NULL, &rect);
}

void draw_line(float x1, float y1, float x2, float y2, sol::table color) {
    SDL_SetRenderDrawColor(renderer, color[1], color[2], color[3], 255);
    SDL_RenderDrawLineF(renderer, x1, y1, x2, y2);
}

int init_api(sol::state &lua) {
    lua.set_function("drawRect", draw_rect);
    lua.set_function("getKey", get_key);
    lua.set_function("drawImage", draw_image);
    lua.set_function("drawLine", draw_line);

    return 0;
}