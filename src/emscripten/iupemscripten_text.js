/*global document alert IupCommon Pointer_stringify autoAddDeps mergeInto LibraryManager*/

var LibraryIupText = {
  //	$Button__deps: ['$CommonGlobals'],
  $Text: {
  },

  emjsText_CreateText: function(subType) {
    var widget_object;

    // check what type it is (0 = single-line input, 1 = multi-line input), build accordingly
    if (subType === 0) widget_object = document.createElement("input");
    else if (subType === 1) widget_object = document.createElement("textarea");

    var handle_id = IupCommon.RegisterNewObject(widget_object);
    IupCommon.InitializeObject(widget_object);
    return handle_id;
  },

  emjsText_DestroyText: function(handle_id) {
    // Do I need to removeEventListener?
    IupCommon.DeleteObject(handle_id);
  },

  emjsText_GetText: function(handle_id) {
    var widget_object = IupCommon.GetObjectForID(handle_id);
    var userText;
    if (widget_object) {
      userText = widget_object.value;
    }
    var c_str = allocate(intArrayFromString(userText), 'i8', ALLOC_NORMAL);
    return c_str;
  },

  emjsText_SetText: function(handle_id, text) {
    var widget_object = IupCommon.GetObjectForID(handle_id);
    if(widget_object)
    {
      widget_object.value = Pointer_stringify(text);
    }
  }

};

autoAddDeps(LibraryIupText, '$Text');
mergeInto(LibraryManager.library, LibraryIupText);
