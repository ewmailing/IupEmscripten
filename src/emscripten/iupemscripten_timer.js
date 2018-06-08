var LibraryIupTimer = {
  // $Button__deps: ['$CommonGlobals'],
  $Timer: {
  },

  emjsTimer_CreateTimer: function() {
    console.log('timer is loading');
    var myfunc = function() { console.log('timer is firing'); };
    setTimeout(myfunc, 2000);
  }
};

autoAddDeps(LibraryIupTimer, '$Timer');
mergeInto(LibraryManager.library, LibraryIupTimer);
