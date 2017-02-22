#include "iup.h"
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

#if __ANDROID__
#include <android/log.h>

void MyPrintf(const char* fmt, ...)
{
	va_list ap;	
	va_start(ap, fmt);
	__android_log_vprint(ANDROID_LOG_INFO, "IupTest", fmt, ap);
	va_end(ap);
}
#else
void MyPrintf(const char* fmt, ...)
{
	va_list ap;	
	va_start(ap, fmt);
	fprintf(stderr, fmt, ap);
	va_end(ap);
}
#endif
int OnButtonCallback(Ihandle* ih)
{
	MyPrintf("OnButtonCallback()\n");

	char temp_string[1024];
	static int button_count = 0;
  // Stores button text in string with updated count
	snprintf(temp_string, 1024, "Iup Button %d", button_count);
  // Create button element
	Ihandle* button = IupButton(temp_string, "");

  // Set recursive callback so a new window with button opens
	IupSetCallback(button, "ACTION", (Icallback)OnButtonCallback);
	Ihandle* dialog = IupDialog(button);
	snprintf(temp_string, 1024, "Iup Activity Title %d", button_count);

	IupSetStrAttribute(dialog, "TITLE", temp_string);
	
	IupShow(dialog);

	button_count++;

	return IUP_DEFAULT;
}

int testCallback(Ihandle* ih, int but, int pressed, int x, int y, char* status) {
	char temp_string[1024];
	Ihandle* button = IupButton("CB Test", "");

  // Attach recursive callback
	IupSetCallback(button, "BUTTON_CB", (Icallback)testCallback);
	Ihandle* dialog = IupDialog(button);
	snprintf(temp_string, 1024, "Iup Activity Title");

	IupSetStrAttribute(dialog, "TITLE", temp_string);
	
  // ?? why does this not open a new window like OnButtonCallback? ?? 
	IupShow(dialog);

	return IUP_DEFAULT;
}

int IupEntryPoint(Ihandle* noop)
{
//	IupOpen(0, NULL);
	Ihandle* button = IupButton("Iup Button", "");
  Ihandle* button2 = IupButton("Callback", "");
  Ihandle* testLabel = IupLabel("TestLabel");
	IupSetCallback(button, "ACTION", (Icallback)OnButtonCallback);
  IupSetCallback(button2, "BUTTON_CB", (Icallback)testCallback);

	Ihandle* vb=IupVbox(button, button2, testLabel, NULL);
	IupSetAttribute(vb, "GAP", "10");
	IupSetAttribute(vb, "MARGIN", "10x10");
	IupSetAttribute(vb, "ALIGNMENT", "ACENTER");

	Ihandle* dialog = IupDialog(vb);
	//	IupMap(dialog);
	IupSetAttribute(dialog, "TITLE", "Iup Activity Title");

	IupShow(dialog);
	return 0;
}


int main(int argc, char* argv[])
{
	IupOpen(NULL, NULL);
	IupSetFunction("ENTRY_POINT", IupEntryPoint);
	IupMainLoop();
	return 0;
}
