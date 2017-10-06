
/** \file
 * \brief Web Browser Control
 *
 * See Copyright Notice in "iup.h"
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <stdarg.h>

#include "iup.h"
#include "iupcbs.h"

#include "iup_object.h"
#include "iup_layout.h"
#include "iup_attrib.h"
#include "iup_str.h"
#include "iup_webbrowser.h"
#include "iup_drv.h"
#include "iup_drvfont.h"
#include "iup_key.h"
#include "iup_register.h"

/* #include "iupemscripten_drv.h" */

#if 0
static char* gtkWebBrowserGetItemHistoryAttrib(Ihandle* ih, int id)
{
  WebKitWebBackForwardList *back_forward_list = webkit_web_view_get_back_forward_list ((WebKitWebView*)ih->handle);
  WebKitWebHistoryItem* item = webkit_web_back_forward_list_get_nth_item(back_forward_list, id);
  if (item)
    return iupStrReturnStr(webkit_web_history_item_get_uri(item));
  else
    return NULL;
}

static char* gtkWebBrowserGetForwardCountAttrib(Ihandle* ih)
{
  WebKitWebBackForwardList *back_forward_list = webkit_web_view_get_back_forward_list ((WebKitWebView*)ih->handle);
  return iupStrReturnInt(webkit_web_back_forward_list_get_forward_length(back_forward_list));
}

static char* gtkWebBrowserGetBackCountAttrib(Ihandle* ih)
{
  WebKitWebBackForwardList *back_forward_list = webkit_web_view_get_back_forward_list ((WebKitWebView*)ih->handle);
  return iupStrReturnInt(webkit_web_back_forward_list_get_back_length(back_forward_list));
}

static int gtkWebBrowserSetHTMLAttrib(Ihandle* ih, const char* value)
{
  if (value)
    webkit_web_view_load_string((WebKitWebView*)ih->handle, iupgtkStrConvertToSystem(value), "text/html", "UTF-8", "");
  return 0; /* do not store value in hash table */
}

static int gtkWebBrowserSetCopyAttrib(Ihandle* ih, const char* value)
{
  webkit_web_view_copy_clipboard((WebKitWebView*)ih->handle);
  (void)value;
  return 0;
}

static int gtkWebBrowserSetSelectAllAttrib(Ihandle* ih, const char* value)
{
  webkit_web_view_select_all((WebKitWebView*)ih->handle);
  (void)value;
  return 0;
}

static int gtkWebBrowserSetPrintAttrib(Ihandle* ih, const char* value)
{
  WebKitWebFrame* frame = webkit_web_view_get_main_frame((WebKitWebView*)ih->handle);
  webkit_web_frame_print(frame);
  (void)value;
  return 0;
}

static int gtkWebBrowserSetZoomAttrib(Ihandle* ih, const char* value)
{
  int zoom;
  if (iupStrToInt(value, &zoom))
    webkit_web_view_set_zoom_level((WebKitWebView*)ih->handle, (float)zoom/100.0f);
  return 0;
}

static char* gtkWebBrowserGetZoomAttrib(Ihandle* ih)
{
  int zoom = (int)(webkit_web_view_get_zoom_level((WebKitWebView*)ih->handle) * 100);
  return iupStrReturnInt(zoom);
}

static char* gtkWebBrowserGetStatusAttrib(Ihandle* ih)
{
  WebKitLoadStatus status = webkit_web_view_get_load_status((WebKitWebView*)ih->handle);
  if (status == WEBKIT_LOAD_FAILED)
    return "FAILED";
  else if (status == WEBKIT_LOAD_FINISHED)
    return "COMPLETED";
  else
    return "LOADING";
}

static int gtkWebBrowserSetReloadAttrib(Ihandle* ih, const char* value)
{
  webkit_web_view_reload((WebKitWebView*)ih->handle);
  (void)value;
  return 0; /* do not store value in hash table */
}

static int gtkWebBrowserSetStopAttrib(Ihandle* ih, const char* value)
{
  webkit_web_view_stop_loading((WebKitWebView*)ih->handle);
  (void)value;
  return 0; /* do not store value in hash table */
}

static int gtkWebBrowserSetBackForwardAttrib(Ihandle* ih, const char* value)
{
  int val;
  if (iupStrToInt(value, &val))
  {
    /* Negative values represent steps backward while positive values represent steps forward. */
    webkit_web_view_go_back_or_forward((WebKitWebView*)ih->handle, val);
  }
  return 0; /* do not store value in hash table */
}

static int gtkWebBrowserSetValueAttrib(Ihandle* ih, const char* value)
{
  if (value)
    webkit_web_view_load_uri((WebKitWebView*)ih->handle, value);
  return 0; /* do not store value in hash table */
}

static char* gtkWebBrowserGetValueAttrib(Ihandle* ih)
{
  const gchar* value = webkit_web_view_get_uri((WebKitWebView*)ih->handle);
  return iupStrReturnStr(value);
}

/*********************************************************************************************/

static void gtkWebBrowserDocumentLoadFinished(WebKitWebView *web_view, WebKitWebFrame *frame, Ihandle *ih)
{
  IFns cb = (IFns)IupGetCallback(ih, "COMPLETED_CB");
  if (cb)
    cb(ih, (char*)webkit_web_frame_get_uri(frame));
}

static gboolean gtkWebBrowserLoadError(WebKitWebView *web_view, WebKitWebFrame *frame,
                                       gchar *uri, gpointer web_error, Ihandle *ih)
{
  IFns cb = (IFns)IupGetCallback(ih, "ERROR_CB");
  if (cb)
    cb(ih, uri);

  return FALSE;
}

static int gtkWebBrowserNavigate(WebKitWebView *web_view, WebKitWebFrame *frame, WebKitNetworkRequest *request,
                                 WebKitWebNavigationAction *navigation_action, WebKitWebPolicyDecision *policy_decision, Ihandle *ih)
{
  /*
  char *strReason = iupStrGetMemory(50);
  WebKitWebNavigationReason reason = webkit_web_navigation_action_get_reason(navigation_action);

  switch(reason)
  {
    case WEBKIT_WEB_NAVIGATION_REASON_LINK_CLICKED:
      sprintf(strReason, "%s", "LINK_CLICKED");
      break;
    case WEBKIT_WEB_NAVIGATION_REASON_FORM_SUBMITTED:
      sprintf(strReason, "%s", "FORM_SUBMITTED");
      break;
    case WEBKIT_WEB_NAVIGATION_REASON_BACK_FORWARD:
      sprintf(strReason, "%s", "BACK_FORWARD");
      break;
    case WEBKIT_WEB_NAVIGATION_REASON_RELOAD:
      sprintf(strReason, "%s", "RELOAD");
      break;
    case WEBKIT_WEB_NAVIGATION_REASON_FORM_RESUBMITTED:
      sprintf(strReason, "%s", "FORM_RESUBMITTED");
      break;
    case WEBKIT_WEB_NAVIGATION_REASON_OTHER:
      sprintf(strReason, "%s", "OTHER");
      break;
  }
  */

  IFns cb = (IFns)IupGetCallback(ih, "NAVIGATE_CB");
  if (cb)
  {
    if (cb(ih, (char*)webkit_network_request_get_uri(request)) == IUP_IGNORE)
      return FALSE;
  }

  return FALSE;
}

static WebKitWebView* gtkWebBrowserNewWindow(WebKitWebView  *web_view, WebKitWebFrame *frame, Ihandle *ih)
{
  IFns cb = (IFns)IupGetCallback(ih, "NEWWINDOW_CB");
  if (cb)
    cb(ih, (char*)webkit_web_frame_get_uri(frame));

  return web_view;
}

/*********************************************************************************************/

static void gtkWebBrowserDummyLogFunc(const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data)
{
  /* does nothing */
  (void)log_domain;
  (void)log_level;
  (void)message;
  (void)user_data;
}
#endif

static int emscriptenWebBrowserMapMethod(Ihandle* ih)
{

  return IUP_NOERROR;
}

/* static void gtkWebBrowserComputeNaturalSizeMethod(Ihandle* ih, int *w, int *h, int *children_expand) */
/* { */
/*   int natural_w = 0, natural_h = 0; */
/*   (void)children_expand; /\* unset if not a container *\/ */

/*   /\* natural size is 1 character *\/ */
/*   iupdrvFontGetCharSize(ih, &natural_w, &natural_h); */

/*   *w = natural_w; */
/*   *h = natural_h; */
/* } */

/* static int gtkWebBrowserCreateMethod(Ihandle* ih, void **params) */
/* { */
/*   (void)params; */

/*   ih->data = iupALLOCCTRLDATA(); */

/*   /\* default EXPAND is YES *\/ */
/*   ih->expand = IUP_EXPAND_BOTH; */
/*   ih->data->sb = IUP_SB_HORIZ | IUP_SB_VERT;  /\* default is YES *\/ */

/*   return IUP_NOERROR;  */
/* } */

Iclass* iupWebBrowserNewClass(void)
{
  Iclass* ic = iupClassNew(NULL);

  ic->name = "webbrowser";
  ic->format = NULL; /* no parameters */
  ic->nativetype  = IUP_TYPECONTROL;
  ic->childtype   = IUP_CHILDNONE;
  ic->is_interactive = 1;
  ic->has_attrib_id = 1;   /* has attributes with IDs that must be parsed */

  /* Class functions */
  #if 0
  ic->New = iupWebBrowserNewClass;
  ic->Create = gtkWebBrowserCreateMethod;
  ic->Map = gtkWebBrowserMapMethod;
  ic->UnMap = iupdrvBaseUnMapMethod;
  ic->ComputeNaturalSize = gtkWebBrowserComputeNaturalSizeMethod;
  ic->LayoutUpdate = iupdrvBaseLayoutUpdateMethod;

  /* Callbacks */
  iupClassRegisterCallback(ic, "NEWWINDOW_CB", "s");
  iupClassRegisterCallback(ic, "NAVIGATE_CB", "s");
  iupClassRegisterCallback(ic, "ERROR_CB", "s");

  /* Common */
  iupBaseRegisterCommonAttrib(ic);

  /* Visual */
  iupBaseRegisterVisualAttrib(ic);

  /* Overwrite Visual */
  iupClassRegisterAttribute(ic, "BGCOLOR", NULL, iupdrvBaseSetBgColorAttrib, IUPAF_SAMEASSYSTEM, "DLGBGCOLOR", IUPAF_DEFAULT); 

  /* IupWebBrowser only */
  iupClassRegisterAttribute(ic, "VALUE", gtkWebBrowserGetValueAttrib, gtkWebBrowserSetValueAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "BACKFORWARD", NULL, gtkWebBrowserSetBackForwardAttrib, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "STOP", NULL, gtkWebBrowserSetStopAttrib, NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "RELOAD", NULL, gtkWebBrowserSetReloadAttrib, NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "HTML", NULL, gtkWebBrowserSetHTMLAttrib, NULL, NULL, IUPAF_WRITEONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "STATUS", gtkWebBrowserGetStatusAttrib, NULL, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "COPY", NULL, gtkWebBrowserSetCopyAttrib, NULL, NULL, IUPAF_WRITEONLY | IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "SELECTALL", NULL, gtkWebBrowserSetSelectAllAttrib, NULL, NULL, IUPAF_WRITEONLY | IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "ZOOM", gtkWebBrowserGetZoomAttrib, gtkWebBrowserSetZoomAttrib, NULL, NULL, IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "PRINT", NULL, gtkWebBrowserSetPrintAttrib, NULL, NULL, IUPAF_WRITEONLY | IUPAF_NO_INHERIT);

  iupClassRegisterAttribute(ic, "BACKCOUNT", gtkWebBrowserGetBackCountAttrib, NULL, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttribute(ic, "FORWARDCOUNT", gtkWebBrowserGetForwardCountAttrib, NULL, NULL, NULL, IUPAF_NO_DEFAULTVALUE|IUPAF_READONLY|IUPAF_NO_INHERIT);
  iupClassRegisterAttributeId(ic, "ITEMHISTORY",  gtkWebBrowserGetItemHistoryAttrib,  NULL, IUPAF_READONLY|IUPAF_NO_INHERIT);
#endif

  return ic;
}