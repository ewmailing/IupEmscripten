/** \file
 * \brief Emscripten Driver Core
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>          
#include <stdlib.h>
#include <string.h>          

#include "iup.h"

#include "iup_str.h"
#include "iup_drv.h"
#include "iup_drvinfo.h"
#include "iup_object.h"
#include "iup_globalattrib.h"

#include "iupemscripten_drv.h"


#if 0
char* iupEmscriptenGetNativeWindowHandle(Ihandle* ih)
{
  id window = ih->handle->window;
  if (window)
    return (char*)window;
  else
    return NULL;
}
#endif

void* iupdrvGetDisplay(void)
{
  return NULL;
}


void iupEmscriptenUpdateGlobalColors(void)
{
  iupGlobalSetDefaultColorAttrib("DLGBGCOLOR", 237,237,237);

  iupGlobalSetDefaultColorAttrib("DLGFGCOLOR", 0,0,0);

  iupGlobalSetDefaultColorAttrib("TXTBGCOLOR", 255,255,255);

  iupGlobalSetDefaultColorAttrib("TXTFGCOLOR", 0,0,0);

  iupGlobalSetDefaultColorAttrib("MENUBGCOLOR", 183,183,183);

  iupGlobalSetDefaultColorAttrib("MENUFGCOLOR", 0,0,0);
}

int iupdrvOpen(int *argc, char ***argv)
{                        
  (void)argc; /* unused in the Emscripten driver */
  (void)argv;

	// Assuming we're always on the main thread.

  // TODO: Use this to reinitialize static/global variables???
	iupEmscripten_InitializeInternalGlobals();
  
  IupSetGlobal("DRIVER", "Emscripten");

//  IupSetGlobal("SYSTEMLANGUAGE", iupEmscriptenGetSystemLanguage());

  iupEmscriptenUpdateGlobalColors();

  IupSetGlobal("_IUP_RESET_GLOBALCOLORS", "YES");  /* will update the global colors when the first dialog is mapped */

  return IUP_NOERROR;
}

void iupdrvClose(void)
{

	iupEmscripten_DestroyInternalGlobals();
	
}
