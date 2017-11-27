/*global document alert IupCommon Pointer_stringify IupCommon autoAddDeps mergeInto LibraryManager*/

var LibraryIupProgressBar = {
  $ProgressBar: {},
  emjsProgressBar_Create: function() {
    var widget_object;
    widget_object = document.createElement('progress');
    var handle_id = IupCommon.RegisterNewObject(widget_object);
    IupCommon.InitializeObject(widget_object);
    console.log(widget_object);
    console.log('chris kind mother was in the progress bar create function sucking some d');
    return handle_id;
  }
};
autoAddDeps(LibraryIupProgressBar, '$ProgressBar');
mergeInto(LibraryManager.library, LibraryIupProgressBar);
console.log('eeyyyy boyyyeee u in da progress bar file');
