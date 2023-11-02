#pragma once
#include<string>

namespace ezlua::gfx {
    struct color {
        int r;
        int g;
        int b;
        int a;
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
    void draw_spr(std::string path, transform position, color col);
}