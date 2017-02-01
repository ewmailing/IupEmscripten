//@ sourceURL=iupemscripten_button.js


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
        //	Add 'type' char* to function calls??
        var c_callback = Module.cwrap('emscriptenButtonCallbackTrampoline', null, ['number', 'string']); //what do i need to do in array?
				//c_callback(handle_id, ih_pointer);
        console.log("action: " + c_callback);
				c_callback(handle_id, "action");
				//c_callback(handle_id, 0);
				//c_callback(handle_id, button.ihPointer);
				//alert("hi");

			});
      // for BUTTON_CB, need on mousedown and onmouseup
      button.addEventListener("mousedown", function() {
				var c_callback = Module.cwrap('emscriptenButtonCallbackTrampoline', null, ['number', 'string']);
        console.log("onmousedown: " + c_callback);
				c_callback(handle_id, "buttoncb");
      });
      button.addEventListener("mouseup", function() {
				var c_callback = Module.cwrap('emscriptenButtonCallbackTrampoline', null, ['number', 'string']);
        console.log("onmouseup: " + c_callback);
				c_callback(handle_id, "buttoncb");
      });
		}

	},


}

autoAddDeps(LibraryIupButton, '$Button');
mergeInto(LibraryManager.library, LibraryIupButton);

