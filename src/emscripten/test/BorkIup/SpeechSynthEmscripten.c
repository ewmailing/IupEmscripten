#include "SpeechSynth.h"
#include <stddef.h>

static Speech_DidFinishCallback s_speechDidFinishCallback = NULL;

void Speech_Init()
{
}

void Speech_Quit()
{

}

void Speech_SayString(const char* input_string)
{
}

void Speech_SetFinishedCallback(Speech_DidFinishCallback callback_function)
{
	s_speechDidFinishCallback = callback_function;
}
