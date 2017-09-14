#include "SpeechSynth.h"
#include "BlurrrCore.h" // help convert UTF8 to Win strings
#include <sapi.h>
#include <stdlib.h>
#include <SDL.h> // for threads

static Speech_DidFinishCallback s_speechDidFinishCallback = NULL;


void Speech_Init()
{
	// I originally tried creating the synth here, but it didn't seem to work when trying to use on another threead.
	// I also tried calling ::CoInitialize(NULL) here, but CoCreateInstance on the other thread, but that also didn't work.
	// So now all of it is done in the other thread.
	// Since the other thread doesn't block, I don't care so much if the allocation is slow (or not)

}

void Speech_Quit()
{

}

// https://stackoverflow.com/questions/15573246/store-a-variable-that-holds-user-input-data-in-sapi5-speak-function
// https://stackoverflow.com/questions/26132995/getting-sapi-to-say-a-string
// TODO: Look up Iup's and SDL's functions for this.


struct MyThreadUserData
{
	wchar_t* inputWString;
};



static int RunSpawnedThread(void* user_data)
{
	HRESULT hr;
	struct MyThreadUserData* my_thread_user_data = (struct MyThreadUserData*)user_data;
	ISpVoice* speech_synthesizer = NULL;

	::CoInitialize(NULL);
	hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL, IID_ISpVoice, (void **)&speech_synthesizer);
	if (!SUCCEEDED(hr))
	{
		OutputDebugStringA("CoCreateInstance failed");
	}

	// Microsoft blocks on this API.
	//	HRESULT hr = s_speechSynthesizer->Speak(L"Hello world", 0, NULL);
	hr = speech_synthesizer->Speak(my_thread_user_data->inputWString, 0, NULL);

	speech_synthesizer->Release();
	speech_synthesizer = NULL;
	::CoUninitialize();

	BlurrrCore_Free(my_thread_user_data->inputWString);
	free(my_thread_user_data);

	// TODO: How to signal main thread playback is done?
	
	if(NULL != s_speechDidFinishCallback)
	{
		s_speechDidFinishCallback();
	}

	return 0;
}

void Speech_SayString(const char* input_string)
{
	struct MyThreadUserData* my_thread_user_data = (struct MyThreadUserData*)calloc(1, sizeof(struct MyThreadUserData));
	// Ugh. Microsoft only supports their wide string format and not UTF8. Must convert.
	my_thread_user_data->inputWString = BlurrrPlatformWindows_CreateWINfromUTF8String(input_string);

	SDL_Thread* the_thread = SDL_CreateThread(RunSpawnedThread, "SpeechThread", my_thread_user_data);
	if(NULL == the_thread)
	{
		SDL_Log("SDL_CreateThread failed: %s\n", SDL_GetError());
	}
	else
	{
		SDL_DetachThread(the_thread);  /* will go away on its own upon completion. */

	}
}

void Speech_SetFinishedCallback(Speech_DidFinishCallback callback_function)
{
	s_speechDidFinishCallback = callback_function;
}


