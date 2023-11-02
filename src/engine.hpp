#pragma once

#include"assets.hpp"
#include"types.hpp"
#include"shader.hpp"

#include<string>
#include<SDL2/SDL.h>
#include<SDL2/SDL_opengl.h>
#include<sol/sol.hpp>

using namespace std;

namespace ezlua {
    enum SHAPE {
        RECTANGLE,
        LINE,
        _SHAPE_TOTAL
    };
    class engine {
        public:
            int initialize();
            void tick();

            void draw_text(string fontpath, string text, int x, int y, TEXT_ALIGN alignment, color c);
            void draw_line(int x, int y, int width, color c);
            void draw_rect(int x, int y, int width, int height, sol::table color);
            void draw_sprite(int x, int y, int width, int height, string path);

        private:
            void generate_shapes();
            int _init_lua();
            SDL_Window *window;
            SDL_GLContext gl_context;
            asset_store assets;
            sol::state lua_state;
            sol::protected_function lua_ontick;
            unsigned int vaos[static_cast<int>(SHAPE::_SHAPE_TOTAL)];

            ezlua::shader_program* shader;
    };
}