


var LibraryIupButton = {
//	$Button__deps: ['$CommonGlobals'],
	$Button: {
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

	emjsButton_SetTitle: function(handle_id, button_title) {
		var widget_object = IupCommon.GetObjectForID(handle_id);
		if(widget_object)
		{
			widget_object.innerHTML = Pointer_stringify(button_title);
		}
	},

	emjsButton_SetCallback: function(handle_id, ih_pointer) {

		var button = IupCommon.GetObjectForID(handle_id);
//button.ihPointer = ih_pointer;
		if(button)
		{
			button.addEventListener("click", function() { 
			//	var c_callback = Module.cwrap('emscriptenButtonCallbackTrampoline', null, ['number', 'number']);
				var c_callback = Module.cwrap('emscriptenButtonCallbackTrampoline', null, ['number']);
				//c_callback(handle_id, ih_pointer);
				c_callback(handle_id);
				//c_callback(handle_id, 0);
				//c_callback(handle_id, button.ihPointer);
				//alert("hi");

			});

		}

	},


}

autoAddDeps(LibraryIupButton, '$Button');
mergeInto(LibraryManager.library, LibraryIupButton);

