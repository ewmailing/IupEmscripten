/** \file
 * \brief Abstract Layout Management (not exported API)
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUP_LAYOUT_H 
#define __IUP_LAYOUT_H

#ifdef __cplusplus
extern "C" {
#endif

IUP_EXPORTI /* called from IupMap and IupRefresh */
void iupLayoutCompute(Ihandle* ih);  /* can be called before map */
IUP_EXPORTI void iupLayoutUpdate(Ihandle* ih);   /* called only after map */

IUP_EXPORTI void iupLayoutApplyMinMaxSize(Ihandle* ih, int *w, int *h);

/* Other functions declared in <iup.h> and implemented here. 
IupRefresh
*/


#ifdef __cplusplus
}
#endif

#endif
