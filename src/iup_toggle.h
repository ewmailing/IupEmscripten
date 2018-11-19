/** \file
 * \brief Toggle Controls (not exported API)
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUP_TOGGLE_H 
#define __IUP_TOGGLE_H

#ifdef __cplusplus
extern "C" {
#endif


void iupdrvButtonAddBorders(int *x, int *y);  /* reuse button borders */

void iupdrvToggleInitClass(Iclass* ic);
void iupdrvToggleAddCheckBox(int *x, int *y, const char* str);

IUP_EXPORTI Ihandle *iupRadioFindToggleParent(Ihandle* ih_toggle);
char* iupToggleGetPaddingAttrib(Ihandle* ih);

enum {IUP_TOGGLE_IMAGE, IUP_TOGGLE_TEXT};

struct _IcontrolData 
{
  int type,                         /* the 2 toggle possibilities */
      is_radio,
      flat,
      horiz_padding, vert_padding;  /* toggle margin for images */
};


#ifdef __cplusplus
}
#endif

#endif
