/** \file
 * \brief IUP Core pre-defined dialogs (not exported API)
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUP_PREDIAL_H 
#define __IUP_PREDIAL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "iup_export.h"

IUP_EXPORTI /* Used by the IupScanf implementation */
IUP_EXPORTI int iupDataEntry(int maxlin, int* maxcol, int* maxscr, char* title, char** text, char** data);

IUP_EXPORTI /* Popups a dialog with IUP Version, used in IupOpen */
IUP_EXPORTI void iupShowVersion(void);

/* Other functions declared in <iup.h> and implemented here. 
IUP_EXPORTI IupListDialog
IUP_EXPORTI IupAlarm
IUP_EXPORTI IupMessagef
IUP_EXPORTI IupGetFile
IUP_EXPORTI IupGetText
*/

#ifdef __cplusplus
}
#endif

#endif
