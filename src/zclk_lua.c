#include "zclk_lua.h"
#include "zclk.h"

static void _stack_dump(lua_State *L)
{
    int i;
    int top = lua_gettop(L); /* depth of the stack */
    printf("lua stack -> [ ");
    for (i = 1; i <= top; i++)
    { /* repeat for each level */
        int t = lua_type(L, i);
        switch (t)
        {
        case LUA_TSTRING:
        { /* strings */
            printf("'%s'", lua_tostring(L, i));
            break;
        }
        case LUA_TBOOLEAN:
        { /* Booleans */
            printf(lua_toboolean(L, i) ? "true" : "false");
            break;
        }
        case LUA_TNUMBER:
        { /* numbers */
            printf("%g", lua_tonumber(L, i));
            break;
        }
        default:
        { /* other values */
            printf("%s", lua_typename(L, t));
            break;
        }
        }
        printf(" "); /* put a separator */
    }
    printf("]\n"); /* end the listing */
}

static zclk_command *zclk_command_getobj(lua_State *L)
{
    int top = lua_gettop(L);
    zclk_command *cmd = *((zclk_command**)luaL_checkudata(L, 1, LUA_ZCLK_COMMAND_OBJECT));
    if (cmd == NULL)
    {
        luaL_typeerror(L, top, LUA_ZCLK_COMMAND_OBJECT);
    }
    lua_pop(L, 1);
    return cmd;
}

static zclk_res lua_cmd_handler(zclk_command* cmd, void* handler_args)
{
    printf("command %s\n", cmd->name);
    lua_State *L = (lua_State *)handler_args;

    /* get the lua command handler from cmd object */
    lua_rawgeti(L, LUA_REGISTRYINDEX, cmd->lua_handler_ref);

    /* get the lua userdata for the cmd object */
    lua_rawgeti(L, LUA_REGISTRYINDEX, cmd->lua_udata_ref);

    // Call the handler with 1 arguments, returning 1 result
    lua_call(L, 1, 1);
    // Get the result
    int res = (int)lua_tointeger(L, -1);
    lua_pop(L, 1);

    return res;
}

static int zclk_command_free(lua_State *L)
{
    zclk_command *cmd = *((zclk_command**)luaL_checkudata(L, 1, LUA_ZCLK_COMMAND_OBJECT));
    free_command(cmd);
    return 0;
}

static int zclk_command_new(lua_State *L)
{
    int handler_ref = luaL_ref(L, LUA_REGISTRYINDEX);
    const char* desc = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);
    const char* short_name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);
    const char* name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    //printf("name = %s, short = %s, desc = %s, handler fn handler_ref = %d\n", 
    //name, short_name, desc, handler_ref);

    //TODO: get lua command handler function and use it here.
    zclk_command *cmd = new_zclk_command(name, short_name, desc, &lua_cmd_handler);
    cmd->lua_handler_ref = handler_ref;
    
    zclk_command** cmdptr = lua_newuserdata(L, sizeof(zclk_command*));
    (*cmdptr) = cmd;
    cmd->lua_udata_ref = luaL_ref(L, LUA_REGISTRYINDEX);

    lua_rawgeti(L, LUA_REGISTRYINDEX, cmd->lua_udata_ref);
    _stack_dump(L);

    // set metatable of zclk_command object
    luaL_getmetatable(L, LUA_ZCLK_COMMAND_OBJECT);
    lua_setmetatable(L, -2);
    
    return 1;
}

static int zclk_command_lua_exec(lua_State *L)
{
    if(lua_istable(L, lua_gettop(L))) {
        int arglen = luaL_len(L, lua_gettop(L)) + 1;
        char **lua_argv;
        lua_argv = (char **)calloc(arglen, sizeof(char *));
        if(lua_argv == NULL)
        {
            return luaL_error(L, "Error allocating array of arg values.\n");
        }

        /* table is in the stack at index 't' */
        lua_pushnil(L);  /* first key */

        for (int i = 1; i < arglen; i++)
        {
            if (lua_next(L, -2) != 0)
            {
                /* uses 'key' (at index -2) and 'value' (at index -1) */
                // printf("%s - %s\n",
                //        lua_typename(L, lua_type(L, -2)),
                //        lua_typename(L, lua_type(L, -1)));

                const char *arg_val = luaL_checkstring(L, -1);
                lua_argv[i] = (char *)arg_val;
                /* removes 'value'; keeps 'key' for next iteration */
                lua_pop(L, 1);
            }
            else
            {
                return luaL_error(L, "argument #%d not found.\n", i);
            }
        }

        /* pop the index for the lua_next call*/
        lua_pop(L, 1);

        /* get the 0th value i.e. name of the script */
        lua_geti(L, lua_gettop(L), 0);
        const char *arg_val = luaL_checkstring(L, lua_gettop(L));
        lua_argv[0] = (char *)arg_val;

        /* pop the 0th arg value*/
        lua_pop(L, 1);
        /* pop the arg table */
        lua_pop(L, 1);

        zclk_command *cmd = zclk_command_getobj(L);
        // printf("Exec command: %s\n", cmd->name);
        // for (int i = 0; i < arglen; i++)
        // {
        //     printf("arg[%2d] = %s\n", i, lua_argv[i]);
        // }

        zclk_command_exec(cmd, L, arglen, lua_argv);

        return 0;
    }
    else{
        return luaL_error(L, "first argument should be arg table");
    }
}

static const luaL_Reg ZclkCommand_funcs[] =
{
    {"new", zclk_command_new},
    {NULL, NULL}
};

static const luaL_Reg ZclkCommand_meths[] =
{
    {"__gc", zclk_command_free},
    {"exec", zclk_command_lua_exec},
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
