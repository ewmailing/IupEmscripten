#include "SpeechSynth.h"
#import <Cocoa/Cocoa.h>

static NSSpeechSynthesizer* s_speechSynthesizer = nil;
static Speech_DidFinishCallback s_speechDidFinishCallback = NULL;

@interface MySpeechFinishedDelegate : NSObject <NSSpeechSynthesizerDelegate>
@end

@implementation MySpeechFinishedDelegate

// This seems to fire on the main thread in my tests.
- (void)speechSynthesizer:(NSSpeechSynthesizer*)speech_synthesizer didFinishSpeaking:(BOOL)finished_speaking
{
//	[[speech_synthesizer delegate] release];
//	[speech_synthesizer release];
	
	if(NULL != s_speechDidFinishCallback)
	{
		s_speechDidFinishCallback();
	}
}

@end

void Speech_Init()
{
	if(s_speechSynthesizer)
	{
		return;
	}
	NSSpeechSynthesizer* speech_synth = [[NSSpeechSynthesizer alloc] initWithVoice:nil];
	MySpeechFinishedDelegate* speech_delegate = [[MySpeechFinishedDelegate alloc] init];
	[speech_synth setDelegate:speech_delegate];

	s_speechSynthesizer = speech_synth;
}

void Speech_Quit()
{
	[[s_speechSynthesizer delegate] release];
	[s_speechSynthesizer release];
	s_speechSynthesizer = nil;

}

void Speech_SayString(const char* input_string)
{
//	NSSpeechSynthesizer* speech_synth = [[NSSpeechSynthesizer alloc] initWithVoice:nil];
//	MySpeechFinishedDelegate* speech_delegate = [[MySpeechFinishedDelegate alloc] init];
//	[speech_synth setDelegate:speech_delegate];
	NSString* ns_string = [NSString stringWithUTF8String:input_string];
	
	NSSpeechSynthesizer* speech_synth = s_speechSynthesizer;

	[speech_synth startSpeakingString:ns_string];
}

void Speech_SetFinishedCallback(Speech_DidFinishCallback callback_function)
{
	s_speechDidFinishCallback = callback_function;
}
