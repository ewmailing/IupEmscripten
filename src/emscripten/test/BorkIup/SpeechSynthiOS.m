#include "SpeechSynth.h"
#import <AVFoundation/AVFoundation.h>

static AVSpeechSynthesizer* s_speechSynthesizer = nil;
static Speech_DidFinishCallback s_speechDidFinishCallback = NULL;

@interface MySpeechFinishedDelegate : NSObject <AVSpeechSynthesizerDelegate>
@end

@implementation MySpeechFinishedDelegate

- (void)speechSynthesizer:(AVSpeechSynthesizer*)speech_synthesizer didFinishSpeechUtterance:(AVSpeechUtterance*)utterance
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
	AVSpeechSynthesizer* speech_synth = [[AVSpeechSynthesizer alloc] init];
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

	NSString* ns_string = [NSString stringWithUTF8String:input_string];

	AVSpeechSynthesizer* speech_synth = s_speechSynthesizer;
	
	AVSpeechUtterance* speech_utterance = [AVSpeechUtterance speechUtteranceWithString:ns_string];

	[speech_synth speakUtterance:speech_utterance];

	
}

void Speech_SetFinishedCallback(Speech_DidFinishCallback callback_function)
{
	s_speechDidFinishCallback = callback_function;
}

