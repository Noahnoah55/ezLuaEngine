#pragma once
#include<module.hpp>
#include<string>

namespace ezlua {
    struct color {
        float r;
        float g;
        float b;
        float a;
    };

    struct transform {
        float x;
        float y;
        float width;
        float height;
        float rotation;
    };
    
    class gfx: virtual public module {
        int initialize(sol::state *lua);
        int on_tick(sol::state *lua);
        const char* get_name(int *len);

        void _draw_rect(transform position, color col);
        void draw_rect(float x, float y, float width, float height, float rot, sol::table color);
        void _draw_spr(transform position, std::string path);
        void draw_spr(float x, float y, float width, float height, float rot, std::string path);
    };
}