/** \file
 * \brief Ihandle <-> Name table manager (not exported API).
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUP_NAMES_H 
#define __IUP_NAMES_H

#ifdef __cplusplus
extern "C" {
#endif

IUP_EXPORTI /* called only in IupOpen and IupClose */
void iupNamesInit(void);
void iupNamesFinish(void);
void iupNamesDestroyHandles(void);

IUP_EXPORTI /* called from IupDestroy */
void iupRemoveNames(Ihandle* ih);

/* Other functions declared in <iup.h> and implemented here.
IUP_EXPORTI IupGetName
IUP_EXPORTI IupGetAllDialogs
IUP_EXPORTI IupGetAllNames
IUP_EXPORTI IupSetHandle
IUP_EXPORTI IupGetHandle
*/

#ifdef __cplusplus
}
#endif

#endif
