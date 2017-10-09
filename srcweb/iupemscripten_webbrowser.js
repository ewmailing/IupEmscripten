/*global document alert IupCommon Pointer_stringify IupCommon autoAddDeps mergeInto LibraryManager*/

var LibraryIupWebBrowser = {
  //	$Button__deps: ['$CommonGlobals'],
  $WebBrowser: {
  },

  emjsWebBrowser_CreateBrowser: function() {
    var widget_object;
    widget_object = document.createElement('iframe');
    widget_object.src = 'https://www.mdn.org';

    var handle_id = IupCommon.RegisterNewObject(widget_object);
    IupCommon.InitializeObject(widget_object);
    console.log(widget_object);
    return handle_id;
  },

  emjsWebBrowser_DestroyBrowser: function(handle_id) {
    IupCommon.DeleteObject(handle_id);
  }


};

autoAddDeps(LibraryIupWebBrowser, '$WebBrowser');
mergeInto(LibraryManager.library, LibraryIupWebBrowser);
