/*
The purpose of this file is to start up the program and get into Lua.
Most of the time, you will not need to touch this file.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "iup.h"
#include "iupcbs.h"

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"


void DoStartupOnlyOnce(void);
void IupEntryPoint(void);

// Tricky: Do not #include "SDL.h" before declaring int main. SDL.h does some macro magic to hijack the definition on iOS to make SDL's booting go normally. But this will break normal use cases, like with IUP.
// Do not modify main(). Not all platforms use main as the entry point so your changes may have no effect.
int main(int argc, char* argv[])
{
	// Remember: Not all platforms run main.
	DoStartupOnlyOnce();
	IupOpen(&argc, &argv); // removed because IupLua is presumably doing this.
	IupSetFunction("ENTRY_POINT", (Icallback)IupEntryPoint);
	IupMainLoop();
	return 0;
}


#include "BlurrrCore.h"
#include "BlurrrLua.h"

#include "SpeechSynth.h"


#define MAX_FILE_STRING_LENGTH 2048
#define MAX_PHRASE_STRING_LENGTH 10000


/* These functions are provided by the various libraries, but don't provide header files.
	So we just declare their interfaces here as any header file would do.
*/
//extern int luaopen_SDL(lua_State* L); /* default Lua action */
//extern int luaopen_BlurrrCore(lua_State* L); /* default Lua action */

//extern int iuplua_open(lua_State *L);
//extern int iupweblua_open(lua_State * L);

static _Bool s_isInit = 0;

struct UIWidgets
{
	Ihandle* textViewEntry;
	Ihandle* textViewResult;
	Ihandle* buttonSubmit;
	Ihandle* mainDialog;
};
struct GlobalResources
{
	struct UIWidgets uiWidgets;
	struct lua_State* luaState;
};

struct GlobalResources* g_globalResources = NULL;



static int report(lua_State* L, int status)
{
	const char* msg;
	if(status)
	{
		msg = lua_tostring(L, -1);
		if (msg == NULL) msg = "(error with no message)";
		BlurrrLog_SysLog("status=%d, %s\n", status, msg);
		lua_pop(L, 1);
	}
	return status;
}


void CallLuaVoidFunction(struct lua_State* lua_state, const char* function_name)
{
	lua_getglobal(lua_state, function_name);
	int ret_flag = lua_pcall(lua_state, 0, 0, 0);
	if(0 != ret_flag)
	{
		report(lua_state, ret_flag);
	}
	else
	{
		// nothing to lua_pop with void function
	}
}

void LoadLuaScript()
{
	BlurrrLog_SysLog("LoadLuaScript start");

	struct lua_State* lua_state;
	int ret_flag;
	char lua_file_path[MAX_FILE_STRING_LENGTH];
	struct BlurrrFileHandle* rw_ops;
	
		BlurrrLog_SysLog("luaL_newstate start");

    // Override point for customization after application launch.
	lua_state = luaL_newstate();
		BlurrrLog_SysLog("luaL_openlibs start");
	luaL_openlibs(lua_state);
	
		BlurrrLog_SysLog("BlurrrLua_Init start");

	BlurrrLua_Init(lua_state);

		BlurrrLog_SysLog("BlurrrLua_Init end");

//	luaopen_SDL(lua_state);
//	luaopen_BlurrrCore(lua_state);
//	iuplua_open(lua_state);
//	iupweblua_open(lua_state);
	/*
	luaopen_ALmixer(lua_state);
*/
//	luaopen_lpeg(lua_state);
	BlurrrLog_SysLog("BlurrrPath_GetResourceDirectoryString");


	lua_file_path[0] = '\0';
	BlurrrPath_GetResourceDirectoryString(lua_file_path, MAX_FILE_STRING_LENGTH);
	BlurrrStdlib_strlcat(lua_file_path, "main.lua", MAX_FILE_STRING_LENGTH);

	BlurrrLog_SysLog("BlurrrPath_GetResourceDirectoryString lua_file_path %s", lua_file_path);

	BlurrrLog_SysLog("Open File");

	// We use SDL to open/read the file because on Android, it is inside the .apk (zip), so standard file functions won't work.
	rw_ops = BlurrrFile_Open(lua_file_path, "r");
	if(NULL != rw_ops)
	{
	BlurrrLog_SysLog("BlurrrPath_GetResourceDirectoryString rw_ops");
		int64_t file_size;
		file_size = BlurrrFile_Size(rw_ops);
		if(file_size > 0)
		{
			char* file_data_buffer;
			size_t bytes_read;
			file_data_buffer = (char*)calloc(file_size, sizeof(char));
			bytes_read = BlurrrFile_Read(rw_ops, file_data_buffer, 1, file_size);

			ret_flag = luaL_loadbuffer(lua_state, file_data_buffer, file_size, lua_file_path);
			if(0 != ret_flag)
			{
				report(lua_state, ret_flag);
				
			}
			else
			{
				ret_flag = lua_pcall(lua_state, 0, 0, 0);
				if(0 != ret_flag)
				{
					report(lua_state, ret_flag);
				}
			}
			free(file_data_buffer);
		}
		else
		{
			BlurrrLog_SysLog("Error: main.lua had a file size <= 0: %zu", file_size);
		}
	}
	else
	{
		BlurrrLog_SysLog("Error: Could not open/find main.lua");
		
	}
	BlurrrFile_Close(rw_ops);
	
	
	
	g_globalResources->luaState = lua_state;

	
}


void BorkifyString(lua_State* lua_state, const char* input_string, char result_string[], size_t max_string_len)
{
	if(result_string == NULL)
	{
		return;
	}
	if(max_string_len < 1)
	{
		return;
	}
	result_string[0] = '\0';
	
//	strlcpy(result_string, "velcome to zee wunderfol world of zee cheef", max_string_len);


	lua_getglobal(lua_state, "TranslateSynth");
	lua_pushstring(lua_state, input_string);
	int ret_flag = lua_pcall(lua_state, 1, 1, 0);
	if(0 != ret_flag)
	{
		report(lua_state, ret_flag);
	}
	else
	{
		const char* translated_string = lua_tostring(lua_state, -1);
		// strlcpy isn't available on Visual Studio and not by default on Ubuntu gcc (need libbsd-dev)
		strncpy(result_string, translated_string, MAX_PHRASE_STRING_LENGTH);
		lua_pop(lua_state, -1);
	}

}

static void OnSpeechFinished()
{
	// WARNING: Extremely dangerous because this is not guaranteed to callback on the main UI thread.
	Ihandle* submit_button = g_globalResources->uiWidgets.buttonSubmit;
	IupSetAttribute(submit_button, "ACTIVE", "YES");
	
}

static int OnSubmitButtonPressed(Ihandle* submit_button)
{
	char result_string[MAX_PHRASE_STRING_LENGTH];
	
	Ihandle* text_view_entry = g_globalResources->uiWidgets.textViewEntry;
	Ihandle* text_view_result = g_globalResources->uiWidgets.textViewResult;
	lua_State* lua_state = g_globalResources->luaState;
	
	char* entry_string = IupGetAttribute(text_view_entry, "VALUE");
	if(NULL == entry_string)
	{
		IupSetStrAttribute(text_view_result, "VALUE", "");
		return IUP_DEFAULT;
	}


	BorkifyString(lua_state, entry_string, result_string, MAX_PHRASE_STRING_LENGTH);
	
	IupSetStrAttribute(text_view_result, "VALUE", result_string);
	
	// Avoid sending an empty string to the speech synth. They may not give a callback in that case and we will be left with a stuck disabled button.
	
	if(strlen(result_string) > 0)
	{
		// disable the button until done
		IupSetAttribute(submit_button, "ACTIVE", "NO");
		
		Speech_SayString(result_string);
	}
	
	return IUP_DEFAULT;
}

static void CreateGUI()
{
BlurrrLog_SysLog("calling CreateGUI");

	Ihandle* text_view_entry = IupMultiLine(NULL);

	IupSetAttribute(text_view_entry, "VISIBLECOLUMNS", "40");
	IupSetAttribute(text_view_entry, "VISIBLELINES", "10");
	IupSetAttribute(text_view_entry, "EXPAND", "HORIZONTAL");

	IupSetStrAttribute(text_view_entry, "VALUE", "Welcome to the wonderful world of the Swedish Chef!\n");

	
	Ihandle* text_view_result = IupMultiLine(NULL);
//	IupSetAttribute(text_view_result, "SIZE", "500x100");
	IupSetAttribute(text_view_result, "VISIBLECOLUMNS", "40");
	IupSetAttribute(text_view_result, "VISIBLELINES", "10");
	IupSetAttribute(text_view_result, "EXPAND", "HORIZONTAL");
	IupSetAttribute(text_view_result, "READONLY", "YES");
	
	Ihandle* submit_button = IupButton("Bork-ify!", NULL);
	IupSetCallback(submit_button, "ACTION", (Icallback)OnSubmitButtonPressed);
	IupSetAttribute(text_view_result, "ALIGNMENT", "RIGHT");

	
	
	Ihandle* vbox = IupVbox(text_view_entry, text_view_result, submit_button, NULL);
	
	Ihandle* the_dialog = IupDialog(vbox);
	// For Microsoft Windows: Set the top-left application icon
	IupSetAttribute(the_dialog, "ICON", "IDI_ICON1");
BlurrrLog_SysLog("Before IupShow");


	IupShow(the_dialog);
	
	
	g_globalResources->uiWidgets.textViewEntry = text_view_entry;
	g_globalResources->uiWidgets.textViewResult = text_view_result;
	g_globalResources->uiWidgets.buttonSubmit = submit_button;
	g_globalResources->uiWidgets.mainDialog = the_dialog;
	
BlurrrLog_SysLog("end of CreateGUI");

}


static void DestroyGlobals()
{
	free(g_globalResources);
	g_globalResources = NULL;
}



// This function is called when the program exits.
static void IupExitPoint()
{
	CallLuaVoidFunction(g_globalResources->luaState, "IupExitPoint");

//	iuplua_close(g_globalResources->luaState);
	lua_close(g_globalResources->luaState);
	g_globalResources->luaState = NULL;
//	IupClose();
	s_isInit = 0;
	
	DestroyGlobals();
	Speech_Quit();
	BlurrrCore_Quit();
}

// IupLua has a strange startup sequence that is causing problems with the next-gen Iup backends because all of them require deferring to the OS's native event loop.
// IupLua seems to check to see if IupOpen is called outside of Lua first or in Lua and will do different things.
// When called outside of Lua, it skips certain registration operations which is breaking us.
// iuplua_opencall_internal in particular returns false in this case, which skips calling IupWebBrowserOpen in iupweb and breaks things.
// Unfortunately, because we must defer to the OS native loop for the new platforms,
// we are relying on the new IupEntryPoint behavior which means we must start in C and use Iup.
// The workaroud seems to be to immediately start Lua and call require() to get things going.
// Because the next-gen startup is not guaranteed to use main as the sole entry point, we must take care to not do our initialization twice.
// See _IUPOPEN_CALL and EXTERNAL and INTERNAL and iuplua_opencall_internal and iupweblua_open
void DoStartupOnlyOnce()
{
BlurrrLog_SysLog("DoStartupOnlyOnce");
	if(s_isInit == 1)
	{
		return;
	}
	
	BlurrrCore_Init();
	Speech_Init();
	Speech_SetFinishedCallback(OnSpeechFinished);
	g_globalResources = calloc(1, sizeof(struct GlobalResources));
BlurrrLog_SysLog("calling LoadLuaScript");

	LoadLuaScript();
	BlurrrLog_SysLog("finished LoadLuaScript");


	s_isInit = 1;
}

void IupEntryPoint()
{
	// most platforms have started in main(), but not all of them.
	DoStartupOnlyOnce();

	// This tells IUP to call IupExitPoint on exit.
	IupSetFunction("EXIT_CB", (Icallback)IupExitPoint);
	IupSetInt(NULL, "UTF8MODE", 1);

	CreateGUI();
}

