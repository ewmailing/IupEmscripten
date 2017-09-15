#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

#include "BlurrrLua.h"

extern int luaopen_lpeg(lua_State *L);
void BlurrrLua_Init(lua_State* lua_state)
{
	// Load LPeg since we don't runtime load dll's with Emscripten
    luaL_requiref(lua_state, "lpeg", luaopen_lpeg, 1);
    lua_pop(lua_state, 1);  /* remove lib */
}

