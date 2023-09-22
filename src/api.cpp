#include<emscripten.h>
#include<iostream>
#include<stdio.h>
#include<SDL2/SDL.h>
#include"../lua/lua.h"
#include"../lua/lauxlib.h"
#include"../lua/lualib.h"
#include"singletons.hpp"

// Not fixed
void drawSquare(int x0, int y0, int x1, int y1) {
    SDL_Rect rect;
    rect.x = x0;
    rect.y = y0;
    rect.w = x1;
    rect.h = y1;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    //std::cout << rect.x << rect.y << rect.w << rect.h << "\n";
}

// Not fixed
int getKey(char *keyname)
{
    int len;
    const Uint8 *keystate = SDL_GetKeyboardState(&len);
    return keystate[SDL_GetScancodeFromName(keyname)];
}

static int logMsg(lua_State *L)
{
    int n = lua_gettop(L);
    if (n != 1) {
        lua_pushliteral(L, "expected 1 argument");
        lua_error(L);
    }
    if (!lua_isstring(L, 1)) {
        lua_pushliteral(L, "expected a string");
        lua_error(L);
    }
    const char *msg = lua_tostring(L, 1);
    printf("[LUA] %s\n", msg);
    return 0;
}

void init_api() {
    lua_register(L, "logMsg", logMsg);
}