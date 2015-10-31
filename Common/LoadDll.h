#ifndef LOADDLL_H_INCLUDED
#define LOADDLL_H_INCLUDED

typedef HMODULE (WINAPI *PLoadLibraryW)(LPCWSTR);
typedef HMODULE (WINAPI *PGetModuleHandleW)(LPCWSTR);
typedef BOOL    (WINAPI *PFreeLibrary)(HMODULE);
typedef FARPROC (WINAPI *PGetProcAddress)(HMODULE, char*);
typedef DWORD   (WINAPI *PGetCurrentDirectoryW)(LPWSTR,DWORD);
typedef DWORD   (WINAPI *PGetLastError) ( VOID );
typedef VOID    (WINAPI *PSetLastError) ( DWORD dwErrCode );

#define MARKER_BEGIN    0x19822891
#define MARKER_END        0x31415926
#define REMOTE_MAX_ARGUMENTS    8

struct RemoteDllThreadBlock
{
    DWORD                MarkerBegin;
    DWORD                ErrorLoad;                    // error value for LoadLibrary
    DWORD                ErrorFunction;                // error value for executed function
    DWORD                ReturnCodeForFunction;        // error value for executed function
    DWORD                ErrorFree;                    // error value for FreeLibrary
    DWORD                LastError;

    HMODULE                hModule;
    BOOL                bLoadLibrary;
    BOOL                bFreeLibrary;
    DWORD                dwArgumentCount; // 0..8
    DWORD                Arguments[REMOTE_MAX_ARGUMENTS];

    PLoadLibraryW        fnLoadLibraryW;
    PGetModuleHandleW    fnGetModuleHandleW;
    PFreeLibrary        fnFreeLibrary;
    PGetProcAddress        fnGetProcAddress;
    PGetLastError        fnGetLastError;
    PSetLastError        fnSetLastError;

    WCHAR                lpModulePath[MAX_PATH];    // the DLL path
    CHAR                lpFunctionName[256];        // the called function
    DWORD                MarkerEnd;
};

struct RemoteGetCurrentDirectoryThreadBlock
{
    PGetCurrentDirectoryW    fnGetCurrentDirectoryW;
    WCHAR                    lpDirectory[MAX_PATH];
    DWORD                    dwReturnCode;
};


// inject function RemoteThread() into target process
DWORD ExecuteRemoteThread(
        HANDLE    hProcess,
        BOOL    bLoad,
        BOOL    bFree,
        LPCTSTR    lpDllPath,
        char*    lpFunctionName,
        DWORD*    pReturnCodeForFunction,
        LONG*    pLastError,
        DWORD*    pErrorLoad,
        DWORD*    pErrorFunction,
        DWORD*    pErrorFree,
        DWORD    dwArgumentCount,
        DWORD*    pdwArguments
        );

// and this is the code we are injecting
DWORD __stdcall RemoteDllThread( RemoteDllThreadBlock* );

// and this is the code we are injecting
DWORD __stdcall RemoteGetCurrentDirectoryThread( RemoteGetCurrentDirectoryThreadBlock* );


// That's the THING
// The whole shebang makes a number of assumptions:
// -- target process is a Win32 process
// -- kernel32.dll loaded at same address in each process (safe)
// -- bem() shorter than MAXINJECTSIZE
// -- bem() does not rely on the C/C++ runtime
// -- /GZ is _not_ used. (If it is, the compiler generates calls
//    to functions which are not injected into the target. Oops!
// -- Target function uses WINAPI (pascal) call convention.
DWORD LoadDllForRemoteThread(
        DWORD    processID,
        BOOL    bLoad,
        BOOL    bFree,
        LPCTSTR    lpModuleName,
        char*    lpFunctionName,
        DWORD*    pReturnCodeForFunction,
        LONG*    pLastError,
        DWORD*    pErrorLoad,
        DWORD*    pErrorFunction,
        DWORD*    pErrorFree,
        DWORD    dwArgumentCount,
        DWORD*    pdwArguments
        );

//DWORD RemoteGetCurrentDirectory( DWORD, LPWSTR, DWORD, DWORD* );

// Check OS
DWORD IsWindowsNT();
BOOL RemoteSimpleFunction(DWORD processId, DWORD dwArgument, char* lpszFunction, DWORD* lpdwFuncRetVal);
void EnableDebugPriv(void);

#endif