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
            arglist.push(lua.lua_tonumber(L, i));
            console.log(arglist[i]);
            i++;
        }
        ccall(fname, ret, args, arglist);
        return 0;
    })
    lua.lua_register(L, fname, func)
}