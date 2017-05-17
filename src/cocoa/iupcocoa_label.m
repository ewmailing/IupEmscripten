/** \file
 * \brief Label Control
 *
 * See Copyright Notice in "iup.h"
 */

#include <Cocoa/Cocoa.h>

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

#include "iupcocoa_drv.h"

#import "IUPCocoaVerticalAlignmentTextFieldCell.h"


static int cocoaLabelSetPaddingAttrib(Ihandle* ih, const char* value)
{
	// Our Cocoa iupdrvbaseUpdateLayout contains a special case to handle padding. We just need to make sure the padding values get set here.
	// Other platforms seem to be skipping separators. We could theoretically support this since we are just manually computing offsets in iupdrvbaseUpdateLayout.
	if(ih->handle && ih->data->type != IUP_LABEL_SEP_HORIZ && ih->data->type != IUP_LABEL_SEP_VERT)
	{
		// I believe this sets the internal data structure values.
		iupStrToIntInt(value, &ih->data->horiz_padding, &ih->data->vert_padding, 'x');
		// HACK: I need to force a redraw. iupdrvbaseUpdateLayout queries the PADDING attribute, but it is not immediately set yet. So I'll force it to set now.
		iupAttribSetStr(ih, "PADDING", value);
		
		// Windows always calls iupdrvRedrawNow, and we need to too because the change won't update without it.
		// But this can require a new layout, so we need IupRefresh.
		IupRefresh(ih);
	}
	return 0;
}


static int cocoaLabelSetTitleAttrib(Ihandle* ih, const char* value)
{
	id the_label = ih->handle;
	if(the_label)
	{
		// This could be a NSTextField, some kind of image, or something else.
		
		if([the_label respondsToSelector:@selector(setStringValue:)])
		{
			NSString* ns_string = nil;
			if(value)
			{
				char* stripped_str = iupStrProcessMnemonic(value, NULL, 0);   /* remove & */

				// This will return nil if the string can't be converted.
				ns_string = [NSString stringWithUTF8String:stripped_str];
				
				if(stripped_str && stripped_str != value)
				{
					free(stripped_str);
				}
			}
			else
			{
				ns_string = @"";
			}
			// This will throw an exception for a nil string.
			[the_label setStringValue:ns_string];
			// I don't think I want sizeToFit because IUP sets a bunch of sizes, plus the wordwrap stuff might screw this up.
//			[the_label sizeToFit];
			
			// I think I need to call this. I noticed in another program, when I suddenly set a long string, it seems to use the prior layout. This forces a relayout.
			IupRefresh(ih);
			
		}
	}
	return 1;

}


static char* cocoaLabelGetTitleAttrib(Ihandle* ih)
{
	id the_label = ih->handle;
	if(the_label)
	{
		// This could be a NSTextField, some kind of image, or something else.
		
		if([the_label respondsToSelector:@selector(setStringValue:)])
		{
			NSString* ns_string = [the_label stringValue];
			if(ns_string)
			{
				return iupStrReturnStr([ns_string UTF8String]);
			}
		}
	}
	return NULL;
	
}

static int cocoaLabelSetAlignmentAttrib(Ihandle* ih, const char* value)
{
	if(ih->data->type != IUP_LABEL_SEP_HORIZ && ih->data->type != IUP_LABEL_SEP_VERT)
	{
		if(ih->data->type == IUP_LABEL_TEXT)
		{
			NSTextField* the_label = (NSTextField*)ih->handle;
			// Note: We might be able to get away with any kind of NSControl
			NSCAssert([the_label isKindOfClass:[NSTextField class]], @"Expected NSTextField");

			char value1[30], value2[30];
			
			iupStrToStrStr(value, value1, value2, ':');

			
			if (iupStrEqualNoCase(value1, "ARIGHT"))
			{
				[the_label setAlignment:NSTextAlignmentRight];
			}
			else if (iupStrEqualNoCase(value1, "ACENTER"))
			{
				[the_label setAlignment:NSTextAlignmentCenter];
			}
			else /* "ALEFT" */
			{
				[the_label setAlignment:NSTextAlignmentLeft];

			}
			
			
			// Vertical alignment is not built into NSTextField.
			// We implemented our own custom NSTextFieldCell subclass to handle this case.
			
			if (iupStrEqualNoCase(value2, "ABOTTOM"))
			{
				NSCAssert([[the_label cell] isKindOfClass:[IUPCocoaVerticalAlignmentTextFieldCell class]], @"Expected IUPCocoaVerticalAlignmentTextFieldCell");
				IUPCocoaVerticalAlignmentTextFieldCell* vertical_alignment_cell = (IUPCocoaVerticalAlignmentTextFieldCell*)[the_label cell];
				[vertical_alignment_cell setAlignmentMode:IUPTextVerticalAlignmentBottom];
				return 0;
			}
			else if (iupStrEqualNoCase(value2, "ATOP"))
			{
				NSCAssert([[the_label cell] isKindOfClass:[IUPCocoaVerticalAlignmentTextFieldCell class]], @"Expected IUPCocoaVerticalAlignmentTextFieldCell");
				IUPCocoaVerticalAlignmentTextFieldCell* vertical_alignment_cell = (IUPCocoaVerticalAlignmentTextFieldCell*)[the_label cell];
				[vertical_alignment_cell setAlignmentMode:IUPTextVerticalAlignmentTop];
				return 0;
			}
			else  /* ACENTER (default) */
			{
				NSCAssert([[the_label cell] isKindOfClass:[IUPCocoaVerticalAlignmentTextFieldCell class]], @"Expected IUPCocoaVerticalAlignmentTextFieldCell");
				IUPCocoaVerticalAlignmentTextFieldCell* vertical_alignment_cell = (IUPCocoaVerticalAlignmentTextFieldCell*)[the_label cell];
				[vertical_alignment_cell setAlignmentMode:IUPTextVerticalAlignmentCenter];
			}
			return 1;
		}
		else if(ih->data->type == IUP_LABEL_IMAGE)
		{
			NSImageView* the_label = (NSImageView*)ih->handle;
			// Note: We might be able to get away with any kind of NSControl
			NSCAssert([the_label isKindOfClass:[NSImageView class]], @"Expected NSImageView");
			
			char value1[30], value2[30];
			
			iupStrToStrStr(value, value1, value2, ':');
			
			
			if(iupStrEqualNoCase(value1, "ARIGHT"))
			{
				if(iupStrEqualNoCase(value2, "ABOTTOM"))
				{
					[the_label setImageAlignment:NSImageAlignBottomRight];
				}
				else if (iupStrEqualNoCase(value2, "ATOP"))
				{
					[the_label setImageAlignment:NSImageAlignTopRight];
				}
				else  /* ACENTER */
				{
					[the_label setImageAlignment:NSImageAlignRight];
				}
			}
			else if (iupStrEqualNoCase(value1, "ACENTER"))
			{
				if(iupStrEqualNoCase(value2, "ABOTTOM"))
				{
					[the_label setImageAlignment:NSImageAlignBottom];
				}
				else if (iupStrEqualNoCase(value2, "ATOP"))
				{
					[the_label setImageAlignment:NSImageAlignTop];
				}
				else  /* ACENTER */
				{
					[the_label setImageAlignment:NSImageAlignCenter];
				}
			}
			else /* "ALEFT" */
			{
				if(iupStrEqualNoCase(value2, "ABOTTOM"))
				{
					[the_label setImageAlignment:NSImageAlignBottomLeft];
				}
				else if (iupStrEqualNoCase(value2, "ATOP"))
				{
					[the_label setImageAlignment:NSImageAlignTopLeft];
				}
				else  /* ACENTER */
				{
					[the_label setImageAlignment:NSImageAlignLeft];
				}
			}
			
			
			return 1;
		}
	}
	
	return 0;
}

// Warning: The pre-10.10 behavior never behaved well. Maybe it should be removed.
static int cocoaLabelSetWordWrapAttrib(Ihandle* ih, const char* value)
{
	if (ih->data->type == IUP_LABEL_TEXT)
	{
		NSTextField* the_label = (NSTextField*)ih->handle;
		// Note: We might be able to get away with any kind of NSControl
		NSCAssert([the_label isKindOfClass:[NSTextField class]], @"Expected NSTextField");
		if(iupStrBoolean(value))
		{
			// setLineBreakMode Requires 10.10+. Allows for both word wrapping and different ellipsis behaviors.
			if([the_label respondsToSelector:@selector(setLineBreakMode:)])
			{
				[the_label setLineBreakMode:NSLineBreakByWordWrapping];
			}
			else
			{

				char* ellipsis_state = iupAttribGet(ih, "ELLIPSIS");
				if(iupStrBoolean(ellipsis_state))
				{
					// Ellipsis only seem to appear when multiline is enabled
					[the_label setUsesSingleLineMode:NO];
					[[the_label cell] setScrollable:NO];
					
					[[the_label cell] setWraps:YES];
					[[the_label cell] setLineBreakMode:NSLineBreakByTruncatingTail];
					[[the_label cell] setTruncatesLastVisibleLine:YES];
				}
				else
				{
					[the_label setUsesSingleLineMode:NO];
					[[the_label cell] setScrollable:NO];
					
					[[the_label cell] setWraps:YES];
					[[the_label cell] setLineBreakMode:NSLineBreakByWordWrapping];
					[[the_label cell] setTruncatesLastVisibleLine:NO];
				}
				
			}
			
		}
		else
		{
			// setLineBreakMode Requires 10.10+. Allows for both word wrapping and different ellipsis behaviors.
			if([the_label respondsToSelector:@selector(setLineBreakMode:)])
			{
				// Wrapping and ellipsis are mutually exclusive
				char* ellipsis_state = iupAttribGet(ih, "ELLIPSIS");
				if(iupStrBoolean(ellipsis_state))
				{
					[the_label setLineBreakMode:NSLineBreakByTruncatingTail];
				}
				else
				{
					[the_label setLineBreakMode:NSLineBreakByClipping];
				}
			}
			else
			{
				
				char* ellipsis_state = iupAttribGet(ih, "ELLIPSIS");
				if(iupStrBoolean(ellipsis_state))
				{
					// Ellipsis only seem to appear when multiline is enabled
					[the_label setUsesSingleLineMode:NO];
					[[the_label cell] setScrollable:NO];
					
					[[the_label cell] setWraps:YES];
					[[the_label cell] setLineBreakMode:NSLineBreakByWordWrapping];
					[[the_label cell] setTruncatesLastVisibleLine:YES];
				}
				else
				{
					[the_label setUsesSingleLineMode:YES];
					[[the_label cell] setScrollable:YES];
					
					[[the_label cell] setWraps:NO];
					[[the_label cell] setLineBreakMode:NSLineBreakByClipping];
					[[the_label cell] setTruncatesLastVisibleLine:NO];
				}
				
			}
		}
		return 1;
	}
	return 0;
}

// Warning: The pre-10.10 behavior never behaved well. Maybe it should be removed.
static int cocoaLabelSetEllipsisAttrib(Ihandle* ih, const char* value)
{
	if (ih->data->type == IUP_LABEL_TEXT)
	{
		NSTextField* the_label = (NSTextField*)ih->handle;
		// Note: We might be able to get away with any kind of NSControl
		NSCAssert([the_label isKindOfClass:[NSTextField class]], @"Expected NSTextField");



		if(iupStrBoolean(value))
		{
			// setLineBreakMode Requires 10.10+. Allows for both word wrapping and different ellipsis behaviors.
			if([the_label respondsToSelector:@selector(setLineBreakMode:)])
			{
				// Wrapping and ellipsis are mutually exclusive
				// TODO: Expose different ellipsis modes to public API
				[the_label setLineBreakMode:NSLineBreakByTruncatingTail];

			}
			else
			{
				// Ellipsis only seem to appear when multiline is enabled
				[the_label setUsesSingleLineMode:NO];
				[[the_label cell] setScrollable:NO];
				
				[[the_label cell] setWraps:YES];
				[[the_label cell] setLineBreakMode:NSLineBreakByWordWrapping];
				[[the_label cell] setTruncatesLastVisibleLine:YES];
				
			}
		}
		else
		{
			// setLineBreakMode Requires 10.10+. Allows for both word wrapping and different ellipsis behaviors.
			if([the_label respondsToSelector:@selector(setLineBreakMode:)])
			{
				// Wrapping and ellipsis are mutually exclusive

				char* wordwrap_state = iupAttribGet(ih, "WORDWRAP");
				if(iupStrBoolean(wordwrap_state))
				{
					[the_label setLineBreakMode:NSLineBreakByWordWrapping];
				}
				else
				{
					[the_label setLineBreakMode:NSLineBreakByClipping];
				}
				
			}
			else
			{
				
				char* wordwrap_state = iupAttribGet(ih, "WORDWRAP");
				if(iupStrBoolean(wordwrap_state))
				{
					[the_label setUsesSingleLineMode:NO];
					[[the_label cell] setScrollable:NO];
					
					[[the_label cell] setWraps:YES];
					[[the_label cell] setLineBreakMode:NSLineBreakByWordWrapping];
					[[the_label cell] setTruncatesLastVisibleLine:YES];
				}
				else
				{
					[the_label setUsesSingleLineMode:YES];
					[[the_label cell] setScrollable:YES];
					
					[[the_label cell] setWraps:NO];
					[[the_label cell] setLineBreakMode:NSLineBreakByClipping];
					[[the_label cell] setTruncatesLastVisibleLine:NO];
				}
								
			}

		
		}
		return 1;
	}
	return 0;
}


static int cocoaLabelMapMethod(Ihandle* ih)
{
	char* value;
	// using id because we may be using different types depending on the case
	id the_label = nil;
	
	value = iupAttribGet(ih, "SEPARATOR");
	if (value)
	{
		if (iupStrEqualNoCase(value, "HORIZONTAL"))
		{
			ih->data->type = IUP_LABEL_SEP_HORIZ;

//			NSBox* horizontal_separator= [[NSBox alloc] initWithFrame:NSMakeRect(20.0, 20.0, 250.0, 1.0)];
			NSBox* horizontal_separator= [[NSBox alloc] initWithFrame:NSMakeRect(0.0, 0.0, 250.0, 1.0)];
			[horizontal_separator setBoxType:NSBoxSeparator];
			the_label = horizontal_separator;
			
		}
		else /* "VERTICAL" */
		{
			ih->data->type = IUP_LABEL_SEP_VERT;

//			NSBox* vertical_separator=[[NSBox alloc] initWithFrame:NSMakeRect(20.0, 20.0, 1.0, 250.0)];
			NSBox* vertical_separator=[[NSBox alloc] initWithFrame:NSMakeRect(0.0, 0.0, 1.0, 250.0)];
			[vertical_separator setBoxType:NSBoxSeparator];
			the_label = vertical_separator;

		}
	}
	else
	{
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
			
#if 0
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
#endif

		}
		else
		{
			ih->data->type = IUP_LABEL_TEXT;

			the_label = [[NSTextField alloc] initWithFrame:NSZeroRect];
//			the_label = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, 100, 100)];

			
			IUPCocoaVerticalAlignmentTextFieldCell* textfield_cell = [[IUPCocoaVerticalAlignmentTextFieldCell alloc] initTextCell:@"textfield"];
			[the_label setCell:textfield_cell];
			[textfield_cell release];
			
			
			
			[the_label setBezeled:NO];
			[the_label setDrawsBackground:NO];
//			[the_label setDrawsBackground:YES]; // sometimes helpful for debugging layout issues
			[the_label setEditable:NO];
//			[the_label setSelectable:NO];
			// TODO: FEATURE: I think this is really convenient for users so it should be the default
			[the_label setSelectable:YES];
			
//			NSFont* the_font = [the_label font];
//			NSLog(@"font %@", the_font);
			[the_label setFont:[NSFont systemFontOfSize:0.0]];

			
			
#if 1
			if([the_label respondsToSelector:@selector(setLineBreakMode:)])
			{
				[the_label setLineBreakMode:NSLineBreakByClipping];
				
			}
			else
			{
				
				[[the_label cell] setTruncatesLastVisibleLine:NO];


				
				
				[the_label setUsesSingleLineMode:YES];
				[[the_label cell] setScrollable:YES];
				
				[[the_label cell] setWraps:NO];
				[[the_label cell] setLineBreakMode:NSLineBreakByClipping];
				[[the_label cell] setTruncatesLastVisibleLine:NO];
				
			}
			
			
	
#else
			
			
			[the_label setUsesSingleLineMode:NO];
			[[the_label cell] setWraps:YES];
			[[the_label cell] setScrollable:NO];
			
//			[[the_label cell] setTruncatesLastVisibleLine:YES];

			// setLineBreakMode Requires 10.10+. Allows for both word wrapping and different ellipsis behaviors.
//			[the_label setLineBreakMode:NSLineBreakByWordWrapping];
#endif
			
		}
	}
	
	if (!the_label)
	{
		return IUP_ERROR;
	}
	
	
	ih->handle = the_label;

	
	
	/* add to the parent, all GTK controls must call this. */
//	iupgtkAddToParent(ih);
	
	
//	Ihandle* ih_parent = ih->parent;
//	id parent_native_handle = ih_parent->handle;
	
	iupCocoaAddToParent(ih);
	
	
	/* configure for DRAG&DROP of files */
	if (IupGetCallback(ih, "DROPFILES_CB"))
	{
		iupAttribSet(ih, "DROPFILESTARGET", "YES");
	}
	
	return IUP_NOERROR;
}


static void cocoaLabelUnMapMethod(Ihandle* ih)
{
	id the_label = ih->handle;
	iupCocoaRemoveFromParent(ih);
	[the_label release];
	ih->handle = nil;

}


void iupdrvLabelInitClass(Iclass* ic)
{
  /* Driver Dependent Class functions */
  ic->Map = cocoaLabelMapMethod;
	ic->UnMap = cocoaLabelUnMapMethod;
	

#if 0

  /* Driver Dependent Attribute functions */

  /* Overwrite Visual */
  iupClassRegisterAttribute(ic, "ACTIVE", iupBaseGetActiveAttrib, gtkLabelSetActiveAttrib, IUPAF_SAMEASSYSTEM, "YES", IUPAF_DEFAULT);

  /* Visual */
  iupClassRegisterAttribute(ic, "BGCOLOR", iupBaseNativeParentGetBgColorAttrib, gtkLabelSetBgColorAttrib, IUPAF_SAMEASSYSTEM, "DLGBGCOLOR", IUPAF_DEFAULT);

  /* Special */
  iupClassRegisterAttribute(ic, "FGCOLOR", NULL, iupdrvBaseSetFgColorAttrib, IUPAF_SAMEASSYSTEM, "DLGFGCOLOR", IUPAF_DEFAULT);
	
#endif
	
  iupClassRegisterAttribute(ic, "TITLE", cocoaLabelGetTitleAttrib, cocoaLabelSetTitleAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  /* IupLabel only */
  iupClassRegisterAttribute(ic, "ALIGNMENT", NULL, cocoaLabelSetAlignmentAttrib, "ALEFT:ACENTER", NULL, IUPAF_NO_INHERIT);  /* force new default value */
#if 0
  iupClassRegisterAttribute(ic, "IMAGE", NULL, gtkLabelSetImageAttrib, NULL, NULL, IUPAF_IHANDLENAME|IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
#endif
  iupClassRegisterAttribute(ic, "PADDING", iupLabelGetPaddingAttrib, cocoaLabelSetPaddingAttrib, IUPAF_SAMEASSYSTEM, "0x0", IUPAF_NOT_MAPPED);
#if 0
  /* IupLabel GTK and Motif only */
  iupClassRegisterAttribute(ic, "IMINACTIVE", NULL, gtkLabelSetImInactiveAttrib, NULL, NULL, IUPAF_IHANDLENAME|IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
#endif
	
  /* IupLabel Windows and GTK only */
  iupClassRegisterAttribute(ic, "WORDWRAP", NULL, cocoaLabelSetWordWrapAttrib, NULL, NULL, IUPAF_DEFAULT);
  iupClassRegisterAttribute(ic, "ELLIPSIS", NULL, cocoaLabelSetEllipsisAttrib, NULL, NULL, IUPAF_DEFAULT);

#if 0
  /* IupLabel GTK only */
  iupClassRegisterAttribute(ic, "MARKUP", NULL, NULL, NULL, NULL, IUPAF_DEFAULT);
#endif
}
