/** \file
 * \brief Timer for the Emscripten backend.
 *
 * See Copyright Notice in "iup.h"
 */
#include <stdio.h>
#include <stdlib.h>

#include "iup.h"
#include "iupcbs.h"
#include "iup_object.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_assert.h"
#include "iup_timer.h"

extern void emjsTimer_CreateTimer(void);

#if 0
- (void) onTimerCallback:(NSTimer*)theTimer
{
  Icallback callback_function;
  Ihandle* ih = (Ihandle*)[[[self theTimer] userInfo] pointerValue];
  callback_function = IupGetCallback(ih, "ACTION_CB");
  
  if(callback_function)
    {
      CFTimeInterval start_time = [self startTime];
      double current_time = CACurrentMediaTime();
      NSUInteger elapsed_time = (NSUInteger)(((current_time - start_time) * 1000.0) + 0.5);
      iupAttribSetInt(ih, "ELAPSEDTIME", (int)elapsed_time);
    
      if(callback_function(ih) == IUP_CLOSE)
        {
          IupExitLoop();
        }
    }
}

#endif


#if 0
void iupdrvTimerRun(Ihandle* ih)
{
  unsigned int time_ms;
  

  if (ih->handle != nil) { /* timer already started */
    return;
  }
  
  time_ms = iupAttribGetInt(ih, "TIME");
  if (time_ms > 0)
    {
      IupCocoaTimerController* timer_controller = [[IupCocoaTimerController alloc] init];
      // CACurrentMediaTime is tied to a real time clock. It uses mach_absolute_time() under the hood.
      // GNUStep: Neither of these is likely directly portable (CACurrentMediaTime more likely), so we may need an #ifdef here.
      // [[NSDate date] timeIntervalSince1970]; isn't so great because it is affected by network clock changes and so forth.
      double start_time = CACurrentMediaTime();

      NSTimer* the_timer = [NSTimer scheduledTimerWithTimeInterval:(time_ms/1000.0)
                            target:timer_controller
                            selector:@selector(onTimerCallback:)
                            userInfo:(id)[NSValue valueWithPointer:ih]
                            repeats:YES
                            ];
    


      // Cocoa seems to block timers or events sometimes. This can be seen
      // when I'm animating (via a timer) and you open an popup box or move a slider.
      // Apparently, sheets and dialogs can also block (try printing).
      // To work around this, Cocoa provides different run-loop modes. I need to
      // specify the modes to avoid the blockage.
      // NSDefaultRunLoopMode seems to be the default. I don't think I need to explicitly
      // set this one, but just in case, I will set it anyway.
      [[NSRunLoop currentRunLoop] addTimer:the_timer forMode:NSRunLoopCommonModes];


      [timer_controller setTheTimer:the_timer];
      [timer_controller setStartTime:start_time];

      ih->handle = (__unsafe_unretained void*)timer_controller;
    }
  
}
#endif

#if 0
static void cocoaTimerDestroy(Ihandle* ih)
{
  if(nil != ih->handle)
    {
      IupCocoaTimerController* timer_controller = (IupCocoaTimerController*)ih->handle;
      NSTimer* the_timer = [timer_controller theTimer];
    
      [the_timer invalidate];
    
      // This will also release the timer instance via the dealloc
      [timer_controller release];
    
      ih->handle = nil;
    }
}
#endif

void iupdrvTimerStop(Ihandle* ih)
{

  //	cocoaTimerDestroy(ih);

}

static int emscriptenTimerMapMethod(Ihandle* ih)
{
  int timer_id = 0;
  InativeHandle* new_handle = NULL;

  emjsTimer_CreateTimer();
  /* new_handle = (InativeHandle*)calloc(1, sizeof(InativeHandle)); */

  /* new_handle->handleID = timer_id; */
  /* ih->handle = new_handle; */

  /* iupEmscripten_SetIntKeyForIhandleValue(timer_id, ih); */

  /* iupEmscripten_AddWidgetToParent(ih); */
  
	return IUP_NOERROR;
}

static void emscriptenTimerUnMapMethod(Ihandle* ih)
{
	/*
	id the_label = ih->handle;
	[the_label release];
	ih->handle = nil;
	*/

}

void iupdrvTimerInitClass(Iclass* ic)
{
  /* Driver Dependent Class functions */
  ic->Map = emscriptenTimerMapMethod;
	ic->UnMap = emscriptenTimerUnMapMethod;
}


