#include<iostream>
#include<emscripten.h>
#include<SDL2/SDL.h>

SDL_Window *window;
SDL_Renderer *renderer;
int a = 1;

EM_JS(void, call_alert, (), {
    alert('hello world');
    throw 'all done';
});

void mainloop() {
    // Poll events
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_KEYUP) {
            if (e.key.keysym.scancode == SDL_SCANCODE_RETURN) {
                call_alert();
            }
        }
    }

    // Set a color for drawing matching the earlier `ctx.fillStyle = "green"`.
    SDL_SetRenderDrawColor(renderer, /* RGBA: green */ 0x00, 0x80, 0x00, 0xFF);
    // Create and draw a rectangle like in the earlier `ctx.fillRect()`.
    SDL_Rect rect = {.x = 10+a, .y = 10, .w = 150, .h = 100};
    SDL_RenderFillRect(renderer, &rect);

    // Render everything from a buffer to the actual screen.
    SDL_RenderPresent(renderer);
}

int main() {
    std::cout << "Hello, world!\n";
    SDL_Init(SDL_INIT_VIDEO);

    SDL_CreateWindowAndRenderer(600,300,0,&window, &renderer);
    emscripten_set_main_loop(mainloop, 0, 1);

    SDL_Quit();
    return 0;
}