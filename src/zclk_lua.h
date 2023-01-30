#ifndef __ZCLK_LUA_H__
#define __ZCLK_LUA_H__

#include "zclk_common.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

/* The Zclk command lua object */
#define LUA_ZCLK_COMMAND_OBJECT     "zclk_command"

/* The Zclk option lua object */
#define LUA_ZCLK_OPTION_OBJECT      "zclk_option"

/* The Zclk argument lua object */
#define LUA_ZCLK_ARGUMENT_OBJECT    "zclk_argument"

/* The lua module method for 'zclk' */
MODULE_API int luaopen_zclk(lua_State* L);

#endif //__ZCLK_LUA_H__
