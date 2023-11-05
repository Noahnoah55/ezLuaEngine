#pragma once
#include<string>

namespace ezlua::gfx {
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

    int init_gfx();
    void draw_rect(transform position, color col);
    void draw_spr(transform position, std::string path);
}