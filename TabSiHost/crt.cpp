/// CRT simple implementation

#include "StdAfx.h"

/*
 /OPT:NOWIN98 /ENTRY:MyCRTStartup
*/
//extern "C" { int _afxForceEXCLUDE; }

//kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib
#pragma  comment (lib, "kernel32.lib")
#pragma  comment (lib, "advapi32.lib")
#pragma  comment (lib, "user32.lib")
#pragma  comment (lib, "shell32.lib")
#pragma  comment (lib, "Shlwapi.lib")


extern int MainFunction();

extern "C" void MyCRTStartup()
{
    int nRet = MainFunction();
    ::ExitProcess((UINT)nRet);
}
