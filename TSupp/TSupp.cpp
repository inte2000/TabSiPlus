// TSupp.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "TSupp.h"

BOOL APIENTRY DllMain( HANDLE hModule,DWORD  ul_reason_for_call,LPVOID lpReserved)
{
  switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
//@**#---2006-05-14 22:06:24 (oRbIt)---#**@
//            ::DisableThreadLibraryCalls((HMODULE)hModule);
//            break;
        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}


// This is an example of an exported variable
TSUPP_API int nTSupp=0;

// This is an example of an exported function.
TSUPP_API int fnTSupp(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see TSupp.h for the class definition
CTSupp::CTSupp()
{ 
    return; 
}

