/*global document alert IupCommon Pointer_stringify IupCommon autoAddDeps mergeInto LibraryManager*/

varLibraryIupProgressBar = {
  $ProgressBar: {},
  emjsProgressBar_Create: function() {
    var widget_object;
    widget_object = document.createElement('progress');
    var handle_id = IupCommon.RegisterNewObject(widget_object);
    IupCommon.InitializeObject(widget_object);
    console.log(widget_object);
    return handle_id;
  }
};
