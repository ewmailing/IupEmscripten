
var LibraryIupLabel = {
  //	$Button__deps: ['$CommonGlobals'],
  $Label: {
  },

  emjsLabel_CreateLabel: function() {
    var widget_object;
    widget_object = document.createElement("div");
    var handle_id = IupCommon.RegisterNewObject(widget_object);
    widget_object.id = 'label_' + handle_id;
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

  emjsLabel_SetFGColor: function(handle_id, color) {
    //TO DO NEXT
  }

  emjsLabel_SetAlignmentAttrib: function(handle_id, value) {
    // FUT - check to make sure ih->data->type != IUP_LABEL_SETP_HORIZ
    
    // get object to apply alignment to
    var widget_object = IupCommon.GetObjectForID(handle_id);

    if (value === "ARIGHT") {
      widget_object.style.textAlign = "right";  
    }
    else if (value === "ACENTER") {
      widget_object.style.textAlign = "center";
    }
    else {  // must be ALEFT
      widget_object.style.textAlign = "left"; 
    }

    // cant really align vertically in CSS like it does in gtk...
    // will need to further understand how the containers work and
    // maybe use padding or margin percentage to emulate
    
    return 1;
  }

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
