
var LibraryIupLabel = {
  //	$Button__deps: ['$CommonGlobals'],
  $Label: {
  },

  emjsLabel_CreateLabel: function() {
    var widget_object;
    widget_object = document.createElement("div");
    var handle_id = IupCommon.RegisterNewObject(widget_object);
    return handle_id;
  },

  emjsLabel_DestroyLabel: function(handle_id) {
    // Do I need to removeEventListener?
    IupCommon.DeleteObject(handle_id);
  },

  emjsLabel_SetTitle: function(handle_id, label_title) {
    console.log("label title " + label_title);
    var widget_object = IupCommon.GetObjectForID(handle_id);
    if(widget_object)
    {
      widget_object.innerHTML = Pointer_stringify(label_title);
    }
  },

  emjsLabel_CreateSeparator: function(handle_id, type) {
    var widget_object = IupCommon.GetObjectForID(handle_id);
    if (type === "horizontal") {
      var h = document.createElement("hr");
      widget_object.appendChild(h); 
    }
    else if (type === "vertical") {
      alert("vertical separator");
    }
  }

};

autoAddDeps(LibraryIupLabel, '$Label');
mergeInto(LibraryManager.library, LibraryIupLabel);

