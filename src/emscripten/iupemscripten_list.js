/*global document alert IupCommon Pointer_stringify IupCommon autoAddDeps mergeInto LibraryManager*/

var LibraryIupList = {
  //	$Button__deps: ['$CommonGlobals'],
  $List: {
  },

  emjsList_CreateList: function() {
    console.log("Create list fired");
    var widget_object;
    widget_object = document.createElement("select");
    var handle_id = IupCommon.RegisterNewObject(widget_object);
    widget_object.id = 'list_' + handle_id;
    return handle_id;
  },

  emjsList_DestroyList: function(handle_id) {
    // Do I need to removeEventListener?
    IupCommon.DeleteObject(handle_id);
  }

};

autoAddDeps(LibraryIupList, '$List');
mergeInto(LibraryManager.library, LibraryIupList);

