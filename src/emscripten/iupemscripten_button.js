


var LibraryIupButton = {
//	$Button__deps: ['$CommonGlobals'],
	$Button: {
		idCounter: 1,
		objectIDMap: {},
	},


	emjsButton_CreateButton: function() {

		var widget_object;

		var current_id = Button.idCounter;
		Button.idCounter++;
		widget_object = document.createElement("button");


		Button.objectIDMap[current_id] = widget_object;

		// Save id with object just in case we need it
		widget_object.handleID = current_id;
		var body = document.getElementsByTagName("body")[0];
		body.appendChild(widget_object);

		return current_id;

	},


	emjsButton_DestroyButton: function(handle_id) {
		var widget_object = Button.objectIDMap[handle_id];
		if(widget_object)
		{
			widget_object.close();
			Button.objectIDMap[handle_id] = null;
		}
	},

	emjsButton_SetTitle: function(handle_id, button_title) {
		var widget_object = Button.objectIDMap[handle_id];
		if(widget_object)
		{
			widget_object.innerHTML = Pointer_stringify(button_title);
		}
	},

	emjsButton_SetCallback: function(handle_id, ih_pointer) {

		var button = Button.objectIDMap[handle_id];
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
				alert("hi");

			});

		}

	},


}

autoAddDeps(LibraryIupButton, '$Button');
mergeInto(LibraryManager.library, LibraryIupButton);

