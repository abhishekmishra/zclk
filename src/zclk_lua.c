#include "zclk_lua.h"
#include "zclk.h"

static zclk_res lua_cmd_handler(zclk_command* cmd, void* handler_args)
{
    printf("command %s\n", cmd->name);

    return ZCLK_RES_SUCCESS;
}

static int zclk_command_free(lua_State *L)
{
    zclk_command *cmd = *((zclk_command**)luaL_checkudata(L, 1, LUA_ZCLK_COMMAND_OBJECT));
    free_command(cmd);
    return 0;
}

static int zclk_command_new(lua_State *L)
{
    int ref = luaL_ref(L, LUA_REGISTRYINDEX);
    const char* desc = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);
    const char* short_name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);
    const char* name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    printf("name = %s, short = %s, desc = %s, handler fn ref = %d\n", name, short_name, desc, ref);

    //TODO: get lua command handler function and use it here.
    zclk_command *cmd = new_zclk_command(name, short_name, desc, &lua_cmd_handler);
    cmd->lua_handler_ref = ref;
    
    zclk_command** cmdptr = lua_newuserdata(L, sizeof(zclk_command*));
    (*cmdptr) = cmd;
    
    // set metatable of zclk_command object
    luaL_getmetatable(L, LUA_ZCLK_COMMAND_OBJECT);
    lua_setmetatable(L, -2);
    
    return 1;
}

static const luaL_Reg ZclkCommand_funcs[] =
{
    {"new", zclk_command_new},
    {NULL, NULL}
};

static const luaL_Reg ZclkCommand_meths[] =
{
    {"__gc", zclk_command_free},
    {NULL, NULL}
};

int luaopen_zclk(lua_State* L)
{
    // create turtle metatable
    luaL_newmetatable(L, LUA_ZCLK_COMMAND_OBJECT);

    // metatable.__index = metatable
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    // register methods
    luaL_setfuncs(L, ZclkCommand_meths, 0);
    
    // register functions - only turtle.new
    luaL_newlib(L, ZclkCommand_funcs);

    return 1;
}
