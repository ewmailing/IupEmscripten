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
            case 0:
                console.log("dropdown");
          console.log(widget_object.length);
                return widget_object.length;
            case 1:
                console.log("editboxdropdown");
                return widget_object.length;
            case 2:
            case 3:
                console.log("multiple or single list");
                return widget_object.length;
            case 4:
                return "";
            default:
                return 0;
        }
    },

  emjsList_AppendItem: function(handle_id, sub_type, value) {
        var widget_object = IupCommon.GetObjectForID(handle_id);

        switch(sub_type) {
        case 0:
            console.log("dropdown2");
          console.log(value);
            var item = document.createElement('option');
            item.innerHTML = Pointer_stringify(value);
            widget_object.appendChild(item);
          return "";
        case 1:
            console.log("editboxdropdown2");
            return widget_object.length;
        case 2:
        case 3:
            console.log("multiple or single list2");
            return widget_object.length;
        case 4:
            return "";
        default:
            return 0;
        }
    }
};

autoAddDeps(LibraryIupList, '$List');
mergeInto(LibraryManager.library, LibraryIupList);
