// TabSiHost.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "LoadDll.h"
#include "SplashWnd.h"
#include "Resource.h"

#include "../Common/Localization.h"
#include "../Common/SourceInsight.h"
#include "../Common/Options.h"

DWORD    dwWindowsNTMajorVersion = 0;

HANDLE hAnotherTabSiHostEvent = NULL;
LPCTSTR szGlobalKernelName = _T("Local\\TabSiHostIsAlreadyRunning");


//////////////////////////////////////////////////////////////////////////

BOOL IsAnotherTabSiHost_Capture( BOOL bInitialState )
{
    if( hAnotherTabSiHostEvent != NULL )
    {
        return TRUE;
    }
    hAnotherTabSiHostEvent = CreateEvent( NULL, TRUE, bInitialState, szGlobalKernelName );    

    DWORD dwer = GetLastError();    

    return (dwer==ERROR_ALREADY_EXISTS);
}

//////////////////////////////////////////////////////////////////////////

void ReleaseAnotherTabSiHost()
{
    if( hAnotherTabSiHostEvent != NULL )
    {
        CloseHandle( hAnotherTabSiHostEvent );
        hAnotherTabSiHostEvent = NULL;
    }
}

LPCTSTR GetAppPathName(LPCTSTR lpszFileName, LPTSTR lpszFullNameBuf, int nBufSize)
{
    ASSERT(lpszFullNameBuf);

    DWORD res = ::GetModuleFileName(NULL,lpszFullNameBuf,nBufSize);
    if(res == 0)
        return NULL;

    LPTSTR p = NULL;
    for( int i = lstrlen(lpszFullNameBuf) - 1; i >= 0; i-- )
    {
        if(lpszFullNameBuf[i] == _T('\\') )
        {
            p = &lpszFullNameBuf[i];
            break;
        }
    }

    if(p == NULL)
    {
       p = lpszFullNameBuf;
    }
    else
    {
       p++;
    }

    lstrcpy(p,lpszFileName);

    return lpszFullNameBuf;
}


//////////////////////////////////////////////////////////////////////////

DWORD MyWaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds) // for simplier debug
{
    DWORD res = ::WaitForSingleObject(hHandle, dwMilliseconds);
    return res;
}

//////////////////////////////////////////////////////////////////////////

TCHAR szDllPath[_MAX_PATH] = _T(""); // TabSiPlus.dll name


//////////////////////////////////////////////////////////////////////////
VOID CALLBACK ThreadTimerProc(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime)
{
    hwnd = hwnd;
    uMsg = uMsg;
    idEvent = idEvent;
    dwTime = dwTime;

#ifdef _DEBUG
    OutputDebugString(_T("TabSiHost > ThreadTimerProc before FindAndHookSourceInsightWindow()\r\n"));
#endif

    BOOL bSuccess = FindAndHookSourceInsightWindow(szDllPath);
    if(bSuccess)
    {
        BOOL bRunOnceOnly = FALSE;
        DWORD dwLoadOnce;
        if(GetDwordRegPorpValue(lpszKeyRoot,lpszLoadOncePropName,dwLoadOnce))
        {
            bRunOnceOnly = (dwLoadOnce != 0);
        }

        if(bRunOnceOnly)
        {
            ::ResetEvent(hAnotherTabSiHostEvent); // Stop process after one Dll load
        }
    }

    DWORD res = MyWaitForSingleObject(hAnotherTabSiHostEvent, 0);
    if( res != WAIT_OBJECT_0 )
    {
        ::PostQuitMessage(0);
    }
}

//////////////////////////////////////////////////////////////////////////

int MainFunction()
{
    SetRegistryRootKey(HKEY_CURRENT_USER);

    BOOL res;
    DWORD dwBufSize;

    TCHAR szCaption[128] = _T("");
    TCHAR szRequireNT[256] = _T("");
    TCHAR szGetModuleFilenameError[256] = _T("");
    TCHAR szCommandLineHelp[1024] = _T("");

    dwBufSize = SIZEOF_ARRAY(szCaption);
    LocLoadString(IDS_MESSAGEBOX_CAPTION, szCaption, &dwBufSize);
    dwBufSize = SIZEOF_ARRAY(szRequireNT);
    LocLoadString(IDS_REQUIRE_NT, szRequireNT, &dwBufSize);
    dwBufSize = SIZEOF_ARRAY(szGetModuleFilenameError);
    LocLoadString(IDS_GETMODULEFILNAME_ERROR, szGetModuleFilenameError, &dwBufSize);
    dwBufSize = SIZEOF_ARRAY(szCommandLineHelp);
    LocLoadString(IDS_COMMANDLINE_HELP, szCommandLineHelp, &dwBufSize);

    LPWSTR szCommandLineW = ::GetCommandLineW(); //CommandLineToArgvW only has unicode version
    if(szCommandLineW == NULL)
    {
        return -1;
    }
    int argc = 0;
    LPWSTR* argv = ::CommandLineToArgvW(szCommandLineW, &argc);
    if( argv == NULL )
    {
        ::MessageBox(NULL, szRequireNT, szCaption, MB_OK | MB_ICONERROR);
        return -1;
    }

    BOOL bAnother = FALSE;

    BOOL bUnload =    FALSE;
    BOOL bRestart =    FALSE;

    if( argc > 1 )
    {
        bUnload = lstrcmpW( argv[1], L"/u" ) == 0 || lstrcmpW( argv[1], L"/U" ) == 0;
        bRestart = lstrcmpW( argv[1], L"/r" ) == 0 || lstrcmpW( argv[1], L"/R" ) == 0;

        if(argc > 2 || (!bUnload && !bRestart))
        {
            MessageBox(NULL, szCommandLineHelp, szCaption, MB_OK | MB_ICONINFORMATION);
            GlobalFree(argv );
            return 1;
        }

        if(bUnload)
        {
            bAnother = IsAnotherTabSiHost_Capture(TRUE);
            if( bAnother )
            {
                ::ResetEvent(hAnotherTabSiHostEvent);
            }
            ReleaseAnotherTabSiHost();
            ::GlobalFree(argv);
            return 0;
        }
        else if(bRestart)
        {
            bAnother = IsAnotherTabSiHost_Capture(TRUE);
            if(bAnother)
            {
                ::ResetEvent(hAnotherTabSiHostEvent);
                DWORD dwRet = MyWaitForSingleObject(hAnotherTabSiHostEvent, 3000);
                if( dwRet != WAIT_OBJECT_0 )
                {
                    ASSERT(FALSE && _T("WaitForSingleObject error!"));
                    ::GlobalFree(argv);
                    return -1;
                }
            }
            bAnother = FALSE;
        }
        else
        {
            ASSERT(FALSE);
            // return -1; // warning C4702: unreachable code
        }
    }
    else
    {
        bAnother = IsAnotherTabSiHost_Capture(TRUE);
    }

    ::GlobalFree(argv);
    argv = NULL;

    if(bAnother != FALSE)
    {
        return 0;
    }

    if(hAnotherTabSiHostEvent == NULL)
    {
        ASSERT(FALSE);
        return -1;
    }

    dwWindowsNTMajorVersion = IsWindowsNT();

    if(dwWindowsNTMajorVersion < 4)
    {
        ::MessageBox(NULL, szRequireNT, szCaption, MB_OK | MB_ICONERROR);
        return -1;
    }

    if(GetAppPathName(lpszDllFileName,szDllPath,MAX_PATH) == NULL)
    {
        ::MessageBox(NULL, szGetModuleFilenameError, szCaption, MB_OK | MB_ICONERROR);
        return -2;
    }

    //////////////////////////////////////////////////////////////////////////
    //Let's work
    DWORD dwShowSplash = 1;
    GetDwordRegPorpValue(lpszKeyRoot,lpszShowSplashPropName,dwShowSplash,1);

    if(dwShowSplash != 0)
    {
        TCHAR szBmpFile[MAX_PATH];
        GetAppPathName(splashBmpFileName,szBmpFile,MAX_PATH);
        HBITMAP hBkBmp = (HBITMAP)::LoadImage(NULL,szBmpFile,IMAGE_BITMAP,0,0,
                                              LR_LOADFROMFILE|LR_CREATEDIBSECTION);
        if(hBkBmp != NULL)
        {
            CreateSplashScreen(hBkBmp, TRUE, 3000);
        }
    }

    ::SetTimer(NULL, 0, 2000, ThreadTimerProc);

    MSG msg;
    while((res = ::GetMessage(&msg, NULL, 0, 0)) != 0)
    { 
        if (res == -1)
        {
            // handle the error and possibly exit
            break;
        }
        else
        {
            ::TranslateMessage(&msg); 
            ::DispatchMessage(&msg); 
        }
    } 

    ::SetEvent(hAnotherTabSiHostEvent);
    ReleaseAnotherTabSiHost();

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////
///  CRT:

/*
#ifdef SUBSYSTEM_WINDOWS

int CALLBACK _tWinMain( HINSTANCE, HINSTANCE, LPTSTR, int )
{
    int res = MainFunction();
    return res;
}

#else

int _tmain( int, TCHAR ** ) //( int argc, TCHAR **argv )
{
    int res = MainFunction();
    return res;
}

#endif
//*/

//////////////////////////////////////////////////////////////////////////
