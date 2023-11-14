#include"engine.hpp"
#include"fetch.hpp"
#include<spdlog/spdlog.h>
#include<iostream>
#include<emscripten.h>

ezlua::engine engine;
bool initialized = false;

void main_loop() {
    if (initialized) {
        engine.tick();
    }
}

void init_engine() {
    spdlog::info("Engine initializing");
    if (engine.initialize() != 0) {
        spdlog::error("Engine failed to start");
        return;
    }
    spdlog::info("Engine intitialized successfully, switching to main loop");
    initialized = true;
}

int main() {
    spdlog::info("Fetching Manifest");
    fetch_by_manifest("manifest.txt", init_engine);

    emscripten_set_main_loop(main_loop, 0, 1);

    spdlog::critical("Exited main loop!");

    return 0;
}