


var LibraryIupDialog = {
//	$Dialog__deps: ['$CommonGlobals'],
	$Dialog: {
		hasCreatedFirstDialog: false,
		idCounter: 0,
		objectIDMap: {},
	},


	emjsDialog_CreateDialog: function() {

		var dialog;

		Dialog.counter++;
		var current_id = Dialog.counter;

		if(!Dialog.hasCreatedFirstDialog)
		{
			//dialog = document.getElementsByTagName("body")[0];
			dialog = window.self;
			Dialog.hasCreatedFirstDialog = true;
		}
		else
		{
			dialog = window.open();
		}

		Dialog.objectIDMap[current_id] = dialog;

		// Save id with object just in case we need it
		dialog.handleID = current_id;

		return current_id;

	},


	emjsDialog_DestroyDialog: function(handle_id) {
		var dialog = Dialog.objectIDMap[handle_id];
		if(dialog)
		{

			dialog.close();
			Dialog.objectIDMap[handle_id] = null;
		}
	}


}

autoAddDeps(LibraryIupDialog, '$Dialog');
mergeInto(LibraryManager.library, LibraryIupDialog);

