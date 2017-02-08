
var LibraryIupLabel = {
//	$Button__deps: ['$CommonGlobals'],
	$Label: {
	},


	emjsButton_CreateButton: function() {

		var widget_object;
		widget_object = document.createElement("button");
		var handle_id = IupCommon.RegisterNewObject(widget_object);
		return handle_id;
	},


	emjsButton_DestroyButton: function(handle_id) {
		// Do I need to removeEventListener?
		IupCommon.DeleteObject(handle_id);
	},

	emjsLabel_SetTitle: function(handle_id, button_title) {
		var widget_object = IupCommon.GetObjectForID(handle_id);
		if(widget_object)
		{
			widget_object.innerHTML = Pointer_stringify(button_title);
		}
	},

};

autoAddDeps(LibraryIupLabel, '$Label');
mergeInto(LibraryManager.library, LibraryIupLabel);

