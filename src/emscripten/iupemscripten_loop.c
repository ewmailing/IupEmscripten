/** \file
 * \brief Emscripten Message Loop
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdio.h>    
#include <string.h>    

#include "iup.h"
#include "iupcbs.h"


static IFidle emscripten_idle_cb = NULL;


void iupdrvSetIdleFunction(Icallback f)
{
  emscripten_idle_cb = (IFidle)f;
}

void IupExitLoop(void)
{
}


int IupMainLoopLevel(void)
{
  return 0;
}

/* I don't see any possible way of supporting this. 
   Emscripten/Java controls the main loop and users don't have access to it.
*/
int IupMainLoop(void)
{
	IFidle entry_callback = (IFidle)IupGetFunction("ENTRY_POINT");

	if(entry_callback)
	{
		entry_callback();
	}

	return IUP_NOERROR;

}

int IupLoopStepWait(void)
{
  return IUP_DEFAULT;
}

int IupLoopStep(void)
{
  return IUP_DEFAULT;
}

void IupFlush(void)
{
}

