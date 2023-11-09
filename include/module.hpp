#pragma once
#include<sol/sol.hpp>

namespace ezlua {
    class module {
        public:
            virtual int initialize(sol::state *lua) = 0;
            virtual int on_tick(sol::state *lua) = 0;
            virtual const char* get_name(int *len) = 0;
    };
}