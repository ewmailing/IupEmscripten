/** \file
 * \brief Button Control
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
#include "iup_button.h"
#include "iup_drv.h"
#include "iup_drvfont.h"
#include "iup_image.h"
#include "iup_key.h"

#include "iupemscripten_drv.h"




void iupdrvButtonAddBorders(int *x, int *y)
{

	
}

static int emscriptenButtonMapMethod(Ihandle* ih)
{
    JNIEnv* jni_env;
	jclass java_class;
    jmethodID method_id;
	jobject java_widget;
	char* attribute_value;
   
	jni_env = iupEmscripten_GetEnvThreadSafe();




	// TODO: Button and ImageButton are two different classes and we'll have to handle them separately.
	// emscripten.widget.Button
	// emscripten.widget.ImageButton
	attribute_value = iupAttribGet(ih, "IMAGE");
	if(attribute_value && *attribute_value!=0)
	{
		ih->data->type |= IUP_BUTTON_IMAGE;
	
		__emscripten_log_print(ANDROID_LOG_INFO, "emscriptenButtonMapMethod", "TODO: ImageButton not implemented"); 
		
	}
	else
	{

		java_class = (*jni_env)->FindClass(jni_env, "br/pucrio/tecgraf/iup/IupButtonHelper");
		method_id = (*jni_env)->GetStaticMethodID(jni_env, java_class, "createButton", "(J)Lemscripten/widget/Button;");
		java_widget = (*jni_env)->CallStaticObjectMethod(jni_env, java_class, method_id, (jlong)(intptr_t)ih);

		ih->handle = (jobject)((*jni_env)->NewGlobalRef(jni_env, java_widget));
			__emscripten_log_print(ANDROID_LOG_INFO, "emscriptenButtonMapMethod", "got button: %x", ih->handle); 

		(*jni_env)->DeleteLocalRef(jni_env, java_widget);
		(*jni_env)->DeleteLocalRef(jni_env, java_class);

	}

	// NOTE: ImageButton doesn't directly support text
	// NOTE: I set the Java properties (for TITLE) in createButton because it was easier to do there.
	// However, setting these flags is easier to do here in C.
	attribute_value = iupAttribGet(ih, "TITLE");
	if(attribute_value && *attribute_value!=0)
	{
		ih->data->type |= IUP_BUTTON_TEXT;
		/*
		if(ih->data->type & IUP_BUTTON_IMAGE)
		{
		}
		else
		{
		}
		*/

	}

	iupEmscripten_AddWidgetToParent(jni_env, ih);

	return IUP_NOERROR;	
}

static void emscriptenButtonUnMapMethod(Ihandle* ih)
{



	if(ih && ih->handle)
	{
    	JNIEnv* jni_env;
		jclass java_class;
		jmethodID method_id;
    	jni_env = iupEmscripten_GetEnvThreadSafe();

		java_class = (*jni_env)->FindClass(jni_env, "br/pucrio/tecgraf/iup/IupCommon");
    	method_id = (*jni_env)->GetStaticMethodID(jni_env, java_class, "removeWidgetFromParent", "(J)V");
    	(*jni_env)->CallStaticVoidMethod(jni_env, java_class, method_id, (jlong)(intptr_t)ih);
    	(*jni_env)->DeleteLocalRef(jni_env, java_class);


		(*jni_env)->DeleteGlobalRef(jni_env, ih->handle);
		ih->handle = NULL;
	}
}

void iupdrvButtonInitClass(Iclass* ic)
{
	/* Driver Dependent Class functions */
	ic->Map = emscriptenButtonMapMethod;
	ic->UnMap = emscriptenButtonUnMapMethod;
		__emscripten_log_print(ANDROID_LOG_INFO, "iupdrvButtonInitClass", "entered"); 
	
#if 0

	ic->LayoutUpdate = gtkButtonLayoutUpdateMethod;
	
	/* Driver Dependent Attribute functions */
	
	/* Overwrite Common */
	iupClassRegisterAttribute(ic, "STANDARDFONT", NULL, gtkButtonSetStandardFontAttrib, IUPAF_SAMEASSYSTEM, "DEFAULTFONT", IUPAF_NO_SAVE|IUPAF_NOT_MAPPED);
	
	/* Overwrite Visual */
	iupClassRegisterAttribute(ic, "ACTIVE", iupBaseGetActiveAttrib, gtkButtonSetActiveAttrib, IUPAF_SAMEASSYSTEM, "YES", IUPAF_DEFAULT);
	
	/* Visual */
	iupClassRegisterAttribute(ic, "BGCOLOR", NULL, gtkButtonSetBgColorAttrib, IUPAF_SAMEASSYSTEM, "DLGBGCOLOR", IUPAF_DEFAULT);
	
	/* Special */
	iupClassRegisterAttribute(ic, "FGCOLOR", NULL, gtkButtonSetFgColorAttrib, IUPAF_SAMEASSYSTEM, "DLGFGCOLOR", IUPAF_DEFAULT);
	iupClassRegisterAttribute(ic, "TITLE", NULL, gtkButtonSetTitleAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
	
	/* IupButton only */
	iupClassRegisterAttribute(ic, "ALIGNMENT", NULL, gtkButtonSetAlignmentAttrib, "ACENTER:ACENTER", NULL, IUPAF_NO_INHERIT);  /* force new default value */
	iupClassRegisterAttribute(ic, "IMAGE", NULL, gtkButtonSetImageAttrib, NULL, NULL, IUPAF_IHANDLENAME|IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
	iupClassRegisterAttribute(ic, "IMINACTIVE", NULL, gtkButtonSetImInactiveAttrib, NULL, NULL, IUPAF_IHANDLENAME|IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
	iupClassRegisterAttribute(ic, "IMPRESS", NULL, NULL, NULL, NULL, IUPAF_IHANDLENAME|IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
	
	iupClassRegisterAttribute(ic, "PADDING", iupButtonGetPaddingAttrib, gtkButtonSetPaddingAttrib, IUPAF_SAMEASSYSTEM, "0x0", IUPAF_NOT_MAPPED);
	iupClassRegisterAttribute(ic, "MARKUP", NULL, NULL, NULL, NULL, IUPAF_DEFAULT);
#endif
	
}
