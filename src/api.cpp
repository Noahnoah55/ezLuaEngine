#include<emscripten.h>
#include<iostream>
#include<SDL2/SDL.h>
#include"singletons.hpp"

void init_api() {
    EM_ASM(
        add_func("drawSquare", 'number', ['number', 'number', 'number', 'number']);
    );
}

int _drawSquare(int x0, int y0, int x1, int y1) {
    SDL_Rect rect;
    rect.x = x0;
    rect.y = y0;
    rect.w = x1;
    rect.h = y1;
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
    std::cout << rect.x << rect.y << rect.w << rect.h << "\n";
    return 0;
}

extern "C" {
    int drawSquare(int x0, int y0, int x1, int y1)
    {
        return _drawSquare(x0, y0, x1, y1);
    }
}