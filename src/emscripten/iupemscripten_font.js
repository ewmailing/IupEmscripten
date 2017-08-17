/*global document alert IupCommon Pointer_stringify IupCommon autoAddDeps mergeInto LibraryManager*/

var LibraryIupFont = {
  //	$Button__deps: ['$CommonGlobals'],
  $Font: {
  },

  emjsFont_GetStringWidth: function(handle_id, str) {

    var widget_object = IupCommon.GetObjectForID(handle_id);
    var temp_object;
    temp_object = document.createElement("div");
    document.body.appendChild(temp_object);

    if(widget_object) {
      temp_object.style.fontFamily = widget_object.style.fontFamily;
      temp_object.style.fontSize = widget_object.style.fontSize;
    }

    temp_object.innerHTML = Pointer_stringify(str);
    temp_object.style.position = 'absolute';

    var w = Math.ceil(temp_object.clientWidth);

    document.body.removeChild(temp_object);

    return w;
  },


  emjsFont_GetMultiLineStringSize: function(handle_id, str, out_ptr_width, out_ptr_height) {
    // may need to pass in object type, font family, font size

    var widget_object = IupCommon.GetObjectForID(handle_id);
    var temp_object = document.createElement("div");
    document.body.appendChild(temp_object);
    var user_str = Pointer_stringify(str);

    if (widget_object) {
      temp_object.style.fontFamily = widget_object.style.fontFamily;
      temp_object.style.fontSize = widget_object.style.fontSize;
    }
    if (widget_object.nodeName == 'button') {
      var tmp_button = document.createElement("button");
      tmp_button.style.position = 'absolute';
      tmp_button.innerHTML = user_str;
      temp_object.appendChild(tmp_button);
    }
    else {
      temp_object.innerHTML = user_str;
    }


    // 1. check if widget object is a button
    // 2. if its a button, label, etc, query the browser - what is the default font we should be using

    temp_object.style.position = 'absolute';

    var w = Math.ceil(temp_object.clientWidth);
    var h = Math.ceil(temp_object.clientHeight);
    Module.setValue(out_ptr_width,w,'i32');
    Module.setValue(out_ptr_height,h,'i32');

    console.log("WIDGET STATS: w-" + w +" h:" + h);
    document.body.removeChild(temp_object);
  },


  emjsFont_GetCharSize: function(handle_id, out_ptr_width, out_ptr_height) {

    var widget_object = IupCommon.GetObjectForID(handle_id);
    var temp_object;
    temp_object = document.createElement("div");
    document.body.appendChild(temp_object);

    if(widget_object) {
      temp_object.style.fontFamily = widget_object.style.fontFamily;
      temp_object.style.fontSize = widget_object.style.fontSize;
    }

    temp_object.innerHTML = "WWWWWWWW";
    temp_object.style.position = 'absolute';

    var w = Math.ceil(temp_object.clientWidth/8);
    var h = Math.ceil(temp_object.clientHeight);
    Module.setValue(out_ptr_width,w,'i32');
    Module.setValue(out_ptr_height,h,'i32');

    document.body.removeChild(temp_object);
  }


};

autoAddDeps(LibraryIupFont, '$Font');
mergeInto(LibraryManager.library, LibraryIupFont);
