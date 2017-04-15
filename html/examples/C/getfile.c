/*  IupGetFile: Example in C 
    Shows a typical file-selection dialog.
*/

#include <stdlib.h>
#include <stdio.h>
#include <iup.h>

int main(int argc, char **argv)
{
  char file[10240];

  IupOpen(&argc, &argv);
  IupSetLanguage("ENGLISH");

  strcpy(file, "*.txt");

  switch(IupGetFile(file))
  {
    case 1: 
      IupMessage("New file",file);	    
      break;	    
    case 0 : 
      IupMessage("File already exists",file);
      break;	    
    case -1 : 
      IupMessage("IupFileDlg","Operation canceled");
      break;
    case -2 : 
      IupMessage("IupFileDlg","Allocation error");
      break;
    case -3 : 
      IupMessage("IupFileDlg","Invalid parameter");
      break;	    
  }

  IupClose();

  return EXIT_SUCCESS;
}
