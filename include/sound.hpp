#include<module.hpp>
#include<sol/sol.hpp>

namespace ezlua{
    class soundimpl;
    class sound: virtual public module {
        public:
            sound();
            ~sound();
            int initialize(sol::state *lua);
            int on_tick(sol::state *lua);
            const char* get_name(int *len);
        
        private:
            soundimpl *internal;
    };
}