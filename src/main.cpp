#include"engine.hpp"
#include<emscripten.h>

ezlua::engine engine;

void main_loop() {
    engine.tick();
}

int main() {
    engine.initialize();

    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}