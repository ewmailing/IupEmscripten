

var LibraryIupCommon = {
//	$Common__deps: ['$CommonGlobals'],
	$IupCommonGlobals: {
		idCounter: 1,
		objectIDMap: {},
	},

	$IupCommon: {


		GetObjectForID: function(handle_id) {
			return IupCommonGlobals.objectIDMap[handle_id];
		},
		SetObjectForID: function(the_object, handle_id) {
			IupCommonGlobals.objectIDMap[handle_id] = the_object;
		},
		RegisterNewObject: function(the_object) {
			var current_id = IupCommonGlobals.idCounter;
			IupCommonGlobals.objectIDMap[current_id] = the_object;
			the_object.handleID = current_id;
			IupCommonGlobals.idCounter += 1;
			return current_id;
		},
		DeleteObject: function(handle_id) {
			var the_object = IupCommonGlobals.objectIDMap[handle_id] = null;
			the_object.handleID = null;
			IupCommonGlobals.objectIDMap[handle_id] = null;
			the_object = null;
		},
	},

	emjsCommon_AddWidgetToDialog: function(parent_id, child_id) {
		var parent_dialog = IupCommon.GetObjectForID(parent_id);
		var parent_body = parent_dialog.document.getElementsByTagName("body")[0];
		var child_widget = IupCommon.GetObjectForID(child_id);
		parent_body.appendChild(child_widget);
	},

	emjsCommon_AddWidgetToWidget: function(parent_id, child_id) {
		var parent_widget = IupCommon.GetObjectForID(parent_id);
		var child_widget = IupCommon.GetObjectForID(child_id);

		parent_widget.appendChild(child_widget);
	},

  emjsCommon_Alert: function(message) {
    console.log("alert message: ");
    console.log(message);
    alert(Pointer_stringify(message));
  },

  emjsCommon_SetFgColor: function(handle_id, r, g, b)
  {
    var current_widget = IupCommon.GetObjectForID(handle_id);
    current_widget.style.color = "red";
  },

  iupEmscriptenSetFgColor: function(handle_id, r, g, b) {
	  var sel_object = IupCommon.GetObjectForID(handle_id);
    sel_object.style.color = "rgb(" + r + "," + g + "," + b + ")";
  }

};

autoAddDeps(LibraryIupCommon, '$IupCommonGlobals');
autoAddDeps(LibraryIupCommon, '$IupCommon');
mergeInto(LibraryManager.library, LibraryIupCommon);
