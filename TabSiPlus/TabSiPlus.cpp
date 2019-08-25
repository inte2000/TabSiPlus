// TabSiPlus.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "TabSiPlus.h"
#include "LoadDll.h"
#include "SourceInsight.h"
#include "TabWndUIThread.h"

#include "SIFrameWnd.h"
#include "SiMDIWnd.h"

#include "GlobalFileList.h"
#include "ApiHook.h"

#include "CmdIconMap.h"
#include "CmdIconResource.h"

#include "..\TSupp\TSupp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//    Note!
//
//        If this DLL is dynamically linked against the MFC
//        DLLs, any functions exported from this DLL which
//        call into MFC must have the AFX_MANAGE_STATE macro
//        added at the very beginning of the function.
//
//        For example:
//
//        extern "C" BOOL PASCAL EXPORT ExportedFunction()
//        {
//            AFX_MANAGE_STATE(AfxGetStaticModuleState());
//            // normal function body here
//        }
//
//        It is very important that this macro appear in each
//        function, prior to any calls into MFC.  This means that
//        it must appear as the first statement within the 
//        function, even before any object variable declarations
//        as their constructors may generate calls into the MFC
//        DLL.
//
//        Please see MFC Technical Notes 33 and 58 for additional
//        details.
//

//#pragma data_seg(".SharedData")
volatile BOOL bInitialized = FALSE;
//#pragma data_seg()
//#pragma comment( linker, "/SECTION:.SharedData,RWS" )

#pragma comment( linker, "/BASE:0x5BD00000" )

#pragma comment(linker, "/SECTION:.text,ERW")

//#pragma comment( linker, "/OUT:TaskManagerEx.dll" )

//static char g_szStamp_Mark[] = "\r\n\r\nTabSiPlus.cpp Timestamp: " __DATE__ ", " __TIME__ "\r\n\r\n";

BOOL bExitPadding = FALSE;

static CApiHookMgr *g_pHookMgr = NULL;
static GlobalFileList g_OpenFiles;

GlobalFileList  *g_pOpenFileList = &g_OpenFiles;

HANDLE WINAPI MyCreateFileA(LPCSTR lpFileName,DWORD dwDesiredAccess,DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes,DWORD dwCreationDisposition,DWORD dwFlagsAndAttributes,HANDLE hTemplateFile) 
{ 
    TCHAR szSrcFile[MAX_PATH];
#ifdef UNICODE
    ::MultiByteToWideChar(CP_ACP, 0, lpFileName, -1, szSrcFile, MAX_PATH) ;
#else
    _tcscpy(szSrcFile,lpFileName);
#endif

    //调用原始的API
    HANDLE hReturn = CreateFileA(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
    if(hReturn != INVALID_HANDLE_VALUE)
    {
        if(IsSourceCodeFile(szSrcFile) && cfg_bSmartLoc)
            g_OpenFiles.AddToList(szSrcFile);
    }

//    DebugTracing(gnDbgLevelNormalDebug,_T("MyCreateFileA(%s,%x)"),szSrcFile,(int)hReturn);

    return hReturn;
} 

HANDLE WINAPI MyCreateFileW(LPCWSTR lpFileName,DWORD dwDesiredAccess,DWORD dwShareMode,LPSECURITY_ATTRIBUTES lpSecurityAttributes,DWORD dwCreationDisposition,DWORD dwFlagsAndAttributes,HANDLE hTemplateFile) 
{ 
    TCHAR szSrcFile[MAX_PATH];
#ifdef UNICODE
    _tcscpy(szSrcFile,lpFileName);
#else
    ::WideCharToMultiByte(CP_ACP,0,lpFileName,-1,szSrcFile,MAX_PATH,NULL,NULL);
#endif
    //调用原始的API
    HANDLE hReturn = CreateFileW(lpFileName,dwDesiredAccess,dwShareMode,lpSecurityAttributes,dwCreationDisposition,dwFlagsAndAttributes,hTemplateFile);
    if(hReturn != INVALID_HANDLE_VALUE)
    {
        if(IsSourceCodeFile(szSrcFile) && cfg_bSmartLoc)
            g_OpenFiles.AddToList(szSrcFile);
    }

//    DebugTracing(gnDbgLevelNormalDebug,_T("MyCreateFileW(%s,%x)"),szSrcFile,(int)hReturn);
    return hReturn;
}

/////////////////////////////////////////////////////////////////////////////
// CTabSiPlusApp

BEGIN_MESSAGE_MAP(CTabSiPlusApp, CWinApp)
    //{{AFX_MSG_MAP(CTabSiPlusApp)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CTabSiPlusApp theApp;


TABSIPLUSDLL_API BOOL WINAPI Initialize()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());

    DebugTracing(gnDbgLevelNormalDebug, _T("TabSiPlus.dll> Initialize() enter") );
    BOOL res = theApp.Initialize();
    DebugTracing(gnDbgLevelNormalDebug, _T("TabSiPlus.dll> Initialize() returned %d"), res );

    return res;
}

/////////////////////////////////////////////////////////////////////////////
// CTabSiPlusApp construction
CTabSiPlusApp::CTabSiPlusApp()
{
#ifdef _DEBUG
//    InitDebugTracing(gnDbgLevelNormalDebug,gnDbgTraceMacro,TRUE);
//    InitDebugTracing(gnDbgLevelNormalDebug,gnDbgTraceLogFile,TRUE,_T("c:\\sss.txt"));
    InitDebugTracing(gnDbgLevelNormalDebug,gnDbgTraceLogFile,TRUE,_T("c:\\sss_log.txt"));
//    InitDebugTracing(gnDbgLevelNormalDebug,gnDbgTraceNone,FALSE);
#else
    //InitDebugTracing(gnDbgLevelNormalDebug,gnDbgTraceLogFile,TRUE,_T("c:\\sss_log.txt"));
    InitDebugTracing(gnDbgLevelNormalDebug,gnDbgTraceNone,FALSE);
#endif
    SetLimitationMethod(gnLimitDown);
    EnableExtraTimeInfo(FALSE);
    EnableExtraProcessInfo(TRUE);
}

CTabSiPlusApp::~CTabSiPlusApp()
{
    UninitDebugTracing();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTabSiPlusApp object

BOOL CTabSiPlusApp::InitInstance() 
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    
    ::DisableThreadLibraryCalls((HMODULE)AfxGetInstanceHandle());

    DebugTracing(gnDbgLevelNormalDebug,_T("CTabSiPlusApp::InitInstance(bInitialized = %d) Start"),bInitialized);
    if(bInitialized)
        return FALSE;

    DWORD dwWindowsNTMajorVersion = IsWindowsNT();
    if(dwWindowsNTMajorVersion < 4)
        return FALSE;

    SetLocResourceInstHandle(AfxGetInstanceHandle());
//    hMainIconSmall = LocLoadIcon( IDI_SYSTEM_INFO_ICON, 16, 16 );
//    hMainIconBig = LocLoadIcon( IDI_SYSTEM_INFO_ICON, 32, 32 );

    SetRegistryRootKey(HKEY_CURRENT_USER);
    GetBoolRegPorpValue(lpszKeyRoot,lpszSmartLoc,cfg_bSmartLoc,FALSE);
    GetIntegerRegPorpValue(lpszKeyRoot,lpszSrhDeep,cfg_uSrhDeep,2);

    EnableDebugPriv();
    g_pHookMgr = new CApiHookMgr();
    if(g_pHookMgr == NULL)
        return FALSE;

    //g_pHookMgr->HookSystemFuncs();
    g_pHookMgr->HookImport("Kernel32.DLL", "CreateFileW", (PROC)MyCreateFileW);
    g_pHookMgr->HookImport("Kernel32.DLL", "CreateFileA", (PROC)MyCreateFileA);

    DebugTracing(gnDbgLevelNormalDebug,_T("The hook engine has been activated"));

    TCHAR szModuleFile[MAX_PATH];
    g_TSuppDll = LoadLibrary(GetAppFilePathName(lpszTSuppDll,szModuleFile,MAX_PATH));

    if(!PrepareResource(g_TSuppDll))
        return FALSE;

    DebugTracing(gnDbgLevelNormalDebug,_T("CTabSiPlusApp::InitInstance End"));
    return CWinApp::InitInstance();
}

int CTabSiPlusApp::ExitInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

    DebugTracing(gnDbgLevelNormalDebug,_T("CTabSiPlusApp::ExitInstance(bInitialized = %d) Start"),bInitialized);

    if(bInitialized)
    {
        Deinitialize();
    }

    if(g_pHookMgr)
    {
        if(g_pHookMgr->AreThereHookedFunctions())
        {
            g_pHookMgr->UnHookImport("Kernel32.DLL", "CreateFileW");
            g_pHookMgr->UnHookImport("Kernel32.DLL", "CreateFileA");
            //g_pHookMgr->UnHookAllFuncs();
        }

        
        delete g_pHookMgr;
        g_pHookMgr = NULL;
    }

    if(g_TSuppDll)
        FreeLibrary((HMODULE)g_TSuppDll);

    DebugTracing(gnDbgLevelNormalDebug,_T("CTabSiPlusApp::ExitInstance End"));
    return CWinApp::ExitInstance();
}

BOOL CTabSiPlusApp::Initialize()
{
    DebugTracing(gnDbgLevelNormalDebug,_T("CTabSiPlusApp::Initialize() Start"));

    if(IsAnotherTabSiPlusDll())
      return FALSE;

    InitGlobalVar();

    DebugTracing(gnDbgLevelNormalDebug,_T("CTabSiPlusApp::Initialize, %s"),g_szCurDircetory);

    g_pTabWndUIThread = (CTabWndUIThread *)AfxBeginThread(RUNTIME_CLASS(CTabWndUIThread),THREAD_PRIORITY_NORMAL,0,0,NULL);

    bInitialized = TRUE;
//    Run();
    DebugTracing(gnDbgLevelNormalDebug,_T("CTabSiPlusApp::Initialize() end"));
    return TRUE;
}

BOOL CTabSiPlusApp::Deinitialize()
{
    bInitialized = FALSE;
    if(g_pTabWndUIThread)
    {
        DWORD dwExitCode;
        if(::GetExitCodeThread(g_pTabWndUIThread->m_hThread, &dwExitCode))
        {
            if(dwExitCode == STILL_ACTIVE)
            {
                ::WaitForSingleObject(g_pTabWndUIThread->m_hThread,2000);//等待线程结束
            }
        }
        delete g_pTabWndUIThread;
        g_pTabWndUIThread = NULL;
    }

    return TRUE;
}

BOOL CTabSiPlusApp::IsAnotherTabSiPlusDll()
{
    CString strMutexName;
    strMutexName.Format( _T("Local\\TabSiPlusDllIsAlreadyLoaded_PID_%08X"), GetCurrentProcessId());

    HANDLE hMutex = CreateMutex( NULL, FALSE, strMutexName );    
    DWORD dwer = GetLastError();    
    BOOL bExisted = (hMutex==NULL || dwer==ERROR_ALREADY_EXISTS);
    if(bExisted && hMutex)
    {
        ::CloseHandle(hMutex);
    }
    return bExisted;
}

