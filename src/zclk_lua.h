#ifndef __ZCLK_LUA_H__
#define __ZCLK_LUA_H__

#include "zclk_common.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#define LUA_ZCLK_COMMAND_OBJECT "zclk_command"

MODULE_API int luaopen_zclk(lua_State* L);

#endif //__ZCLK_LUA_H__
