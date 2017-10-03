/*global document alert IupCommon Pointer_stringify IupCommon autoAddDeps mergeInto LibraryManager*/

var LibraryIupWebBrowser = {
  //	$Button__deps: ['$CommonGlobals'],
  $Label: {
  },


};

autoAddDeps(LibraryIupWebBrowser, '$Label');
mergeInto(LibraryManager.library, LibraryIupWebBrowser);
