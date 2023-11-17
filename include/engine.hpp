#pragma once

#include"module.hpp"

#include<string>
#include<sol/sol.hpp>

using namespace std;

namespace ezlua {
    
    class engine {
        public:
            int initialize();
            void tick();

        private:
            int init_lua();
            sol::state lua_state;
            sol::protected_function lua_ontick;
            void set_ontick(sol::function);
            
            std::vector<module*> modules;
    };
}