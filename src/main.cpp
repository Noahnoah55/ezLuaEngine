#include"engine.hpp"
#include<spdlog/spdlog.h>
#include<iostream>
#include<emscripten.h>

ezlua::engine engine;

void main_loop() {
    engine.tick();
}

int main() {
    spdlog::info("Engine initializing");
    if (engine.initialize() != 0) {
        spdlog::error("Engine failed to start");
        return -1;
    }
    spdlog::info("Engine intitialized successfully, starting main loop");

    emscripten_set_main_loop(main_loop, 0, 1);

    spdlog::critical("Exited main loop!");

    return 0;
}