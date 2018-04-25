/*global document alert IupCommon Pointer_stringify IupCommon autoAddDeps mergeInto LibraryManager*/

var LibraryIupLabel = {
  //	$Button__deps: ['$CommonGlobals'],
  $Label: {
  },

  emjsLabel_CreateLabel: function() {
    var widget_object;
    widget_object = document.createElement("div");

    var handle_id = IupCommon.RegisterNewObject(widget_object);
    IupCommon.InitializeObject(widget_object);

    return handle_id;
  },

  emjsLabel_DestroyLabel: function(handle_id) {
    // Do I need to removeEventListener?
    IupCommon.DeleteObject(handle_id);
  },

  emjsLabel_SetTitle: function(handle_id, label_title) {
    // console.log("label title " + label_title);
    var widget_object = IupCommon.GetObjectForID(handle_id);
    if(widget_object)
    {
      widget_object.innerHTML = Pointer_stringify(label_title);
    }
  },

  emjsLabel_SetPadding: function(handle_id, horiz, vert) {
    var widget_object = IupCommon.GetObjectForID(handle_id);

    // how does padding work in Iup?  Is it from corners or center?
    widget_object.style.paddingTop = vert + "px";
    widget_object.style.paddingBottom = vert + "px";
    widget_object.style.paddingLeft = horiz + "px";
    widget_object.style.paddingRight = horiz + "px";
  },

  emjsLabel_SetFGColor: function(handle_id, color) {
    var widget_object = IupCommon.GetObjectForID(handle_id);

    // what format is color? assuming its an rgb separated by spaces
    // console.log('handle id: ' + handle_id);
    // console.log('fg color: ' + color);
    // if (color) {
    //   var rgbVals = color.split(" ");
    //   var jsColor = 'rgb(' + rgbVals[0] + ', ' + rgbVals[1] + ', ' + rgbVals[2] + ')'; 
    //   widget_object.style.color = jsColor;
    // }
  },

  emjsLabel_SetBGColor: function(obj, val) {
    // console.log("bgcolor first arg: " + obj);
    // console.log("bgcolor sec arg: " + val);
  },

  emjsLabel_ToggleActive: function(handle_id, enable) {
    var widget_object = IupCommon.GetObjectForID(handle_id);
    enable === 1 ? widget_object.disabled = false : widget_object.disabled = true;
  },

  emjsLabel_SetAlignmentAttrib: function(handle_id, value) {
    // FUT - check to make sure ih->data->type != IUP_LABEL_SETP_HORIZ

    // get object to apply alignment to
    var widget_object = IupCommon.GetObjectForID(handle_id);
    var alignment = Pointer_stringify(value);

    // works like ARIGHT:ABOTTOM
    if (alignment === "ARIGHT") {
      widget_object.style.height = "30px";
      widget_object.style.display = "table-cell";
      widget_object.style.verticalAlign = "middle";
      widget_object.style.textAlign = "right";  
    }
    else if (alignment === "ACENTER") {

      widget_object.style.height = "30px";
      widget_object.style.display = "table-cell";
      widget_object.style.verticalAlign = "middle";
      widget_object.style.textAlign = "center";
    }
    else {  // must be ALEFT

      widget_object.style.height = "30px";
      widget_object.style.display = "table-cell";
      widget_object.style.verticalAlign = "middle";
      widget_object.style.textAlign = "left"; 
    }

    // cant really align vertically in CSS like it does in gtk...
    // will need to further understand how the containers work and
    // maybe use padding or margin percentage to emulate

    return 1;
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
  },

  emjsLabel_EnableEllipsis: function(handle_id) {
    var widget_object = IupCommon.GetObjectForID(handle_id);
    widget_object.style.whiteSpace = "nowrap";
    widget_object.style.overflow = "hidden";
    widget_object.style.textOverflow = "ellipsis";
    // missing: -o-text-overflow: ellipsis;
  },

  emjsLabel_DisableWordWrap: function(handle_id) {
    var widget_object = IupCommon.GetObjectForID(handle_id);
    widget_object.style.whiteSpace = "-moz-pre-wrap"; /* Firefox */
    widget_object.style.whiteSpace = "-o-pre-wrap"; /* Opera */
    widget_object.style.whiteSpace = "pre-wrap"; /* Chrome */
    widget_object.style.wordWrap = "break-word"; /* IE */
  }

};

autoAddDeps(LibraryIupLabel, '$Label');
mergeInto(LibraryManager.library, LibraryIupLabel);

