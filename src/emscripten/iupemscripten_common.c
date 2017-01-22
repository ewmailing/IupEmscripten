/** \file
 * \brief Emscripten Base Functions
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>              
#include <stdlib.h>
#include <string.h>             
#include <limits.h>             

#include "iup.h"
#include "iupcbs.h"
#include "iupkey.h"

#include "iup_object.h"
#include "iup_childtree.h"
#include "iup_key.h"
#include "iup_str.h"
#include "iup_class.h"
#include "iup_attrib.h"
#include "iup_focus.h"
#include "iup_key.h"
#include "iup_image.h"
#include "iup_drv.h"

#include "iupemscripten_drv.h"

#include <emscripten/log.h>
#include <jni.h>
#include <pthread.h>



static int s_isInitialized = 0;

static JavaVM* s_javaVM = NULL;

// This is intended to be the God Context object you need for a lot of Emscripten APIs (e.g. AssetManager).
// It uses the Application context because that supposedly will be available for the life of the program.
static jobject s_applicationContextObject = NULL;
static pthread_key_t s_attachThreadKey;


void iupEmscripten_ThreadDestroyed(void* user_data)
{
	/* The thread is being destroyed, detach it from the Java VM and set the s_attachThreadKey value to NULL as required */
	JNIEnv* jni_env = (JNIEnv*)user_data;
	if(NULL != jni_env)
	{
		(*s_javaVM)->DetachCurrentThread(s_javaVM);
		pthread_setspecific(s_attachThreadKey, NULL);
	}
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* java_vm, void* reserved)
{
    s_javaVM = java_vm;
	/* SDL notes:
	 * Create s_attachThreadKey so we can keep track of the JNIEnv assigned to each thread
     * Refer to http://developer.emscripten.com/guide/practices/design/jni.html for the rationale behind this
     */
    if(pthread_key_create(&s_attachThreadKey, iupEmscripten_ThreadDestroyed) != 0)
	{
		__emscripten_log_print(ANDROID_LOG_ERROR, "Iup", "Error initializing pthread key");
    }

    return JNI_VERSION_1_6;
}

JNIEnv* iupEmscripten_GetEnvThreadSafe()
{
	JNIEnv* jni_env;
	
	/* ALmixer Notes: */
	/* Careful: If ALmixer is compiled with threads, make sure any calls back into Java are thread safe. */
	/* Unfortunately, JNI is making thread handling even more complicated than usual.
	 * If ALmixer is compiled with threads, it invokes callbacks on a ALmixer private background thread.
	 * In this case, we are required to call AttachCurrentThread for Java.
	 * However, there is a case in ALmixer where the callback doesn't happen on the background thread, but the calling thread.
	 * Calling ALmixer_HaltChannel() will trigger the callback on immediately on the thread you called the function on.
	 * (In this program, it is the main thread.)
	 * But JNI will break and crash if you try calling AttachCurrentThread in this case.
	 * So we need to know what thread we are on. If we are on the background thread, we must call AttachCurrentThread.
	 * Otherwise, we need to avoid calling it and use the current "env".
	 */

	/* There is a little JNI dance you can do to deal with this situation which is shown here.
	*/
	int get_env_stat = (*s_javaVM)->GetEnv(s_javaVM, (void**)&jni_env, JNI_VERSION_1_6);
	if(get_env_stat == JNI_EDETACHED)
	{
		jint attach_status = (*s_javaVM)->AttachCurrentThread(s_javaVM, &jni_env, NULL);
		if(0 != attach_status)
		{
			__emscripten_log_print(ANDROID_LOG_ERROR, "Iup", "AttachCurrentThread failed"); 
		}

		/* SDL notes: */
		/* From http://developer.emscripten.com/guide/practices/jni.html
		 * Threads attached through JNI must call DetachCurrentThread before they exit. If coding this directly is awkward,
		 * in Emscripten 2.0 (Eclair) and higher you can use pthread_key_create to define a destructor function that will be
		 * called before the thread exits, and call DetachCurrentThread from there. (Use that key with pthread_setspecific
		 * to store the JNIEnv in thread-local-storage; that way it'll be passed into your destructor as the argument.)
		 * Note: The destructor is not called unless the stored value is != NULL
		 * Note: You can call this function any number of times for the same thread, there's no harm in it
		 *       (except for some lost CPU cycles)
		 */
		pthread_setspecific(s_attachThreadKey, (void*)jni_env);

	}
	else if(JNI_OK == get_env_stat)
	{
		// don't need to do anything
	}
	else if (get_env_stat == JNI_EVERSION)
	{
		__emscripten_log_print(ANDROID_LOG_ERROR, "Iup", "GetEnv version not supported"); 
	}


	return jni_env;
}

void iupEmscripten_RetainIhandle(JNIEnv* jni_env, jobject native_widget, Ihandle* ih)
{
	if(ih)
	{
		ih->handle = (jobject)((*jni_env)->NewGlobalRef(jni_env, native_widget));
		__emscripten_log_print(ANDROID_LOG_INFO, "Iup", "NewGlobalRef on ih->handle: %x", ih->handle); 
	}
}

void iupEmscripten_ReleaseIhandle(JNIEnv* jni_env, Ihandle* ih)
{
	if(ih && ih->handle)
	{
		__emscripten_log_print(ANDROID_LOG_INFO, "Iup", "DeleteGlobalRef on ih->handle: %x", ih->handle); 
		(*jni_env)->DeleteGlobalRef(jni_env, ih->handle);
		ih->handle = NULL;
	}
}


jobject iupEmscripten_GetApplication(JNIEnv* jni_env)
{
	jclass java_class;
    jmethodID method_id;
	jobject ret_object;

	java_class = (*jni_env)->FindClass(jni_env, "br/pucrio/tecgraf/iup/IupApplication");
	method_id = (*jni_env)->GetStaticMethodID(jni_env, java_class, "getIupApplication", "()Lbr/pucrio/tecgraf/iup/IupApplication;");
	ret_object = (*jni_env)->CallStaticObjectMethod(jni_env, java_class, method_id);

	(*jni_env)->DeleteLocalRef(jni_env, java_class);

	return ret_object;

}

jobject iupEmscripten_GetCurrentActivity(JNIEnv* jni_env)
{
	jclass java_class;
    jmethodID method_id;
	jobject ret_object;

	jobject application_object = iupEmscripten_GetApplication(jni_env);

    java_class = (*jni_env)->GetObjectClass(jni_env, application_object);
	method_id = (*jni_env)->GetMethodID(jni_env, java_class, "getCurrentActivity", "()Lemscripten/app/Activity;");
	ret_object = (*jni_env)->CallObjectMethod(jni_env, application_object, method_id);

	(*jni_env)->DeleteLocalRef(jni_env, java_class);
	(*jni_env)->DeleteLocalRef(jni_env, application_object);

	return ret_object;
}

void iupEmscripten_AddWidgetToParent(JNIEnv* jni_env, Ihandle* ih)
{


	jclass java_class;
    jmethodID method_id;

	jobject parent_native_handle = iupChildTreeGetNativeParentHandle(ih);
	jobject child_handle = ih->handle;
	
		__emscripten_log_print(ANDROID_LOG_INFO, "iupEmscriptenAddWidgetToParent", "parent_native_handle:%x, ih->handle: %x", parent_native_handle, ih->handle); 


	java_class = (*jni_env)->FindClass(jni_env, "br/pucrio/tecgraf/iup/IupCommon");
	method_id = (*jni_env)->GetStaticMethodID(jni_env, java_class, "addWidgetToParent", "(Ljava/lang/Object;Ljava/lang/Object;)V");
	(*jni_env)->CallStaticVoidMethod(jni_env, java_class, method_id, parent_native_handle, child_handle);

	(*jni_env)->DeleteLocalRef(jni_env, java_class);


}



void iupdrvActivate(Ihandle* ih)
{

}

void iupdrvReparent(Ihandle* ih)
{

	
}


void iupdrvBaseLayoutUpdateMethod(Ihandle *ih)
{

#if 0
	id parent_native_handle = iupChildTreeGetNativeParentHandle(ih);
	NSView* parent_view = nil;
	if([parent_native_handle isKindOfClass:[NSWindow class]])
	{
		NSWindow* parent_window = (NSWindow*)parent_native_handle;
		parent_view = [parent_window contentView];
	}
	else if([parent_native_handle isKindOfClass:[NSView class]])
	{
		parent_view = (NSView*)parent_native_handle;
	}
	else
	{
		NSCAssert(1, @"Unexpected type for parent widget");
		@throw @"Unexpected type for parent widget";
	}
	
	
	
	id child_handle = ih->handle;
	NSView* the_view = nil;
	if([child_handle isKindOfClass:[NSView class]])
	{
		the_view = (NSView*)child_handle;
	}
	else if([child_handle isKindOfClass:[CALayer class]])
	{
		NSCAssert(1, @"CALayer not implemented");
		@throw @"CALayer not implemented";
	}
	else
	{
		NSCAssert(1, @"Unexpected type for parent widget");
		@throw @"Unexpected type for parent widget";
	}
	
	
//	iupgtkNativeContainerMove((GtkWidget*)parent, widget, x, y);

//	iupgtkSetPosSize(GTK_CONTAINER(parent), widget, ih->x, ih->y, ih->currentwidth, ih->currentheight);

	/*
	CGSize fitting_size = [the_view fittingSize];
	ih->currentwidth = fitting_size.width;
	ih->currentheight = fitting_size.height;
*/
	
	NSRect parent_rect = [parent_view frame];

	NSRect the_rect = NSMakeRect(
		ih->x,
		// Need to invert y-axis, and also need to shift/account for height of widget because that is also lower-left instead of upper-left.
		parent_rect.size.height - ih->y - ih->currentheight,
		ih->currentwidth,
		ih->currentheight
	);
	[the_view setFrame:the_rect];
//	[the_view setBounds:the_rect];
	
	
#endif
}

void iupdrvBaseUnMapMethod(Ihandle* ih)
{
	// Why do I need this when everything else has its own UnMap method?
	//NSLog(@"iupdrvBaseUnMapMethod not implemented. Might be leaking");
}

void iupdrvDisplayUpdate(Ihandle *ih)
{
	jobject the_handle = ih->handle;
	
	// call ViewGroup.invalidate()

}

void iupdrvDisplayRedraw(Ihandle *ih)
{
	iupdrvDisplayUpdate(ih);
}

void iupdrvScreenToClient(Ihandle* ih, int *x, int *y)
{
}



int iupdrvBaseSetZorderAttrib(Ihandle* ih, const char* value)
{
  return 0;
}

void iupdrvSetVisible(Ihandle* ih, int visible)
{
	jobject the_object = ih->handle;
}

int iupdrvIsVisible(Ihandle* ih)
{
	return 1;
}

int iupdrvIsActive(Ihandle *ih)
{
  return 1;
}

void iupdrvSetActive(Ihandle* ih, int enable)
{
}

char* iupdrvBaseGetXAttrib(Ihandle *ih)
{
  return NULL;
}

char* iupdrvBaseGetYAttrib(Ihandle *ih)
{

  return NULL;
}

/*
char* iupdrvBaseGetClientSizeAttrib(Ihandle *ih)
{

    return NULL;

}
 */

int iupdrvBaseSetBgColorAttrib(Ihandle* ih, const char* value)
{

	

  /* DO NOT NEED TO UPDATE GTK IMAGES SINCE THEY DO NOT DEPEND ON BGCOLOR */

  return 1;
}

int iupdrvBaseSetCursorAttrib(Ihandle* ih, const char* value)
{

  return 0;
}


int iupdrvGetScrollbarSize(void)
{

  return 0;
}

void iupdrvDrawFocusRect(Ihandle* ih, void* _gc, int x, int y, int w, int h)
{

}

void iupdrvBaseRegisterCommonAttrib(Iclass* ic)
{
	/*
#ifndef GTK_MAC
  #ifdef WIN32                                 
    iupClassRegisterAttribute(ic, "HFONT", iupgtkGetFontIdAttrib, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT|IUPAF_NO_STRING);
  #else
    iupClassRegisterAttribute(ic, "XFONTID", iupgtkGetFontIdAttrib, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT|IUPAF_NO_STRING);
  #endif
#endif
  iupClassRegisterAttribute(ic, "PANGOFONTDESC", iupgtkGetPangoFontDescAttrib, NULL, NULL, NULL, IUPAF_NOT_MAPPED|IUPAF_NO_INHERIT|IUPAF_NO_STRING);
*/
}

void iupdrvBaseRegisterVisualAttrib(Iclass* ic)
{
	
}

void iupdrvClientToScreen(Ihandle* ih, int *x, int *y)
{
	
}

void iupdrvPostRedraw(Ihandle *ih)
{

}

void iupdrvRedrawNow(Ihandle *ih)
{

}
void iupdrvSendKey(int key, int press)
{
	
}
void iupdrvSendMouse(int x, int y, int bt, int status)
{
	
}
void iupdrvSleep(int time)
{
	
}
void iupdrvWarpPointer(int x, int y)
{
	
}
