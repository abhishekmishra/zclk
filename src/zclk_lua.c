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
    zclk_command *cmd = *((zclk_command**)luaL_checkudata(L, top, LUA_ZCLK_COMMAND_OBJECT));
    if (cmd == NULL)
    {
        luaL_typeerror(L, top, LUA_ZCLK_COMMAND_OBJECT);
    }
    lua_pop(L, 1);
    return cmd;
}

static zclk_option *zclk_option_getobj(lua_State *L)
{
    int top = lua_gettop(L);
    zclk_option *opt = *((zclk_option**)luaL_checkudata(L, top, LUA_ZCLK_OPTION_OBJECT));
    if (opt == NULL)
    {
        luaL_typeerror(L, top, LUA_ZCLK_OPTION_OBJECT);
    }
    lua_pop(L, 1);
    return opt;
}

static zclk_argument *zclk_argument_getobj(lua_State *L)
{
    int top = lua_gettop(L);
    zclk_argument *arg = *((zclk_argument**)luaL_checkudata(L, top, LUA_ZCLK_ARGUMENT_OBJECT));
    if (arg == NULL)
    {
        luaL_typeerror(L, top, LUA_ZCLK_ARGUMENT_OBJECT);
    }
    lua_pop(L, 1);
    return arg;
}

static zclk_res lua_cmd_handler(zclk_command* cmd, void* handler_args)
{
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

static int zclk_command_lua_name_get(lua_State *L)
{
    zclk_command *cmd = zclk_command_getobj(L);
    lua_pushstring(L, cmd->name);
    return 1;
}

static int zclk_command_lua_short_name_get(lua_State *L)
{
    zclk_command *cmd = zclk_command_getobj(L);
    lua_pushstring(L, cmd->short_name);
    return 1;
}

static int zclk_command_lua_desc_get(lua_State *L)
{
    zclk_command *cmd = zclk_command_getobj(L);
    lua_pushstring(L, cmd->description);
    return 1;
}

// static int zclk_option_free(lua_State *L)
// {
//     zclk_option *opt = *((zclk_option**)luaL_checkudata(L, 1, LUA_ZCLK_OPTION_OBJECT));
//     free_option(opt);
//     return 0;
// }

// static int zclk_argument_free(lua_State *L)
// {
//     zclk_argument *arg = *((zclk_argument**)luaL_checkudata(L, 1, LUA_ZCLK_ARGUMENT_OBJECT));
//     free_argument(arg);
//     return 0;
// }

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
static int zclk_command_lua_bool_option(lua_State *L)
{
    const char *desc = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);
    
    const char *short_name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    const char *name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    zclk_command *cmd = zclk_command_getobj(L);
    
    zclk_command_bool_option(
        cmd,
        name,
        short_name, 
        desc
    );
    
    return 0;
}

static int zclk_command_lua_int_option(lua_State *L)
{
    const char *desc = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    int default_val = luaL_checkinteger(L, lua_gettop(L));
    lua_pop(L, 1);
    
    const char *short_name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    const char *name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    zclk_command *cmd = zclk_command_getobj(L);
    
    zclk_command_int_option(
        cmd,
        name,
        short_name, 
        default_val, 
        desc
    );
    
    return 0;
}

static int zclk_command_lua_double_option(lua_State *L)
{
    const char *desc = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    double default_val = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);
    
    const char *short_name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    const char *name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    zclk_command *cmd = zclk_command_getobj(L);
    
    zclk_command_double_option(
        cmd,
        name,
        short_name, 
        default_val, 
        desc
    );
    
    return 0;
}

static int zclk_command_lua_string_option(lua_State *L)
{
    const char *desc = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    const char *default_val = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);
    
    const char *short_name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    const char *name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    zclk_command *cmd = zclk_command_getobj(L);
    
    zclk_command_string_option(
        cmd,
        name,
        short_name, 
        default_val, 
        desc
    );
    
    return 0;
}

static int zclk_command_lua_flag_option(lua_State *L)
{
    const char *desc = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);
    
    const char *short_name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    const char *name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    zclk_command *cmd = zclk_command_getobj(L);
    
    zclk_command_flag_option(
        cmd,
        name,
        short_name, 
        desc
    );
    
    return 0;
}

static int zclk_command_lua_bool_argument(lua_State *L)
{
    const char *desc = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    int default_val = lua_toboolean(L, lua_gettop(L));
    lua_pop(L, 1);
    
    const char *name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    zclk_command *cmd = zclk_command_getobj(L);
    
    zclk_command_bool_argument(
        cmd,
        name,
        default_val, 
        desc,
        1
    );
    
    return 0;
}

static int zclk_command_lua_int_argument(lua_State *L)
{
    const char *desc = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    int default_val = luaL_checkinteger(L, lua_gettop(L));
    lua_pop(L, 1);
    
    const char *name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    zclk_command *cmd = zclk_command_getobj(L);
    
    zclk_command_int_argument(
        cmd,
        name,
        default_val, 
        desc,
        1
    );
    
    return 0;
}

static int zclk_command_lua_double_argument(lua_State *L)
{
    const char *desc = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    double default_val = luaL_checknumber(L, lua_gettop(L));
    lua_pop(L, 1);
    
    const char *name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    zclk_command *cmd = zclk_command_getobj(L);
    
    zclk_command_double_argument(
        cmd,
        name,
        default_val, 
        desc,
        1
    );
    
    return 0;
}

static int zclk_command_lua_string_argument(lua_State *L)
{
    const char *desc = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    const char *default_val = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);
    
    const char *name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    zclk_command *cmd = zclk_command_getobj(L);
    
    zclk_command_string_argument(
        cmd,
        name,
        default_val, 
        desc,
        1
    );
    
    return 0;
}

static int zclk_command_lua_flag_argument(lua_State *L)
{
    const char *desc = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    int default_val = lua_toboolean(L, lua_gettop(L));
    lua_pop(L, 1);
    
    const char *name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    zclk_command *cmd = zclk_command_getobj(L);
    
    zclk_command_flag_argument(
        cmd,
        name,
        default_val, 
        desc,
        1
    );
    
    return 0;
}

static int zclk_command_lua_get_option(lua_State *L)
{
    const char *name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    zclk_command *cmd = zclk_command_getobj(L);

    zclk_option *opt = zclk_command_get_option(cmd, name);

    /* create new userdata from opt and assign the metatable */
    *(zclk_option**)(lua_newuserdata(L, sizeof(zclk_option *))) = opt;
    luaL_getmetatable(L, LUA_ZCLK_OPTION_OBJECT);
    lua_setmetatable(L, -2);

    return 1;
}

static int zclk_command_lua_get_argument(lua_State *L)
{
    const char *name = luaL_checkstring(L, lua_gettop(L));
    lua_pop(L, 1);

    zclk_command *cmd = zclk_command_getobj(L);

    zclk_argument *opt = zclk_command_get_argument(cmd, name);

    /* create new userdata from opt and assign the metatable */
    *(zclk_argument**)(lua_newuserdata(L, sizeof(zclk_argument *))) = opt;
    luaL_getmetatable(L, LUA_ZCLK_ARGUMENT_OBJECT);
    lua_setmetatable(L, -2);

    return 1;
}

static int zclk_command_lua_subcommand_add(lua_State *L)
{
    zclk_command *subcmd = zclk_command_getobj(L);
    zclk_command *cmd = zclk_command_getobj(L);

    zclk_res err = zclk_command_subcommand_add(cmd, subcmd);

    lua_pushinteger(L, err);
    return 1;
}

static int zclk_option_value(lua_State *L)
{
    zclk_option *opt = zclk_option_getobj(L);
    zclk_val *val = opt->val;
    if (zclk_val_is_bool(val))
    {
        lua_pushboolean(L, zclk_val_get_bool(val));
        return 1;
    }
    else if(zclk_val_is_int(val))
	{
        lua_pushinteger(L, zclk_val_get_int(val));
        return 1;
	}
	else if(zclk_val_is_double(val))
	{
        lua_pushnumber(L, zclk_val_get_double(val));
        return 1;
	}
	else if(zclk_val_is_string(val))
	{
        lua_pushstring(L, zclk_val_get_string(val));
        return 1;
	}
	else if(zclk_val_is_flag(val))
	{
        lua_pushboolean(L, zclk_val_get_bool(val));
        return 1;
	}
    else
    {
        return 0;
    }
}

static int zclk_option_type(lua_State *L)
{
    zclk_option *opt = zclk_option_getobj(L);
    zclk_type typ = opt->val->type;
    switch(typ)
    {
        case ZCLK_TYPE_BOOLEAN:
            lua_pushstring(L, "boolean");
            return 1;
        case ZCLK_TYPE_INT:
            lua_pushstring(L, "integer");
            return 1;
        case ZCLK_TYPE_DOUBLE:
            lua_pushstring(L, "double");
            return 1;
        case ZCLK_TYPE_STRING:
            lua_pushstring(L, "string");
            return 1;
        case ZCLK_TYPE_FLAG:
            lua_pushstring(L, "flag");
            return 1;
        default:
            lua_pushstring(L, "unknown");
            return 1;
    }
}

static int zclk_argument_value(lua_State *L)
{
    zclk_argument *arg = zclk_argument_getobj(L);
    zclk_val *val = arg->val;

    if (zclk_val_is_bool(val))
    {
        lua_pushboolean(L, zclk_val_get_bool(val));
        return 1;
    }
    else if(zclk_val_is_int(val))
	{
        lua_pushinteger(L, zclk_val_get_int(val));
        return 1;
	}
	else if(zclk_val_is_double(val))
	{
        lua_pushnumber(L, zclk_val_get_double(val));
        return 1;
	}
	else if(zclk_val_is_string(val))
	{
        lua_pushstring(L, zclk_val_get_string(val));
        return 1;
	}
	else if(zclk_val_is_flag(val))
	{
        lua_pushboolean(L, zclk_val_get_bool(val));
        return 1;
	}
    else
    {
        return 0;
    }
}

static int zclk_argument_type(lua_State *L)
{
    zclk_argument *arg = zclk_argument_getobj(L);
    zclk_type typ = arg->val->type;
    switch(typ)
    {
        case ZCLK_TYPE_BOOLEAN:
            lua_pushstring(L, "boolean");
            return 1;
        case ZCLK_TYPE_INT:
            lua_pushstring(L, "integer");
            return 1;
        case ZCLK_TYPE_DOUBLE:
            lua_pushstring(L, "double");
            return 1;
        case ZCLK_TYPE_STRING:
            lua_pushstring(L, "string");
            return 1;
        case ZCLK_TYPE_FLAG:
            lua_pushstring(L, "flag");
            return 1;
        default:
            lua_pushstring(L, "unknown");
            return 1;
    }
}

static const luaL_Reg ZclkCommand_funcs[] =
{
    {"new", zclk_command_new},
    {NULL, NULL}
};

static const luaL_Reg zclk_command_meths[] =
{
    {"__gc", zclk_command_free},
    {"name", zclk_command_lua_name_get},
    {"short_name", zclk_command_lua_short_name_get},
    {"description", zclk_command_lua_desc_get},
    {"desc", zclk_command_lua_desc_get},
    {"exec", zclk_command_lua_exec},
    {"bool_option", zclk_command_lua_bool_option},
    {"int_option", zclk_command_lua_int_option},
    {"double_option", zclk_command_lua_double_option},
    {"string_option", zclk_command_lua_string_option},
    {"flag_option", zclk_command_lua_flag_option},
    {"bool_argument", zclk_command_lua_bool_argument},
    {"int_argument", zclk_command_lua_int_argument},
    {"double_argument", zclk_command_lua_double_argument},
    {"string_argument", zclk_command_lua_string_argument},
    {"flag_argument", zclk_command_lua_flag_argument},
    {"get_option", zclk_command_lua_get_option},
    {"get_argument", zclk_command_lua_get_argument},
    {"subcommand", zclk_command_lua_subcommand_add},
    {NULL, NULL}
};

/* does not need gc, as the object is freed by corresponding parent cmd */
static const luaL_Reg zclk_option_meths[] =
{
    // {"__gc", zclk_option_free},
    {"value", zclk_option_value},
    {"type", zclk_option_type},
    {NULL, NULL}
};

/* does not need gc, as the object is freed by corresponding parent cmd */
static const luaL_Reg zclk_argument_meths[] =
{
    // {"__gc", zclk_argument_free},
    {"value", zclk_argument_value},
    {"type", zclk_argument_type},
    {NULL, NULL}
};

int luaopen_zclk(lua_State *L)
{
    // create zclk metatable
    luaL_newmetatable(L, LUA_ZCLK_COMMAND_OBJECT);

    // metatable.__index = metatable
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    // register methods
    luaL_setfuncs(L, zclk_command_meths, 0);

    // create zclk option metatable
    luaL_newmetatable(L, LUA_ZCLK_OPTION_OBJECT);

    // metatable.__index = metatable
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    // register methods
    luaL_setfuncs(L, zclk_option_meths, 0);

    // create zclk argument metatable
    luaL_newmetatable(L, LUA_ZCLK_ARGUMENT_OBJECT);

    // metatable.__index = metatable
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    // register methods
    luaL_setfuncs(L, zclk_argument_meths, 0);

    // register functions - only zclk.new
    luaL_newlib(L, ZclkCommand_funcs);

    return 1;
}
