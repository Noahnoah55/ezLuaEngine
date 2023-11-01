#pragma once

namespace ezlua {
    enum TEXT_ALIGN
    {
        ALIGN_RIGHT,
        ALIGN_LEFT,
        ALIGN_CENTER
    };

    struct color
    {
        int r;
        int g;
        int b;
        int a;
    };
}