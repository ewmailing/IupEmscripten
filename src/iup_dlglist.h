/** \file
 * \brief list of all created dialogs
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUP_DLGLIST_H 
#define __IUP_DLGLIST_H

#ifdef __cplusplus
extern "C" {
#endif


/** \defgroup dlglist List of Dialogs
 * \par
 * See \ref iup_dlglist.h
 * \ingroup cpi */


IUP_EXPORTI /** Adds a dialog to the list. Used only in IupDialog.
 * \ingroup dlglist */
IUP_EXPORTI void iupDlgListAdd(Ihandle *ih);

IUP_EXPORTI /** Removes a dialog from the list. Used only in IupDestroy.
 * \ingroup dlglist */
IUP_EXPORTI void iupDlgListRemove(Ihandle *ih);

/** Returns the number of dialogs.
 * \ingroup dlglist */
IUP_EXPORTI int iupDlgListCount(void);

/** Starts a loop for all the created dialogs. 
 * \ingroup dlglist */
IUP_EXPORTI Ihandle* iupDlgListFirst(void);

/** Retrieve the next dialog on the list. Must call iupDlgListFirst first.
 * \ingroup dlglist */
IUP_EXPORTI Ihandle* iupDlgListNext(void);

/** Increments the number of visible dialogs. 
 * \ingroup dlglist */
IUP_EXPORTI void iupDlgListVisibleInc(void);

/** Decrements the number of visible dialogs.
 * \ingroup dlglist */
IUP_EXPORTI void iupDlgListVisibleDec(void);

/** Returns the number of visible dialogs.
 * \ingroup dlglist */
IUP_EXPORTI int iupDlgListVisibleCount(void);

/* Destroy all dialogs and the list.
IUP_EXPORTI    Called only from IupClose. */
void iupDlgListDestroyAll(void);


#ifdef __cplusplus
}
#endif

#endif
