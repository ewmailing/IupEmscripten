/** \file
 * \brief MAC Font mapping
 *
 * See Copyright Notice in "iup.h"
 */


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


#include "iup.h"

#include "iup_str.h"
#include "iup_array.h"
#include "iup_attrib.h"
#include "iup_object.h"
#include "iup_drv.h"
#include "iup_drvfont.h"
#include "iup_assert.h"

#include "iupemscripten_drv.h"

//#include "iupmac_info.h"

extern int emjsFont_GetStringWidth(int handleID, const char* str);
extern void emjsFont_GetMultiLineStringSize(int handleID, const char* str, int32_t* out_ptr_width, int32_t* out_ptr_height);
extern void emjsFont_GetCharSize(int handleID, int32_t* out_ptr_width, int32_t* out_ptr_height);

char* iupdrvGetSystemFont(void)
{
  static char systemfont[200] = "";
#if 0
  NSFont *font = [NSFont systemFontOfSize:0];
	NSLog(@"systemfont: %@", font);
  char *name = [[font familyName] UTF8String];
  if(*name)
    strcpy(systemfont,name);
  else
    strcpy(systemfont, "Tahoma, 10");
#endif
  return systemfont;
}


int iupdrvSetStandardFontAttrib(Ihandle* ih, const char* value)
{
#if 0 // iupBaseUpdateSizeFromFont missing
  ImacFont* macfont = macFontCreateNativeFont(ih, value);
  if (!macfont)
    return 1;

	/* If FONT is changed, must update the SIZE attribute */
	iupBaseUpdateAttribFromFont(ih);

  /* FONT attribute must be able to be set before mapping, 
      so the font is enable for size calculation. */
  if (ih->handle && (ih->iclass->nativetype != IUP_TYPEVOID)) {
	
  }
#endif
  return 1;
}

// three functions below are used to compute string size:

void iupdrvFontGetMultiLineStringSize(Ihandle* ih, const char* str, int *w, int *h)
{
	int32_t tmp_width = 0;
	int32_t tmp_height = 0;
	int handle_id = 0;

	if(ih && ih->handle)
	{
		handle_id = ih->handle->handleID;
	}


	emjsFont_GetMultiLineStringSize(handle_id, str, &tmp_width, &tmp_height);
	if(w)
	{
		*w = (int)tmp_width;
	}
	if(h)
	{ 
		*h = (int)tmp_height;
	}

	iupEmscripten_Log("iupdrvFontGetMultiLineStringSize being called.  Here's an arg: %s", str);
	return;
}

int iupdrvFontGetStringWidth(Ihandle* ih, const char* str)
{
	int handle_id = 0;
	if(ih && ih->handle)
	{
		handle_id = ih->handle->handleID;
	}
	return emjsFont_GetStringWidth(handle_id, str);
}

void iupdrvFontGetCharSize(Ihandle* ih, int *charwidth, int *charheight)
{
	int32_t tmp_width = 0;
	int32_t tmp_height = 0;
	int handle_id = 0;
	if(ih && ih->handle)
	{
		handle_id = ih->handle->handleID;
	}

	emjsFont_GetCharSize(handle_id, &tmp_width, &tmp_height);

	if(charwidth)
	{
		*charwidth = (int)tmp_width;
	}
	if(charheight)
	{ 
		*charheight = (int)tmp_height;
	}
}

void iupdrvFontInit(void)
{
}

void iupdrvFontFinish(void)
{
}

int iupdrvSetFontAttrib(Ihandle* ih, const char* value)
{
  return 1;
}
