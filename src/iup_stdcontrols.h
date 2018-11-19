/** \file
 * \brief Standard Controls Class Initialization functions (not exported API)
 *
 * See Copyright Notice in "iup.h"
 */
 
#ifndef __IUP_STDCONTROLS_H 
#define __IUP_STDCONTROLS_H

#ifdef __cplusplus
extern "C" {
#endif


IUP_EXPORTI Iclass* iupDialogNewClass(void);
IUP_EXPORTI Iclass* iupMessageDlgNewClass(void);
Iclass* iupColorDlgNewClass(void);
Iclass* iupFontDlgNewClass(void);
IUP_EXPORTI Iclass* iupFileDlgNewClass(void);
IUP_EXPORTI Iclass* iupProgressDlgNewClass(void);

IUP_EXPORTI Iclass* iupLabelNewClass(void);
IUP_EXPORTI Iclass* iupButtonNewClass(void);
IUP_EXPORTI Iclass* iupToggleNewClass(void);
IUP_EXPORTI Iclass* iupRadioNewClass(void);
IUP_EXPORTI Iclass* iupCanvasNewClass(void);
IUP_EXPORTI Iclass* iupFrameNewClass(void);
IUP_EXPORTI Iclass* iupProgressBarNewClass(void);
IUP_EXPORTI Iclass* iupTextNewClass(void);
IUP_EXPORTI Iclass* iupMultilineNewClass(void);
IUP_EXPORTI Iclass* iupValNewClass(void);
IUP_EXPORTI Iclass* iupTabsNewClass(void);
IUP_EXPORTI Iclass* iupSpinNewClass(void);
IUP_EXPORTI Iclass* iupSpinboxNewClass(void);
IUP_EXPORTI Iclass* iupListNewClass(void);
IUP_EXPORTI Iclass* iupTreeNewClass(void);
IUP_EXPORTI Iclass* iupLinkNewClass(void);
IUP_EXPORTI Iclass* iupFlatLabelNewClass(void);
IUP_EXPORTI Iclass* iupFlatButtonNewClass(void);
IUP_EXPORTI Iclass* iupFlatToggleNewClass(void);
IUP_EXPORTI Iclass* iupDropButtonNewClass(void);
IUP_EXPORTI Iclass* iupFlatSeparatorNewClass(void);
IUP_EXPORTI Iclass* iupFlatFrameNewClass(void);
IUP_EXPORTI Iclass* iupFlatTabsNewClass(void);
Iclass* iupDialNewClass(void);
IUP_EXPORTI Iclass* iupGaugeNewClass(void);
IUP_EXPORTI Iclass* iupColorbarNewClass(void);
IUP_EXPORTI Iclass* iupColorBrowserNewClass(void);

IUP_EXPORTI Iclass* iupMenuNewClass(void);
IUP_EXPORTI Iclass* iupItemNewClass(void);
IUP_EXPORTI Iclass* iupSeparatorNewClass(void);
IUP_EXPORTI Iclass* iupSubmenuNewClass(void);

IUP_EXPORTI Iclass* iupFillNewClass(void);
IUP_EXPORTI Iclass* iupHboxNewClass(void);
IUP_EXPORTI Iclass* iupVboxNewClass(void);
IUP_EXPORTI Iclass* iupZboxNewClass(void);
IUP_EXPORTI Iclass* iupCboxNewClass(void);
IUP_EXPORTI Iclass* iupSboxNewClass(void);
IUP_EXPORTI Iclass* iupNormalizerNewClass(void);
IUP_EXPORTI Iclass* iupSplitNewClass(void);
IUP_EXPORTI Iclass* iupScrollBoxNewClass(void);
IUP_EXPORTI Iclass* iupFlatScrollBoxNewClass(void);
Iclass* iupGridBoxNewClass(void);
IUP_EXPORTI Iclass* iupExpanderNewClass(void);
IUP_EXPORTI Iclass* iupDetachBoxNewClass(void);
IUP_EXPORTI Iclass* iupBackgroundBoxNewClass(void);
IUP_EXPORTI Iclass* iupAnimatedLabelNewClass(void);
IUP_EXPORTI Iclass* iupCalendarNewClass(void);
IUP_EXPORTI Iclass* iupDatePickNewClass(void);
IUP_EXPORTI Iclass* iupParamBoxNewClass(void);
Iclass* iupParamNewClass(void);

IUP_EXPORTI Iclass* iupTimerNewClass(void);
IUP_EXPORTI Iclass* iupImageNewClass(void);
IUP_EXPORTI Iclass* iupImageRGBNewClass(void);
IUP_EXPORTI Iclass* iupImageRGBANewClass(void);
IUP_EXPORTI Iclass* iupUserNewClass(void);
IUP_EXPORTI Iclass* iupClipboardNewClass(void);

/*************************************************/

void iupdrvMessageDlgInitClass(Iclass* ic);
void iupdrvColorDlgInitClass(Iclass* ic);
void iupdrvFontDlgInitClass(Iclass* ic);
void iupdrvFileDlgInitClass(Iclass* ic);

/************************************************/

IUP_EXPORTI Iclass* iupBackgroundBoxNewBaseClass(const char* name, const char* base_name);

/* Common definition of the canvas class */
typedef struct _iupCanvas {
IUP_EXPORTI   int sb;    /* scrollbar configuration, valid only after map, use iupBaseGetScrollbar before map */
  double posx, posy;
  int inside_resize;
} iupCanvas;

#ifdef __cplusplus
}
#endif

#endif
