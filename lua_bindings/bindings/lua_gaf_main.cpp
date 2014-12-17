#include "lua_gaf_main.hpp"
#include "lua_gaf.hpp"

int append_gaf_scripts(lua_State* tolua_S)
{
    int top = lua_gettop(tolua_S);
    lua_getglobal(tolua_S, "_G");
    int result = register_all_gaf(tolua_S);
    lua_settop(tolua_S, top);

    return result;
}