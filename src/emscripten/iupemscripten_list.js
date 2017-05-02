/*global document alert IupCommon Pointer_stringify IupCommon autoAddDeps mergeInto LibraryManager*/

var LibraryIupList = {
    //	$Button__deps: ['$CommonGlobals'],
    $List: {
    },

    emjsList_CreateList: function() {
        console.log("Create list fired");
        var widget_object;
        widget_object = document.createElement("select");
        var handle_id = IupCommon.RegisterNewObject(widget_object);
        widget_object.id = 'list_' + handle_id;
        return handle_id;
    },

    emjsList_DestroyList: function(handle_id) {
        // Do I need to removeEventListener?
        IupCommon.DeleteObject(handle_id);
    },
    emjsList_GetCount: function(handle_id, sub_type) {
        var widget_object = IupCommon.GetObjectForID(handle_id);
        switch(sub_type) {
            case 1:
                console.log("dropdown");
                return handle_id.length;
            case 2:
                console.log("editboxdropdown");
                return handle_id.length;
            case "IUPEMSCRIPTENLISTSUBTYPE_MULTIPLELIST":
            case "IUPEMSCRIPTENLISTSUBTYPE_SINGLELIST":
                console.log("multiple or single list");
                return handle_id.length;
            case "IUPEMSCRIPTENLISTSUBTYPE_EDITBOX":
                return "";
            default:
                return 0;
        }
    }
};

autoAddDeps(LibraryIupList, '$List');
mergeInto(LibraryManager.library, LibraryIupList);

