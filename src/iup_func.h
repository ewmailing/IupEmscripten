/** \file
 * \brief Global Function table (not exported API)
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUP_FUNC_H 
#define __IUP_FUNC_H

#ifdef __cplusplus
extern "C" {
#endif

IUP_EXPORTI /* called only in IupOpen and IupClose */
void iupFuncInit(void);
void iupFuncFinish(void);

IUP_EXPORTI char* iupGetCallbackName(Ihandle *ih, const char *name);

/* Other functions declared in <iup.h> and implemented here. 
IUP_EXPORTI IupGetFunction
IUP_EXPORTI IupSetFunction
*/

#ifdef __cplusplus
}
#endif

#endif
