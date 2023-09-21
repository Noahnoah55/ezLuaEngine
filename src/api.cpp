#include<emscripten.h>
#include<iostream>
#include<SDL2/SDL.h>
#include"singletons.hpp"

void init_api() {
    EM_ASM(
        add_func("drawSquare", null, ['number', 'number', 'number', 'number']);
        add_func("getKey", 'number', ['number']);
    );
}

void _drawSquare(int x0, int y0, int x1, int y1) {
    SDL_Rect rect;
    rect.x = x0;
    rect.y = y0;
    rect.w = x1;
    rect.h = y1;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    std::cout << rect.x << rect.y << rect.w << rect.h << "\n";
}

extern "C" {
    void drawSquare(int x0, int y0, int x1, int y1)
    {
        _drawSquare(x0, y0, x1, y1);
    }

    int getKey(int keycode)
    {
        int len;
        const Uint8* keystate = SDL_GetKeyboardState(&len);
        return keystate[keycode];
    }
}