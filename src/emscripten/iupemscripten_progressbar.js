/*global document alert IupCommon Pointer_stringify IupCommon autoAddDeps mergeInto LibraryManager*/

var LibraryIupProgressBar = {
  $ProgressBar: {},

  emjsProgressBar_Create: function() {
    console.log("TESTING");
    var widget_object;
    widget_object = document.createElement('progress');
    console.log(widget_object);
    var handle_id = IupCommon.RegisterNewObject(widget_object);
    IupCommon.InitializeObject(widget_object);
    return handle_id;
  }
};

autoAddDeps(LibraryIupProgressBar, '$ProgressBar');
mergeInto(LibraryManager.library, LibraryIupProgressBar);
