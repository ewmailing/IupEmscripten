/**
 * IupLabel module, C source
 * Please see accompanying JS file for external functions
 *
 * See Copyright Notice in "iup.h"
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdarg.h>

#include "iup.h"
#include "iupcbs.h"

#include "iup_object.h"
#include "iup_layout.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_image.h"
#include "iup_label.h"
#include "iup_drv.h"
#include "iup_image.h"
#include "iup_focus.h"

#include "iup_childtree.h"

#include "iupemscripten_drv.h"

#include "iup_key.h"
#include <emscripten.h>

extern int emjsLabel_CreateLabel(void);
extern void emjsLabel_SetTitle(int handle_id, const char* title);

extern void emjsLabel_CreateSeparator(int handle_id, char* type);
// this probably needs to return a char*
extern void emjsLabel_ToggleActive(int handle_id, int enable);
extern void emjsLabel_SetPadding(int handleID, int horiz, int vert);
extern void emjsLabel_SetFGColor(int handle_id, const char* color); /* should it be constant char*? */
extern void emjsLabel_SetBGColor(int handle_id, char* color); 
extern int emjsLabel_SetAlignmentAttrib(int handle_id, const char* value);
extern void emjsLabel_EnableEllipsis(Ihandle* ih);
extern void emjsLabel_DropFilesTarget(Ihandle* ih);

// adds padding to element
void iupdrvLabelAddBorders(Ihandle* ih, int *x, int *y)
{
  (void)ih;
  (void)x;
  (void)y;
}

static int emscriptenLabelSetTitleAttrib(Ihandle* ih, const char* value)
{
  // do we need this check? (it was in gtk)
  if (ih->data->type == IUP_LABEL_TEXT) {
    emjsLabel_SetTitle(ih->handle->handleID, value);
  }

	return 1;

}

static int emscriptenLabelSetPaddingAttrib(Ihandle* ih, const char* value)
{
  // it will try to set attributes before the widget is created - don't let it
  if (ih == NULL || ih->handle == NULL) {
    return 0;
  }
  else if (ih->handle->handleID == 0) {
    return 0;
  }
  else if (value == NULL) {
    value = "";
  }

  // split Iup's padding format (ex. "0x0") into ints
  iupStrToIntInt(value, &ih->data->horiz_padding, &ih->data->vert_padding, 'x');

  emjsLabel_SetPadding(ih->handle->handleID, ih->data->horiz_padding, ih->data->vert_padding);
  return 0;
}

static int emscriptenLabelSetFgColorAttrib(Ihandle* ih, const char* value)
{
  // call helper func from common; func will parse string and apply color to widget
  // can this be refactored into some other function? doesn't do much..
  iupdrvBaseSetFgColorAttrib(ih, value);

  return 1;
}

static void emscriptenLabelSetActiveAttrib(Ihandle* ih, int enable)
{
  emjsLabel_ToggleActive(ih->handle->handleID, enable);
}

static int emscriptenLabelSetBgColorAttrib(Ihandle* ih, const char* value)
{
  emjsLabel_SetBGColor(7, "red");
  return 1;
}

static int emscriptenLabelSetWordWrapAttrib(Ihandle* ih, const char* value)
{
  return 0;
}

// TODO: check args for this function (its a callback)
// TODO: does this need to support multi-line text?
static int emscriptenLabelSetEllipsisAttrib(Ihandle* ih, const char* value)
{
  // assuming I dont need value right now (that is the string in the label being shortened)
  emjsLabel_EnableEllipsis(ih->handle->handleID);

	// if (ih->data->type == IUP_LABEL_TEXT) returns 1, all else returns 0 ?ERIC
  return 0;
}

static int emscriptenSetDropFilesTargetAttrib(Ihandle* ih, const char* value)
{
  emjsLabel_DropFilesTarget(ih->handle->handleID);
}

static int emscriptenLabelSetAlignmentAttrib(Ihandle* ih, const char* value)
{
  emjsLabel_SetAlignmentAttrib(ih->handle->handleID, value);

  // label_text or label_image returns 1, everything else returns 0? ?ERIC
  return 0;
}

/* { */

/*   GtkWidget* eventbox = (GtkWidget*)iupAttribGet(ih, "_IUP_EXTRAPARENT"); */
/*   unsigned char r, g, b; */

/*   /1* ignore given value, must use only from parent for the scrollbars *1/ */
/*   char* parent_value = iupBaseNativeParentGetBgColor(ih); */

/*   if (iupStrToRGB(parent_value, &r, &g, &b)) */
/*     iupgtkSetBgColor(eventbox, r, g, b); */

/*   (void)value; */
/*   return iupdrvBaseSetBgColorAttrib(ih, parent_value); */
/* } */


static int emscriptenLabelMapMethod(Ihandle* ih)
{
  int label_id = 0;
  InativeHandle* new_handle = NULL;

  label_id = emjsLabel_CreateLabel();
  new_handle = (InativeHandle*)calloc(1, sizeof(InativeHandle));

  new_handle->handleID = label_id;
  ih->handle = new_handle;

  iupEmscripten_SetIntKeyForIhandleValue(label_id, ih);

  // Set text inside label (uses title attribute)
	char* attrib_title = iupAttribGet(ih, "TITLE");
	if(attrib_title && *attrib_title != 0)
	{
		ih->data->type |= IUP_LABEL_TEXT;
		/*
		if(ih->data->type & IUP_BUTTON_IMAGE)
		{
		}
		else
		{
		}
		*/

		emjsLabel_SetTitle(label_id, attrib_title);
	}
#if 0
	char* value;
	value = iupAttribGet(ih, "SEPARATOR");

	if (value)
	{
		if (iupStrEqualNoCase(value, "HORIZONTAL"))
		{
			ih->data->type = IUP_LABEL_SEP_HORIZ;

      // create horizontal separater in js
      emjsLabel_CreateSeparator("horizontal");
		}
		else /* "VERTICAL" */
		{
			ih->data->type = IUP_LABEL_SEP_VERT;

      ih->data->type = IUP_LABEL_SEP_VERT;
#if GTK_CHECK_VERSION(3, 0, 0)
      label = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
#else
      label = gtk_vseparator_new();
#endif
//			NSBox* vertical_separator=[[NSBox alloc] initWithFrame:NSMakeRect(20.0, 20.0, 1.0, 250.0)];
			NSBox* vertical_separator=[[NSBox alloc] initWithFrame:NSMakeRect(0.0, 0.0, 1.0, 250.0)];
			[vertical_separator setBoxType:NSBoxSeparator];
			the_label = vertical_separator;

		}
	}
	else
	{
    // does this mean the label needs to be created here? need to find where the label should be made
    label_id = emjsButton_CreateLabel();

		value = iupAttribGet(ih, "IMAGE");
		if (value)
		{
			ih->data->type = IUP_LABEL_IMAGE;
			
			char *name;
			int make_inactive = 0;
			
			if (iupdrvIsActive(ih))
    name = iupAttribGet(ih, "IMAGE");
			else
			{
    name = iupAttribGet(ih, "IMINACTIVE");
    if (!name)
	  {
		name = iupAttribGet(ih, "IMAGE");
		make_inactive = 1;
	  }
	}
			
			id the_bitmap;
			the_bitmap = iupImageGetImage(name, ih, make_inactive);
			int width;
			int height;
			int bpp;
			
			iupdrvImageGetInfo(the_bitmap, &width, &height, &bpp);

//			static int woffset = 0;
//			static int hoffset = 0;
			
			NSImageView* image_view = [[NSImageView alloc] initWithFrame:NSMakeRect(0, 0, width, height)];
//			NSImageView* image_view = [[NSImageView alloc] initWithFrame:NSMakeRect(woffset, hoffset, width, height)];
			[image_view setImage:the_bitmap];
			
//			woffset += 30;
//			hoffset += 30;
			
			the_label = image_view;
			
			if (!the_bitmap)
					return;
			
			/* must use this info, since image can be a driver image loaded from resources */
			iupdrvImageGetInfo(hBitmap, &width, &height, &bpp);
			
			NSBitmapImageRep* bitmap_image = [[NSBitmapImageRep alloc]
									 initWithBitmapDataPlanes:NULL
									 pixelsWide: width
									 pixelsHigh: height
									 bitsPerSample: 8
									 samplesPerPixel: 4
									 hasAlpha: YES
									 isPlanar: NO
									 colorSpaceName: NSCalibratedRGBColorSpace
									 bytesPerRow: width * 4
									 bitsPerPixel: 32]

		}
		else
		{
			ih->data->type = IUP_LABEL_TEXT;

			the_label = [[NSTextField alloc] initWithFrame:NSZeroRect];
//			the_label = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 100, 100)];

			[the_label setBezeled:NO];
			[the_label setDrawsBackground:NO];
			[the_label setEditable:NO];
//			[the_label setSelectable:NO];
			// TODO: FEATURE: I think this is really convenient for users so it should be the default
			[the_label setSelectable:YES];
			
			NSFont* the_font = [the_label font];
			NSLog(@"font %@", the_font);
		
		}
	}
	

	if (!the_label)
	{
		return IUP_ERROR;
	}
	
	
	ih->handle = the_label;

#endif
	
	/* add to the parent, all GTK controls must call this. */
//	iupgtkAddToParent(ih);
	
	
//	Ihandle* ih_parent = ih->parent;
//	id parent_native_handle = ih_parent->handle;
	
	iupEmscripten_AddWidgetToParent(ih);
	
	
	/* configure for DRAG&DROP of files */
	if (IupGetCallback(ih, "DROPFILES_CB"))
	{
		iupAttribSet(ih, "DROPFILESTARGET", "YES");
	}
	return IUP_NOERROR;
}


static void emscriptenLabelUnMapMethod(Ihandle* ih)
{
	/*
	id the_label = ih->handle;
	[the_label release];
	ih->handle = nil;
	*/

}

void iupdrvLabelInitClass(Iclass* ic)
{
  /* Driver Dependent Class functions */
  ic->Map = emscriptenLabelMapMethod;
//	ic->UnMap = emscriptenLabelUnMapMethod;


  /* Driver Dependent Attribute functions */
  iupClassRegisterAttribute(ic, "ACTIVE", iupBaseGetActiveAttrib, emscriptenLabelSetActiveAttrib, IUPAF_SAMEASSYSTEM, "YES", IUPAF_DEFAULT);

  iupClassRegisterAttribute(ic, "BGCOLOR", iupBaseNativeParentGetBgColorAttrib, emscriptenLabelSetBgColorAttrib, IUPAF_SAMEASSYSTEM, "DLGBGCOLOR", IUPAF_DEFAULT);

  /* iupClassRegisterAttribute(ic, "FGCOLOR", NULL, iupdrvBaseSetFgColorAttrib, IUPAF_SAMEASSYSTEM, "DLGFGCOLOR", IUPAF_DEFAULT); */

  iupClassRegisterAttribute(ic, "FGCOLOR", NULL, emscriptenLabelSetFgColorAttrib, IUPAF_SAMEASSYSTEM, "DLGFGCOLOR", IUPAF_DEFAULT);
  iupClassRegisterAttribute(ic, "PADDING", iupLabelGetPaddingAttrib, emscriptenLabelSetPaddingAttrib, IUPAF_SAMEASSYSTEM, "0x0", IUPAF_NOT_MAPPED);
  iupClassRegisterAttribute(ic, "TITLE", NULL, emscriptenLabelSetTitleAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "WORDWRAP", NULL, emscriptenLabelSetWordWrapAttrib, NULL, NULL, IUPAF_DEFAULT);

  // ERIC?: how does alignment work?
  iupClassRegisterAttribute(ic, "ALIGNMENT", NULL, emscriptenLabelSetAlignmentAttrib, "ALEFT:ACENTER", NULL, IUPAF_NO_INHERIT);  /* force new default value */

  iupClassRegisterAttribute(ic, "ELLIPSIS", NULL, emscriptenLabelSetEllipsisAttrib, NULL, NULL, IUPAF_DEFAULT);

  iupClassRegisterAttribute(ic, "DROPFILESTARGET", NULL, emscriptenSetDropFilesTargetAttrib, NULL, NULL, IUPAF_DEFAULT);
  // gtk defined in iup_dragdrop.c with IUPAF_NO_INHERIT as the last larg - which should be used?


  // ERIC?:
  // dropfilestarget
  // turn div/widget into area that files can be dropped onto to trigger a file upload dialog; as simple as possible
  
#if 0
  /* IupLabel only */
  // upon compile, image is compiled into binary format, possibly need special apis to read
  // emscripten should provide these apis
  iupClassRegisterAttribute(ic, "IMAGE", NULL, gtkLabelSetImageAttrib, NULL, NULL, IUPAF_IHANDLENAME|IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);

  /* IupLabel GTK and Motif only */
  iupClassRegisterAttribute(ic, "IMINACTIVE", NULL, gtkLabelSetImInactiveAttrib, NULL, NULL, IUPAF_IHANDLENAME|IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);

  /* IupLabel GTK only */
  iupClassRegisterAttribute(ic, "MARKUP", NULL, NULL, NULL, NULL, IUPAF_DEFAULT);
#endif
}
