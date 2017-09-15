#ifndef BLURRRCORE_H
#define BLURRRCORE_H

// This is a compat shim stopgap

#ifdef __cplusplus
#define extern "C" {
#endif

#include <string.h>
#include <stddef.h>
#include <stdbool.h>
#include "lua.h"

#define BlurrrStdlib_strlcat strlcat

	
void BlurrrLog_SysLog(const char* fmt, ...);

bool BlurrrCore_Init(void);

void BlurrrCore_Quit(void);

void BlurrrLua_Init(lua_State* lua_state);

struct BlurrrFileHandle;
typedef struct BlurrrFileHandle BlurrrFileHandle;

struct BlurrrFileHandle* BlurrrFile_Open(const char* file_name, const char* mode);
size_t BlurrrFile_Read(struct BlurrrFileHandle* file_handle, char* data_buffer, size_t size, size_t nitems);
int BlurrrFile_Close(struct BlurrrFileHandle* file_handle);
size_t BlurrrFile_Size(struct BlurrrFileHandle* file_handle);

void BlurrrPath_GetResourceDirectoryString(char* out_buffer, size_t max_buf_len);

#ifdef __cplusplus
}
#endif

#endif

