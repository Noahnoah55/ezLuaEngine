#include<sol/sol.hpp>
#include<spdlog/spdlog.h>
#include"input.hpp"

static const char* name = "Input";

const char* ezlua::input::get_name(int *const len) {
    if (len != NULL) {
        *len = strlen(name);
    }
    return name;
}

int ezlua::input::initialize(sol::state *lua) {
    if (SDL_InitSubSystem(SDL_INIT_EVENTS) != 0) {
        spdlog::error("SDL failed to initialize events, reason: {}", SDL_GetError());
        return -1;
    }
    lua->set_function("getKeyHeld", &ezlua::input::get_key_held, this);
    lua->set_function("getKeyJustPressed", &ezlua::input::get_key_just_pressed, this);
    lua->set_function("getScanHeld", &ezlua::input::get_scan_held, this);
    lua->set_function("getScanJustPressed", &ezlua::input::get_scan_just_pressed, this);
    const Uint8* keystate_ptr = SDL_GetKeyboardState(NULL);
    memcpy(keystate_current, keystate_ptr, SDL_NUM_SCANCODES);
    memcpy(keystate_last, keystate_ptr, SDL_NUM_SCANCODES);
    return 0;
}

int ezlua::input::on_tick(sol::state *lua) {
    memcpy(keystate_last, keystate_current, SDL_NUM_SCANCODES);
    memcpy(keystate_current, SDL_GetKeyboardState(NULL), SDL_NUM_SCANCODES);
    return 0;
}

bool ezlua::input::get_key_held(std::string key_name) {
    SDL_Keycode key_code = SDL_GetKeyFromName(key_name.c_str());
    SDL_Scancode scan = SDL_GetScancodeFromKey(key_code);
    return keystate_current[scan];
}

bool ezlua::input::get_key_just_pressed(std::string key_name) {
    SDL_Keycode key_code = SDL_GetKeyFromName(key_name.c_str());
    SDL_Scancode scan = SDL_GetScancodeFromKey(key_code);
    return keystate_current[scan] && !keystate_last[scan];
}

bool ezlua::input::get_scan_held(std::string key_name) {
    SDL_Scancode scan = SDL_GetScancodeFromName(key_name.c_str());
    return keystate_current[scan];
}

bool ezlua::input::get_scan_just_pressed(std::string key_name) {
    SDL_Scancode scan = SDL_GetScancodeFromName(key_name.c_str());
    return keystate_current[scan] && !keystate_last[scan];
}