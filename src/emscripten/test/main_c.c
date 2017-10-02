#include "iup.h"
#include "iup_config.h"
#include "iupcbs.h"
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

void testConfigFile()
{
		int ret_val = 0;	
		Ihandle* config_file = IupConfig();
		IupSetStrAttribute(config_file, "APP_NAME", "TestApp");
		ret_val = IupConfigLoad(config_file);

		const char* config_value;
		if(ret_val == 0)
		{
			const char* config_value = IupConfigGetVariableStrDef(config_file, "Group1", "Key1", "");
			printf("IDBFS worked: found existing config file: config value is %s\n", config_value);
		}
		else
		{
			printf("config file not found\n");
		}
		IupConfigSetVariableStr(config_file, "Group1", "Key1", "Value1");
		IupConfigSave(config_file);
		config_value = IupConfigGetVariableStrDef(config_file, "Group1", "Key1", "");
		printf("retrieved saved config value is %s\n", config_value);



		IupDestroy(config_file);
		config_file = NULL;

}

Ihandle* DropDownListTest(void)
{
  Ihandle* list2;

  list2 = IupList(NULL);
  //IupSetAttributes(list2, "1=\"Banana\", 2=\"Apple\", 3=\"Orange\", 4=\"Strawberry Shortcake vs. B. Pudding\", 5=\"Grape\","
  // "DROPDOWN=YES, NAME=list2, TIP=Drop, XXX_VALUE=2, XXX_SORT=YES, XXX_BGCOLOR=\"192 64 192\"");
  IupSetAttributes(list2,"1=\"Banana\", 2=\"Apple\", 3=\"Orange\", 4=\"Strawberry Shortcake vs. B. Pudding\", 5=\"Grape\","
                   "DROPDOWN=NO, EDITBOX=NO, MULTIPLE=YES, TIP=List, NAME=list2");
  //set_callbacks(list2);

  Ihandle* dialog = IupDialog(list2);
  IupShow(dialog);

  return dialog;
}

/* void IupEntryPoint() */
/* { */
/*   DropDownListTest(); */
/* } */

void IupEntryPoint()
{

	{
		testConfigFile();
	}


  // create test buttons
	Ihandle* button = IupButton("Iup Button", "");
  Ihandle* button2 = IupButton("Callback", "");
  // create test labels
  Ihandle* testLabel = IupLabel("Iup Button");
  Ihandle* testLabel2 = IupLabel("Callback");
  Ihandle* testLabel3 = IupLabel("TestLabel----3");
  // create test list
  Ihandle* testList = IupList(NULL);
  /* IupSetAttributes(testList, "1=Gold, 2=Silver, 3=Bronze, 4=Latão, 5=None," */
  /*                         "DROPDOWN=YES, NAME=testList"); */

  IupSetAttributes(testList, "1=Measurements,"
                   "DROPDOWN=YES, NAME=testList");
  Ihandle* testList2 = IupList(NULL);
  /* IupSetAttributes(testList2, "1=Gold, 2=Silver, 3=Bronze, 4=Latão, 5=None," */
  /*                  "DROPDOWN=YES, EDITBOX=YES, NAME=testList2"); */
  IupSetAttributes(testList2, "1=\"US$ 1000\", 2=\"US$ 2000\", 3=\"US$ 300.000.000\", 4=\"Strawberry Shortcake vs. B. Pudding\","
                   "EDITBOX=NO, DROPDOWN=NO, MULTIPLE=YES, VALUE=\"Edit Here\", NAME=list1");
                   /* "DROPDOWN=YES, NAME=testList2"); */

  /* IupSetAttribute(testList, "DRAGSOURCE", "YES"); */
  /* IupSetAttribute(testList, "DRAGTYPES", "ITEMLIST"); */

  /* Ihandle* frm_medal1 = IupFrame(testList); */
  /* IupSetAttribute(frm_medal1, "TITLE", "List 1"); */

  // test bg and fg color with labels

  /* IupSetAttribute(testLabel, "POSITION", "400x400"); */
  /* IupSetAttribute(testLabel2, "POSITION", "50x50"); */
  /* IupSetAttribute(testLabel3, "POSITION", "100x100"); */

  IupSetAttribute(button, "WIDTH", "50px");
  IupSetAttribute(button, "HEIGHT", "50px");
  IupSetAttribute(button2, "WIDTH", "50px");
  IupSetAttribute(button2, "HEIGHT", "50px");

  //IupSetAttribute(testLabel, "BGCOLOR", "20 129 115");

  // attach callbacks to test buttons
	IupSetCallback(button, "ACTION", (Icallback)OnButtonCallback);
  IupSetCallback(button2, "BUTTON_CB", (Icallback)testCallback);

  // more label-attrib setting, testing in different places (i know its dumb, c is synchronous..)
  IupSetAttribute(testLabel2, "FGCOLOR", "100 255 100");
  IupSetAttribute(testLabel3, "FGCOLOR", "70 70 0");
  /* IupSetAttribute(testLabel, "TITLE", "OVERWRITE"); */

  /* IupSetAttribute(testLabel2, "TITLE", "CONGRATS"); */

	Ihandle* vb=IupHbox(button, button2, testLabel, testLabel2, testLabel3, testList, testList2, NULL);

	/* Ihandle* vb=IupHbox(testList, NULL); */

	/* IupSetAttribute(vb, "GAP", "10"); */
	/* IupSetAttribute(vb, "MARGIN", "10x10"); */
	/* IupSetAttribute(vb, "ALIGNMENT", "ACENTER"); */

	/* Ihandle* hb=IupHbox(testLabel, testLabel2, NULL); */

	Ihandle* dialog = IupDialog(vb);

	//	IupMap(dialog);
	IupSetAttribute(dialog, "TITLE", "Iup Activity Title");

	IupShow(dialog);
}


int main(int argc, char* argv[])
{
	IupOpen(&argc, &argv);
	IupSetFunction("ENTRY_POINT", (Icallback)IupEntryPoint);
	IupMainLoop();
	return 0;
}

