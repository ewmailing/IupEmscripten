#if defined(__EMSCRIPTEN__)

#include "SpeechSynth.h"
#include <stddef.h>
#include <emscripten.h>

static Speech_DidFinishCallback s_speechDidFinishCallback = NULL;


EMSCRIPTEN_KEEPALIVE void SpeechEmscripten_FinishedCallbackTrampoline()
{
	if(s_speechDidFinishCallback)
	{
		s_speechDidFinishCallback();
	}
}

void Speech_Init()
{
}

void Speech_Quit()
{

}

extern void emjsSpeechSynth_SayString(const char* input_string);
void Speech_SayString(const char* input_string)
{
	emjsSpeechSynth_SayString(input_string);
}

void Speech_SetFinishedCallback(Speech_DidFinishCallback callback_function)
{
	s_speechDidFinishCallback = callback_function;
}

#endif // __EMSCRIPTEN__

