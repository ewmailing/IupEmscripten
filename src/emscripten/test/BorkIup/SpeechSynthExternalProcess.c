#include "SpeechSynth.h"
#include "BlurrrCore.h"
#include "SDL.h"

static Speech_DidFinishCallback s_speechDidFinishCallback = NULL;


void Speech_Init()
{

}

void Speech_Quit()
{

}

struct MyThreadUserData
{
//	ISpVoice* speechSynthesizer;
	char* inputString;
};

// I actually don't know how dangerous it is to create a process in a background thread
static int RunSpawnedThread(void* user_data)
{
	struct MyThreadUserData* my_thread_user_data = (struct MyThreadUserData*)user_data;

	// In case we want to do other programs/platforms
#if BLURRR_PLATFORM_LINUX
	// looks like we have to use the full explict path or it won't work
	const char* speech_program = "/usr/bin/spd-say";
	char* argument_array[] =
	{
		my_thread_user_data->inputString,
		"--wait", // makes process finish speaking before returning
		NULL
	};
#else
	const char* speech_program = "/usr/bin/spd-say";
	char* argument_array[] =
	{
		my_thread_user_data->inputString,
		"--wait", // makes process finish speaking before returning
		NULL
	};
#endif

//	SDL_Log("going to say %s", my_thread_user_data->inputString);

	struct BlurrrProcessInfo* blurrr_process_info = BlurrrProcess_CreateProcess(speech_program, argument_array, NULL);
	if(NULL == blurrr_process_info)
	{
		SDL_Log("Failed to create process");
	}
	
	while(BlurrrProcess_UpdateProcess(blurrr_process_info))
	{
		// be nice to CPU
		SDL_Delay(100);
	}

	BlurrrProcess_FreeProcessInfo(blurrr_process_info);

	free(my_thread_user_data->inputString);
	free(my_thread_user_data);

	// TODO: How to signal main thread playback is done?
	SDL_Log("Process finished playing");
	if(NULL != s_speechDidFinishCallback)
	{
		s_speechDidFinishCallback();
	}

	return 0;
}

void Speech_SayString(const char* input_string)
{

	struct MyThreadUserData* my_thread_user_data = (struct MyThreadUserData* )calloc(1, sizeof(struct MyThreadUserData));
	size_t input_str_len = strlen(input_string);

	my_thread_user_data->inputString = (char*)calloc(input_str_len+1, sizeof(char));;
	strncpy(my_thread_user_data->inputString, input_string, input_str_len+1);

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

