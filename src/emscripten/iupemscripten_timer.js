var LibraryIupTimer = {
  // $Button__deps: ['$CommonGlobals'],
  $Timer: {
  },

  emjsTimer_CreateTimer: function(time_ms) {
    var startDate = new Date();
    var startTime = startDate.getTime();
    var timerCallback = function(startTime) {
      var nowDate = new Date();
      var nowTime = nowDate.getTime();
      var elapsedTime = nowTime - startTime;
      console.log(`timer ${time_ms} is firing`);
      var c_callback = Module.cwrap('emscriptenTimerCallbackTrampoline', null, ['number', 'number']);
      c_callback(handle_id);
    };
    var timerId = setInterval(timerCallback, time_ms, startTime);
    console.log(timerId);

    var handle_id = IupCommon.RegisterNewObject(timerId);

    return handle_id;
  },

  emjsTimer_DestroyTimer: function(handle_id) {
    var timerId = IupCommon.GetObjectForID(handle_id);
    clearInterval(timerId);
  },
  
};

autoAddDeps(LibraryIupTimer, '$Timer');
mergeInto(LibraryManager.library, LibraryIupTimer);
