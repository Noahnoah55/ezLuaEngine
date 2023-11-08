#pragma once

#include"module.hpp"

#include<sol/sol.hpp>
#include<SDL2/SDL.h>
#include<string>

namespace ezlua {
    class input: virtual public module {
        public:
            int initialize(sol::state *lua);
            int on_tick(sol::state *lua);
            const char* get_error(int *len);

            bool get_key_held(std::string key_name);
            bool get_key_just_pressed(std::string key_name);
            
            bool get_scan_held(std::string scan_name);
            bool get_scan_just_pressed(std::string scan_name);

        private:
            std::string error = "";
            Uint8 keystate_current[SDL_NUM_SCANCODES];
            Uint8 keystate_last[SDL_NUM_SCANCODES];
    };
}