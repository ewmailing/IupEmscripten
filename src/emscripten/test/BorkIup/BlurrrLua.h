#ifndef BLURRRLUA_H
#define BLURRRLUA_H

// This is a compat shim stopgap

#ifdef __cplusplus
#define extern "C" {
#endif
#include "lua.h"



void BlurrrLua_Init(lua_State* lua_state);

#ifdef __cplusplus
}
#endif

#endif

