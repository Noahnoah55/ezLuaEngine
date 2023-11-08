#include"engine.hpp"
#include<iostream>
#include<emscripten.h>

ezlua::engine engine;

void main_loop() {
    engine.tick();
}

int main() {
    std::cout << "Starting Engine\n";
    engine.initialize();
    std::cout << "Engine Started\n";

    emscripten_set_main_loop(main_loop, 0, 1);

    return 0;
}