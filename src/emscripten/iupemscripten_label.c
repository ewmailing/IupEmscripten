/** \file
 * \brief Label Control
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

static int emscriptenLabelSetTitleAttrib(Ihandle* ih, const char* value)
{
/*
	id the_label = ih->handle;
	if(the_label)
	{

	}
*/
	return 1;

}

extern int emjsLabel_CreateLabel(void);
extern void emjsLabel_SetTitle(int handle_id, const char* title);

extern void emjsLabel_CreateSeparator(int handle_id, char* type);

extern void emjsLabel_SetFGColor(int handle_id, char* color); /* should it be constant char*? */
extern int emjsLabel_SetAlignmentAttrib(int handle_id, const char* value);

#if 0
static int emscriptenLabelSetAlignmentAttrib(Ihandle* ih, const char* value)
{
  if (ih->data->type != IUP_LABEL_SEP_HORIZ && ih->data->type != IUP_LABEL_SEP_VERT)
  {
    GtkMisc* misc = (GtkMisc*)ih->handle;
    PangoAlignment alignment;
    float xalign, yalign;
    char value1[30], value2[30];

    iupStrToStrStr(value, value1, value2, ':');

    if (iupStrEqualNoCase(value1, "ARIGHT"))
    {
      xalign = 1.0f;
      alignment = PANGO_ALIGN_RIGHT;
    }
    else if (iupStrEqualNoCase(value1, "ACENTER"))
    {
      xalign = 0.5f;
      alignment = PANGO_ALIGN_CENTER;
    }
    else /* "ALEFT" */
    {
      xalign = 0;
      alignment = PANGO_ALIGN_LEFT;
    }

    if (iupStrEqualNoCase(value2, "ABOTTOM"))
      yalign = 1.0f;
    else if (iupStrEqualNoCase(value2, "ATOP"))
      yalign = 0;
    else  /* ACENTER (default) */
      yalign = 0.5f;

    gtk_misc_set_alignment(misc, xalign, yalign);

    if (ih->data->type == IUP_LABEL_TEXT)
      pango_layout_set_alignment(gtk_label_get_layout((GtkLabel*)ih->handle), alignment);

    return 1;
  }
  else
    return 0;
}
#endif

static int emscriptenLabelMapMethod(Ihandle* ih)
{
  
	// using id because we may be using different types depending on the case
	//id the_label = NULL;
  //
  int label_id = 0;
  InativeHandle* new_handle = NULL;

  label_id = emjsLabel_CreateLabel();
  new_handle = (InativeHandle*)calloc(1, sizeof(InativeHandle));

  new_handle->handleID = label_id;
  ih->handle = new_handle;

  iupEmscripten_SetIntKeyForIhandleValue(label_id, ih);

  // Set text inside label (uses title attribute)
	char* attribute_value = iupAttribGet(ih, "TITLE");
	if(attribute_value && *attribute_value != 0)
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

		emjsLabel_SetTitle(label_id, attribute_value);
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

#if 0

  /* Driver Dependent Attribute functions */

  /* Overwrite Visual */
  iupClassRegisterAttribute(ic, "ACTIVE", iupBaseGetActiveAttrib, gtkLabelSetActiveAttrib, IUPAF_SAMEASSYSTEM, "YES", IUPAF_DEFAULT);

  /* Visual */
  iupClassRegisterAttribute(ic, "BGCOLOR", iupBaseNativeParentGetBgColorAttrib, gtkLabelSetBgColorAttrib, IUPAF_SAMEASSYSTEM, "DLGBGCOLOR", IUPAF_DEFAULT);

  /* Special */
  iupClassRegisterAttribute(ic, "FGCOLOR", NULL, iupdrvBaseSetFgColorAttrib, IUPAF_SAMEASSYSTEM, "DLGFGCOLOR", IUPAF_DEFAULT);
	
#endif
	
//  iupClassRegisterAttribute(ic, "TITLE", NULL, emscriptenLabelSetTitleAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);


#if 0
  /* IupLabel only */
  iupClassRegisterAttribute(ic, "ALIGNMENT", NULL, gtkLabelSetAlignmentAttrib, "ALEFT:ACENTER", NULL, IUPAF_NO_INHERIT);  /* force new default value */
  iupClassRegisterAttribute(ic, "IMAGE", NULL, gtkLabelSetImageAttrib, NULL, NULL, IUPAF_IHANDLENAME|IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "PADDING", iupLabelGetPaddingAttrib, gtkLabelSetPaddingAttrib, IUPAF_SAMEASSYSTEM, "0x0", IUPAF_NOT_MAPPED);

  /* IupLabel GTK and Motif only */
  iupClassRegisterAttribute(ic, "IMINACTIVE", NULL, gtkLabelSetImInactiveAttrib, NULL, NULL, IUPAF_IHANDLENAME|IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);

  /* IupLabel Windows and GTK only */
  iupClassRegisterAttribute(ic, "WORDWRAP", NULL, gtkLabelSetWordWrapAttrib, NULL, NULL, IUPAF_DEFAULT);
  iupClassRegisterAttribute(ic, "ELLIPSIS", NULL, gtkLabelSetEllipsisAttrib, NULL, NULL, IUPAF_DEFAULT);

  /* IupLabel GTK only */
  iupClassRegisterAttribute(ic, "MARKUP", NULL, NULL, NULL, NULL, IUPAF_DEFAULT);
#endif
}
