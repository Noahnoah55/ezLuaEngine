const luaconf = fengari.luaconf;
const lua     = fengari.lua;
const lauxlib = fengari.lauxlib;
const lualib  = fengari.lualib;

const L = lauxlib.luaL_newstate();


function add_func(fname, ret, args) {
    //func = cwrap(fname, ret, args)
    func = (function() {
        arglist = [];
        i = 1;
        for (arg in args) {
            switch (args[arg]) {
                case 'number':
                    arglist.push(lua.lua_tonumber(L, i));
                    break;
                case 'str':
                    arglist.push(stringToNewUTF8(lua.lua_tojsstring(L,i)));
                    break;
            }
            i++;
        }
        var retval = ccall(fname, ret, args, arglist);
        switch (ret) {
            case 'number':
                lua.lua_pushnumber(L, retval);
                //console.log(retval);
                return 1;
            case null:
                return 0;
        }
        return 0;
    })
    lua.lua_register(L, fname, func)
}

lua.lua_register(L, "log", (function() {
    str = lua.lua_tojsstring(L, 1);
    console.log(str)
    return 0;
}))