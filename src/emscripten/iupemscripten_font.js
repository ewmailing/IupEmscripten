/*global document alert IupCommon Pointer_stringify IupCommon autoAddDeps mergeInto LibraryManager*/

var LibraryIupFont = {
  //	$Button__deps: ['$CommonGlobals'],
  $Font: {
  },

  emjsFont_GetStringWidth: function(ihandle, handle_id, str) {

    var widget_object = IupCommon.GetObjectForID(handle_id);
    var temp_object;

    if (widget_object.nodeName == 'SELECT') {
      temp_object = document.createElement("select");
      var tmp_option = document.createElement("option");
      tmp_option.innerHTML = Pointer_stringify(str);
      temp_object.appendChild(tmp_option);
      document.body.appendChild(temp_object);
    }

    else {
      temp_object = document.createElement("div");
      document.body.appendChild(temp_object);
      temp_object.innerHTML = Pointer_stringify(str);
    }


    if(widget_object) {
      temp_object.style.fontFamily = widget_object.style.fontFamily;
      temp_object.style.fontSize = widget_object.style.fontSize;
    }
    temp_object.style.position = 'absolute';

    var w = Math.ceil(temp_object.clientWidth);
    console.log("WHOLE WIDGET height: " + temp_object.clientHeight);

    // current bug in firefox - not returning true size of entire select element
    // (insead returning size of largest option string)
    // reported: https://bugzilla.mozilla.org/show_bug.cgi?id=1398613
    if (navigator.userAgent.indexOf("Firefox") != -1) {
      w += 27;
    }

    console.log("single string width: " + w);
    console.log(temp_object.innerHTML);

    document.body.removeChild(temp_object);

    return w;
  },


  emjsFont_GetMultiLineStringSize: function(ihandle, handle_id, str, out_ptr_width, out_ptr_height) {
    // may need to pass in object type, font family, font size

    var widget_object = IupCommon.GetObjectForID(handle_id);
    var temp_object = document.createElement("div");
    document.body.appendChild(temp_object);
    var user_str = Pointer_stringify(str);

    if (widget_object) {
      temp_object.style.fontFamily = widget_object.style.fontFamily;
      temp_object.style.fontSize = widget_object.style.fontSize;
    }
    // cannot change button's position within div - messes up calculation
    if (widget_object.nodeName == 'BUTTON') {
      var tmp_button = document.createElement("button");
      tmp_button.innerHTML = user_str;
      temp_object.appendChild(tmp_button);
    }
    // need to add new widgets in here to compute size correctly

    // same methods for widgets above won't work for lists - we need to do what iup expects us to do
    // iup expects to give us one string at a time, and we return the string size
    // iup will look at all the items it needs for the list, and it will construct it based on what it thinks it needs to be
    // case one is dropdown, case two is multi-list view (table view)
    else {
      temp_object.innerHTML = user_str;
    }


    // 1. check if widget object is a button
    // 2. if its a button, label, etc, query the browser - what is the default font we should be using

    temp_object.style.position = 'absolute';

    var w = Math.ceil(temp_object.offsetWidth);
    var h = Math.ceil(temp_object.offsetHeight);
    Module.setValue(out_ptr_width,w,'i32');
    Module.setValue(out_ptr_height,h,'i32');

    document.body.removeChild(temp_object);
  },


  emjsFont_GetCharSize: function(ihandle, handle_id, out_ptr_width, out_ptr_height) {

    var widget_object = IupCommon.GetObjectForID(handle_id);
    var temp_object;
    var w;
    var h;

    if (widget_object.nodeName == 'SELECT') {
      temp_object = document.createElement("select");
      var tmp_option = document.createElement("option");
      tmp_option.innerHTML = "WWWWWWWW";

      temp_object.appendChild(tmp_option);
      document.body.appendChild(temp_object);

      temp_object.style.fontFamily = widget_object.style.fontFamily;
      temp_object.style.fontSize = widget_object.style.fontSize;

      w = Math.ceil(tmp_option.clientWidth/8);
      var sub_type = _emscriptenListGetSubType(ihandle);

      /*
      subtype list (for reference):
      IUPEMSCRIPTENLISTSUBTYPE_UNKNOWN = -1,
      IUPEMSCRIPTENLISTSUBTYPE_DROPDOWN,
      IUPEMSCRIPTENLISTSUBTYPE_EDITBOXDROPDOWN,
      IUPEMSCRIPTENLISTSUBTYPE_EDITBOX,
      IUPEMSCRIPTENLISTSUBTYPE_MULTIPLELIST,
      IUPEMSCRIPTENLISTSUBTYPE_SINGLELIST
      */

      if (sub_type == 3) {
        if (navigator.userAgent.indexOf("Firefox") != -1) {
          h = Math.ceil(temp_object.clientHeight - 4);
        }
        else if (navigator.userAgent.indexOf("Chrome") != -1) {
          h = Math.ceil(temp_object.clientHeight - 5);
        }
        else if (navigator.userAgent.indexOf("Safari") != -1) {
          h = Math.ceil(temp_object.clientHeight - 5);
        }
        else {
          h = Math.ceil(temp_object.clientHeight);
        }
      }
      else {
        h = Math.ceil(temp_object.clientHeight);
      }
    }

    else {
      temp_object = document.createElement("div");
      document.body.appendChild(temp_object);

      if (widget_object) {
        temp_object.style.fontFamily = widget_object.style.fontFamily;
        temp_object.style.fontSize = widget_object.style.fontSize;
      }

      temp_object.innerHTML = "WWWWWWWW";

      w = Math.ceil(temp_object.clientWidth/8);
      h = Math.ceil(temp_object.clientHeight);
    }

    temp_object.style.position = 'absolute';

    Module.setValue(out_ptr_width,w,'i32');
    Module.setValue(out_ptr_height,h,'i32');

    document.body.removeChild(temp_object);
  }


};

autoAddDeps(LibraryIupFont, '$Font');
mergeInto(LibraryManager.library, LibraryIupFont);