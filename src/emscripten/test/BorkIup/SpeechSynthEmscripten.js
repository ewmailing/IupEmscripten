

/*global document alert IupCommon Pointer_stringify IupCommon autoAddDeps mergeInto LibraryManager*/

var LibrarySpeechSynth = {
	//	$Button__deps: ['$CommonGlobals'],
	$SpeechSynth:
	{
	},

	emjsSpeechSynth_Init: function()
	{

	},

	emjsSpeechSynth_Quit: function()
	{

	},

	emjsSpeechSynth_SayString: function(input_string)
	{
        var js_string = Pointer_stringify(input_string);
		console.log("Saying: " + js_string);
		var utterance = new SpeechSynthesisUtterance(js_string);
	
		utterance.onend = function(the_event)
		{
			console.log('Finished in ' + the_event.elapsedTime + ' seconds.');
			// invoke finished callback in C
			_SpeechEmscripten_FinishedCallbackTrampoline();
		};
		
		window.speechSynthesis.speak(utterance);
	}


};

autoAddDeps(LibrarySpeechSynth, '$SpeechSynth');
mergeInto(LibraryManager.library, LibrarySpeechSynth);

