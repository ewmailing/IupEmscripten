/** \file
 * \brief Frame Control (not exported API)
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUP_FRAME_H 
#define __IUP_FRAME_H

#ifdef __cplusplus
extern "C" {
#endif

int iupdrvFrameHasClientOffset(void);
void iupdrvFrameInitClass(Iclass* ic);
void iupdrvFrameGetDecorOffset(int *x, int *y);
IUP_EXPORTI int iupFrameGetTitleHeight(Ihandle* ih);
char* iupFrameGetBgColorAttrib(Ihandle* ih);

#ifdef __cplusplus
}
#endif

#endif
