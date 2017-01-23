


var LibraryIupDialog = {
//	$Dialog__deps: ['$CommonGlobals'],
	$Dialog: {
		hasCreatedFirstDialog: false,
	},


	emjsDialog_CreateDialog: function(window_name, width, height) {

		var dialog;

		if(Dialog.hasCreatedFirstDialog)
		{
			var new_window_name = null;
			if(window_name)
			{
				new_window_name = Pointer_stringify(window_name);
			}
			if(0 === width)
			{
				width = window.self.width;
			}
			if(0 === height)
			{
				height = window.self.height;
			}

			var attrib_str = "width=" + width + ",height=" + height;
			dialog = window.open("", new_window_name, attrib_str);
		}
		else
		{
			//dialog = document.getElementsByTagName("body")[0];
			dialog = window.self;
			Dialog.hasCreatedFirstDialog = true;

			if(window_name)
			{
				dialog.document.title = Pointer_stringify(window_name);
			}


		}

		current_id = IupCommon.RegisterNewObject(dialog);

		return current_id;

	},


	emjsDialog_DestroyDialog: function(handle_id) {

		var dialog = IupCommon.GetObjectForID(handle_id);
		if(dialog)
		{

			dialog.close();
			IupCommon.DeleteObjectWithID(handle_id);
		}
	}


}

autoAddDeps(LibraryIupDialog, '$Dialog');
mergeInto(LibraryManager.library, LibraryIupDialog);

