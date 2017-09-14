#include "SpeechSynth.h"
#include <stddef.h>
#include <stdlib.h>
#include <jni.h>
#include <android/log.h>
#include "BlurrrCore.h"


static Speech_DidFinishCallback s_speechDidFinishCallback = NULL;


void Speech_Init()
{
	JNIEnv* jni_env = BlurrrPlatformAndroid_GetJNIEnv();
	jobject app_context = (jobject)BlurrrPlatformAndroid_GetApplicationContext();

	jclass java_class = (*jni_env)->FindClass(jni_env, "net/playcontrol/SpeechSynthHelper");
	jmethodID method_id = (*jni_env)->GetStaticMethodID(jni_env, java_class, "createSpeechSynth", "(Landroid/content/Context;)V");
	(*jni_env)->CallStaticVoidMethod(jni_env, java_class, method_id, app_context);

	(*jni_env)->DeleteLocalRef(jni_env, java_class);
}

void Speech_Quit()
{
	JNIEnv* jni_env = BlurrrPlatformAndroid_GetJNIEnv();

	jclass java_class = (*jni_env)->FindClass(jni_env, "net/playcontrol/SpeechSynthHelper");
	jmethodID method_id = (*jni_env)->GetStaticMethodID(jni_env, java_class, "destroySpeechSynth", "()V");
	(*jni_env)->CallStaticVoidMethod(jni_env, java_class, method_id);
	(*jni_env)->DeleteLocalRef(jni_env, java_class);
	
}

void Speech_SayString(const char* input_string)
{
	// input_string must not be NULL, but we cannot return early because we're not going to fire a callback for a NULL string.
	if(NULL == input_string)
	{
		input_string = "";
	}
// __android_log_print(ANDROID_LOG_DEBUG, "Speech_SayString", "Going to say: %s", input_string);

	JNIEnv* jni_env = BlurrrPlatformAndroid_GetJNIEnv();

	jclass java_class = (*jni_env)->FindClass(jni_env, "net/playcontrol/SpeechSynthHelper");
	jmethodID method_id = (*jni_env)->GetStaticMethodID(jni_env, java_class, "sayString", "(Ljava/lang/String;)V");
	jstring j_input_string = (*jni_env)->NewStringUTF(jni_env, input_string);
	(*jni_env)->CallStaticVoidMethod(jni_env, java_class, method_id, j_input_string);

	(*jni_env)->DeleteLocalRef(jni_env, j_input_string);
	(*jni_env)->DeleteLocalRef(jni_env, java_class);
}

void Speech_SetFinishedCallback(Speech_DidFinishCallback callback_function)
{
	s_speechDidFinishCallback = callback_function;
}

JNIEXPORT void JNICALL Java_com_net_playcontrol_SpeechSynthHelperr_InvokeFinishedCallback(JNIEnv* jni_env, jclass cls, jobject speech_synth)
{
	if(NULL != s_speechDidFinishCallback)
	{
		s_speechDidFinishCallback();
	}
}


