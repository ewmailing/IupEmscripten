/** \file
 * \brief Windows Driver IupHelp
 *
 * See Copyright Notice in "iup.h"
 */

#include <windows.h>
#include <shellapi.h>

#include <stdlib.h>
#include <stdio.h>

#include "iup.h"

#ifndef SEE_MASK_NOASYNC
#define SEE_MASK_NOASYNC 0x00000100
#endif

int IupExecute(const char *filename, const char* parameters)
{
  /* no need to convert from UTF8 here */
  int err = (int)ShellExecuteA(GetDesktopWindow(), "open", filename, parameters, NULL, SW_SHOWNORMAL);
  if (err <= 32)
  {
    switch (err)
    {
    case ERROR_FILE_NOT_FOUND:
    case ERROR_PATH_NOT_FOUND:
      return -2; /* File not found */
    default:
      return -1; /* Generic error */
    }
  }
  return 1;
}

int IupExecuteWait(const char *filename, const char* parameters)
{                                          
  /* no need to convert from UTF8 here */
  SHELLEXECUTEINFOA ExecInfo;
  memset(&ExecInfo, 0, sizeof(SHELLEXECUTEINFOA));

  ExecInfo.cbSize = sizeof(SHELLEXECUTEINFOA);
  ExecInfo.fMask = SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI | SEE_MASK_NO_CONSOLE;
  ExecInfo.hwnd = GetDesktopWindow();
  ExecInfo.lpVerb = "open";
  ExecInfo.lpFile = filename;
  ExecInfo.lpParameters = parameters;
  ExecInfo.nShow = SW_SHOWNORMAL;

  if (!ShellExecuteExA(&ExecInfo))
  {
    int err = (int)ExecInfo.hInstApp;
    switch (err)
    {
    case SE_ERR_FNF:
    case SE_ERR_PNF:
      return -2; /* File not found */
    default:
      return -1; /* Generic error */
    }
  }

  WaitForSingleObject(ExecInfo.hProcess, INFINITE);

  return 1;
}

int IupHelp(const char* url)
{
  return IupExecute(url, NULL);
}
