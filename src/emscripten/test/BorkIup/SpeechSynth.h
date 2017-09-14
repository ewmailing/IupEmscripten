#ifndef SPEECH_SYNTH_H
#define SPEECH_SYNTH_H

#ifdef __cplusplus
extern "C" {
#endif


extern void Speech_Init(void);
extern void Speech_Quit(void);
extern void Speech_SayString(const char* input_string);

typedef void (*Speech_DidFinishCallback)(void);
extern void Speech_SetFinishedCallback(Speech_DidFinishCallback callback_function);




#ifdef __cplusplus
}
#endif

#endif

