#pragma once

#include"assets.hpp"
#include"types.hpp"

#include<string>
#include<SDL2/SDL.h>
#include<SDL2/SDL_opengl.h>
#include<sol/sol.hpp>

using namespace std;

namespace ezlua {
    class engine {
        public:
            int initialize();
            void tick();

            void draw_text(string fontpath, string text, int x, int y, TEXT_ALIGN alignment, color c);
            void draw_line(int x, int y, int width, color c);
            void draw_rect(int x, int y, int width, int height, sol::table color);
            void draw_sprite(int x, int y, int width, int height, string path);

        private:
            int _init_lua();
            SDL_Window *window;
            SDL_GLContext gl_context;
            asset_store assets;
            sol::state lua_state;
            sol::protected_function lua_ontick;
    };
}