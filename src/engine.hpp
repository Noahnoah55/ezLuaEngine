#pragma once

#include"types.hpp"
#include"shader.hpp"

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

        private:
            void init_primatives();
            int init_lua();
            SDL_Window *window;
            SDL_GLContext gl_context;
            sol::state lua_state;
            sol::protected_function lua_ontick;

            void draw_rect(float x, float y, float width, float height, float rot, sol::table color);
    };
}