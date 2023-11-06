#pragma once

#include"module.hpp"

#include<string>
#include<SDL2/SDL.h>
#include<SDL2/SDL_opengl.h>
#include<sol/sol.hpp>
#include<functional>

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
            void draw_spr(float x, float y, float width, float height, float rot, std::string path);
            
            std::vector<module*> modules;
    };
}