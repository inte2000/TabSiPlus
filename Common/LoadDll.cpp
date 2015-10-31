// Modified by Zoltan Csizmadia, zoltan_csizmadia@yahoo.com
// For companies(Austin,TX): If you would like to get my resume, send an email.
//
//******************************************************************************
//
// Original version made by Felix Kasza, http://www.mvps.org/win32/
//
// demonstrates getting the command line of another process
// requires PROCESS_CREATE_THREAD, PROCESS_VM_OPERATION,
// PROCESS_VM_WRITE, PROCESS_VM_READ to the target, or the
// SeDebugPrivilege

// *** You *must* remove "/GZ" from the debug build settings ***
// (If you use my (felixk's) project file, this has already happened)

#include "stdafx.h"
#include "LoadDll.h"
#include "TabSiPlusDllExport.h" // for TABSIPLUSDLL_DEBUG_API macro

const DWORD MAXINJECTSIZE = 4096;

DWORD IsWindowsNT()
{
   OSVERSIONINFOEX osvi;
   BOOL bOsVersionInfoEx;
   
   // Try calling GetVersionEx using the OSVERSIONINFOEX structure,
   // which is supported on Windows 2000.
   //
   // If that fails, try using the OSVERSIONINFO structure.

   ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

   bOsVersionInfoEx = GetVersionEx ((OSVERSIONINFO *) &osvi);

   if( bOsVersionInfoEx == 0 )
   {
      // If OSVERSIONINFOEX doesn't work, try OSVERSIONINFO.

      osvi.dwOSVersionInfoSize = sizeof (OSVERSIONINFO);
      if (! GetVersionEx ( (OSVERSIONINFO *) &osvi) ) 
         return FALSE;
   }

   return ( osvi.dwPlatformId == VER_PLATFORM_WIN32_NT ? osvi.dwMajorVersion : 0 );
}

PVOID GetFuncAddress(PVOID addr)
{
#ifdef _DEBUG
    //check if instruction is relative jump (E9)
    if (0xE9 != *((UCHAR*)addr))
        return addr;

    // calculate base of relative jump
    ULONG base = (ULONG)((UCHAR*)addr + 5);

    // calculate offset 
    ULONG *offset = (ULONG*)((UCHAR*)addr + 1);

    return (PVOID)(base + *offset);
#else
    // in release, don't have to mess with jumps
    return addr;
#endif
}


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
        )
{
    DWORD rc = (DWORD)-2;

    //Enable debug privilege
    EnableDebugPriv();

    // open the process
    HANDLE hProcess = ::OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, processID );

    if ( hProcess != NULL )
    {
        // let's work
        rc = ::ExecuteRemoteThread( hProcess, bLoad, bFree, lpModuleName, lpFunctionName,
                                    pReturnCodeForFunction, pLastError,
                                    pErrorLoad, pErrorFunction, pErrorFree,
                                    dwArgumentCount, pdwArguments );

        CloseHandle( hProcess );
    }

    return rc;
}

void EnableDebugPriv(void)
{
    HANDLE hToken;
    LUID sedebugnameValue;
    TOKEN_PRIVILEGES tkp;

    // enable the SeDebugPrivilege
    if(!::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
        return;

    if(!::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &sedebugnameValue))
    {
        ::CloseHandle(hToken);
        return;
    }

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sedebugnameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    ::AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL);
    ::CloseHandle(hToken);
}

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
        )
{
    HANDLE ht = 0;
    void *p = 0;
    RemoteDllThreadBlock *c = 0;
    DWORD rc = (DWORD)-1;
    HMODULE hKernel32 = 0;
    RemoteDllThreadBlock localCopy;
    DWORD i;
    DWORD ThreadId = 0;
    DWORD dwReadBytes = 0;
    DWORD WaitObjectRes = 0;

    // clear the parameter block
    ::ZeroMemory( &localCopy, sizeof(localCopy) );
    memset( &localCopy, 0xAA, sizeof(localCopy) ); // debug purposes

    // allocate memory for injected code
    p = VirtualAllocEx( hProcess, 0, MAXINJECTSIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
    if ( p == 0 )
        goto cleanup;

    // allocate memory for parameter block
    c = (RemoteDllThreadBlock*) VirtualAllocEx( hProcess, 0, sizeof(RemoteDllThreadBlock), MEM_COMMIT, PAGE_READWRITE );
    if ( c == 0 )
        goto cleanup;

    // copy function there, we will execute this piece of code
    if ( ! WriteProcessMemory( hProcess, p, GetFuncAddress(RemoteDllThread), MAXINJECTSIZE, 0 ) )
        goto cleanup;
    
    // copy dll path to parameter block
#ifdef _UNICODE
    lstrcpyW( localCopy.lpModulePath, lpDllPath );
#else
    wsprintfW( localCopy.lpModulePath, L"%hs", lpDllPath );
#endif

    if ( lpFunctionName == NULL )
        localCopy.lpFunctionName[0] = '\0';
    else
        lstrcpynA( localCopy.lpFunctionName, lpFunctionName, SIZEOF_ARRAY(localCopy.lpFunctionName) );
    
    localCopy.bLoadLibrary = bLoad;
    localCopy.bFreeLibrary = bFree;
    localCopy.MarkerBegin = MARKER_BEGIN;
    localCopy.MarkerEnd = MARKER_END;

    // kernel32.dll
    hKernel32 = GetModuleHandle( _T("kernel32.dll") );

    if ( hKernel32 == NULL )
        goto cleanup;
    
    // get the addresses for the functions, what we will use in the remote thread

    localCopy.fnLoadLibraryW = (PLoadLibraryW)GetProcAddress( hKernel32, "LoadLibraryW" );
    localCopy.fnGetModuleHandleW = (PGetModuleHandleW)GetProcAddress( hKernel32, "GetModuleHandleW" );
    localCopy.fnFreeLibrary = (PFreeLibrary)GetProcAddress( hKernel32, "FreeLibrary" );
    localCopy.fnGetProcAddress = (PGetProcAddress)GetProcAddress( hKernel32, "GetProcAddress" );
    localCopy.fnGetLastError = (PGetLastError)GetProcAddress( hKernel32, "GetLastError" );
    localCopy.fnSetLastError = (PSetLastError)GetProcAddress( hKernel32, "SetLastError" );

    if (localCopy.fnLoadLibraryW == NULL ||
        localCopy.fnGetModuleHandleW == NULL ||
        localCopy.fnFreeLibrary == NULL ||
        localCopy.fnGetProcAddress == NULL)
        goto cleanup;

    if( dwArgumentCount > REMOTE_MAX_ARGUMENTS )
    {
        goto cleanup;
    }

    if( pdwArguments == NULL && dwArgumentCount != 0 )
    {
        goto cleanup;
    }

    localCopy.dwArgumentCount = dwArgumentCount;
    for( i=0; i<dwArgumentCount; i++ )
    {
        localCopy.Arguments[i] = pdwArguments[i];
    }

    // copy the parameterblock to the other process adress space
    if ( ! WriteProcessMemory( hProcess, c, &localCopy, sizeof localCopy, 0 ) )
        goto cleanup;

    // CreateRemoteThread()
    ht = CreateRemoteThread( hProcess, 0, 0, (DWORD (__stdcall *)( void *)) p, c, 0, &ThreadId );
    if ( ht == NULL )
        goto cleanup;

    WaitObjectRes = WaitForSingleObject( ht, INFINITE );
    switch ( WaitObjectRes )
    {
    case WAIT_TIMEOUT:
        goto cleanup;

    case WAIT_FAILED:
        goto cleanup;

    case WAIT_OBJECT_0:
        // this might just have worked, pick up the result!
        // rad back the prameter block, it has the error code

        // clear the parameter block
        ::ZeroMemory( &localCopy, sizeof(localCopy) );

        if ( ! ReadProcessMemory( hProcess, c, &localCopy, sizeof(localCopy), &dwReadBytes ) )
        {
            rc = 0x100;
            goto cleanup;
        }

        if( dwReadBytes != sizeof(localCopy) )
        {
            rc = 0x200;
            goto cleanup;
        }

        if( localCopy.MarkerBegin != MARKER_BEGIN )
        {
            rc = 0x300;
            goto cleanup;
        }

        if( localCopy.MarkerEnd != MARKER_END )
        {
            rc = 0x400;
            goto cleanup;
        }

      rc = 0;

      if ( pReturnCodeForFunction != NULL )
         *pReturnCodeForFunction = localCopy.ReturnCodeForFunction;

      if( pLastError != NULL )
         *pLastError = localCopy.LastError;

      if ( pErrorLoad != NULL )
         *pErrorLoad = localCopy.ErrorLoad;

      if ( pErrorFunction != NULL )
         *pErrorFunction = localCopy.ErrorFunction;

      if ( pErrorFree != NULL )
         *pErrorFree = localCopy.ErrorFree;

      if ( localCopy.ErrorLoad )
         rc |= 1;

      if ( localCopy.ErrorFunction == 1 )
         rc |= 2;

      if ( localCopy.ErrorFree )
         rc |= 4;

      if ( localCopy.ErrorFunction == 2 )
         rc |= 8;

        break;
    
    default:
        goto cleanup;
    }

cleanup:
    if( ht != NULL )
    {
        CloseHandle( ht );
    }

    // Let's clean
    if ( p != 0 )
        VirtualFreeEx( hProcess, p, 0, MEM_RELEASE );
    if ( c != 0 )
        VirtualFreeEx( hProcess, c, 0, MEM_RELEASE );
    
    return rc;
}
/*
DWORD RemoteGetCurrentDirectory( DWORD pID, LPWSTR lpPath, DWORD size, DWORD* pRetCode )
{
    HANDLE ht = 0;
    void *p = 0;
    RemoteGetCurrentDirectoryThreadBlock *c = 0;
    DWORD rc = (DWORD)-1;
    RemoteGetCurrentDirectoryThreadBlock localCopy;

    HANDLE hProcess = OpenProcess( PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION |
            PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pID );

    if ( hProcess == NULL )
       return 0;

    // clear the parameter block
    ::ZeroMemory( &localCopy, sizeof(localCopy) );

    // allocate memory for injected code
    p = VirtualAllocEx( hProcess, 0, MAXINJECTSIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE );
    if ( p == 0 )
        goto cleanup;

    // allocate memory for parameter block
    c = (RemoteGetCurrentDirectoryThreadBlock*) VirtualAllocEx( 
             hProcess, 0, sizeof(RemoteGetCurrentDirectoryThreadBlock), MEM_COMMIT, PAGE_READWRITE );

    if ( c == 0 )
        goto cleanup;

    // copy function there, we will execute this piece of code
    if ( ! WriteProcessMemory( hProcess, p, GetFuncAddress(RemoteGetCurrentDirectoryThread), MAXINJECTSIZE, 0 ) )
        goto cleanup;
    
    localCopy.fnGetCurrentDirectoryW = (PGetCurrentDirectoryW)GetProcAddress( 
          GetModuleHandle( _T("kernel32.dll") ), "GetCurrentDirectoryW" );

    if (localCopy.fnGetCurrentDirectoryW == NULL)
        goto cleanup;
        
    // copy the parameterblock to the other process adress space
    if ( ! WriteProcessMemory( hProcess, c, &localCopy, sizeof localCopy, 0 ) )
        goto cleanup;

   // CreateRemoteThread()
    ht = CreateRemoteThread( hProcess, 0, 0, (DWORD (__stdcall *)( void *)) p, c, 0, &rc );
    if ( ht == NULL )
        goto cleanup;
    
    rc = WaitForSingleObject( ht, INFINITE );
    switch ( rc )
    {
    case WAIT_TIMEOUT:
        goto cleanup;

    case WAIT_FAILED:
        goto cleanup;

    case WAIT_OBJECT_0:
        // this might just have worked, pick up the result!
        // rad back the prameter block, it has the error code
        if ( ! ReadProcessMemory( hProcess, c, &localCopy, sizeof localCopy, 0 ) )
            goto cleanup;

      if ( pRetCode != NULL )
         *pRetCode = localCopy.dwReturnCode;

      if ( localCopy.dwReturnCode != 0 )
      {
         lstrcpynW( lpPath, localCopy.lpDirectory, size );
      }

      break;
    
    default:
        break;
    }

cleanup:
     // Let's clean
   if ( ht != NULL )
       CloseHandle( ht );
    if ( p != 0 )
        VirtualFreeEx( hProcess, p, 0, MEM_RELEASE );
    if ( c != 0 )
        VirtualFreeEx( hProcess, c, 0, MEM_RELEASE );
    if ( hProcess != NULL)
        CloseHandle( hProcess );

    return rc;
}
*/

BOOL RemoteSimpleFunction( DWORD processId, DWORD dwArgument, char* lpszFunction, DWORD* lpdwFuncRetVal )
{
    HANDLE hRemoteThread = NULL;
    BOOL rc = FALSE;
    HMODULE hKernel32 = NULL;
    FARPROC pfnProc = NULL;
    DWORD ThreadId = 0;
    DWORD dwWaitRes;

    //Enable debug privilege
    EnableDebugPriv();

    // open the process
    HANDLE hProcess = OpenProcess(
        PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ,
        FALSE, processId );

    if ( hProcess == NULL )
        goto cleanup;

    hKernel32 = GetModuleHandle(_T("kernel32.dll"));

    if ( hKernel32 == NULL )
        goto cleanup;

    pfnProc = GetProcAddress( hKernel32, lpszFunction );

    if ( pfnProc == NULL )
        goto cleanup;

    hRemoteThread = CreateRemoteThread( hProcess, 0, 0, 
        (LPTHREAD_START_ROUTINE)pfnProc, (LPVOID)dwArgument, 0, (DWORD*)&ThreadId );

    if( lpdwFuncRetVal != NULL )
    {
        dwWaitRes = WaitForSingleObject( hRemoteThread, INFINITE );
        switch ( rc )
        {
        case WAIT_OBJECT_0:
            GetExitCodeThread( hRemoteThread, lpdwFuncRetVal );
            break;

        case WAIT_FAILED:
        case WAIT_TIMEOUT:
        default:
            goto cleanup;
        }
    }

    rc = TRUE;

cleanup:

    if( hProcess != NULL )
    {
        CloseHandle( hProcess );
    }

    if( hRemoteThread != NULL )
    {
        CloseHandle( hRemoteThread );
    }

    return rc;
}

// The whole shebang makes a number of assumptions:
// -- target process is a Win32 process
// -- kernel32.dll loaded at same address in each process (safe)
// -- bem() shorter than MAXINJECTSIZE
// -- bem() does not rely on the C/C++ runtime
// -- /GZ is _not_ used. (If it is, the compiler generates calls
//    to functions which are not injected into the target. Oops!
// -- Target function uses WINAPI (pascal) call convention.
DWORD __stdcall RemoteDllThread( RemoteDllThreadBlock* execBlock )
{
    // and this is the code we are injecting

    typedef DWORD (WINAPI *PFN)();
    typedef DWORD (WINAPI *PFN1)(DWORD);
    typedef DWORD (WINAPI *PFN2)(DWORD,DWORD);
    typedef DWORD (WINAPI *PFN3)(DWORD,DWORD,DWORD);
    typedef DWORD (WINAPI *PFN4)(DWORD,DWORD,DWORD,DWORD);
    typedef DWORD (WINAPI *PFN5)(DWORD,DWORD,DWORD,DWORD,DWORD);
    typedef DWORD (WINAPI *PFN6)(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD);
    typedef DWORD (WINAPI *PFN7)(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD);
    typedef DWORD (WINAPI *PFN8)(DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD,DWORD);

    HMODULE hModule = NULL;
//__asm int 3
    // clear the error codes
    execBlock->ErrorLoad = 0;
    execBlock->ErrorFunction = 0;
    execBlock->ErrorFree = 0;

    // load the requested dll
    if ( execBlock->bLoadLibrary )
    {
        execBlock->hModule = (HMODULE)(*execBlock->fnLoadLibraryW)( execBlock->lpModulePath );

        hModule = execBlock->hModule;

        execBlock->ErrorLoad = execBlock->hModule != NULL ? 0 : 1;
    }

    // if we didn't load the library, try to query the module handle
    if ( hModule == NULL )
        hModule = (*execBlock->fnGetModuleHandleW)( execBlock->lpModulePath );

    // call function
    if ( execBlock->lpFunctionName[0] != 0 )
    {
        //execute a function if we have a function name
        PFN pfn = (PFN)
            (*execBlock->fnGetProcAddress)( hModule, execBlock->lpFunctionName );

        // execute the function, and get the result
        if ( pfn != NULL )
        {
            DWORD ret = 0;
            DWORD* p = execBlock->Arguments;
            execBlock->ErrorFunction = 0;

            execBlock->LastError = 0;
            execBlock->fnSetLastError( execBlock->LastError );

            // !!!! Switch is a bad idea: compiler does something with it and
            // function don't copy correctly into the remote process
/*
#define S_BEGIN \
            switch( execBlock->dwArgumentCount ) \
            { \

#define S_CASE(x)    case x:
#define S_ENDCASE    break;
// this condition must be checked and prevented in ExecuteRemoteThread()!
#define S_END \
                default: \
                    execBlock->ErrorFunction = 1; \
            }
*/

#define S_BEGIN        DWORD cnt = execBlock->dwArgumentCount; if( cnt != execBlock->dwArgumentCount ) { }
#define S_CASE(x)    else if( cnt == (x) ) {
#define S_ENDCASE    }
#define S_END        else { execBlock->ErrorFunction = 2; }

            S_BEGIN
                S_CASE(0)        ret = (*(PFN) pfn)();  S_ENDCASE
                S_CASE(1)        ret = (*(PFN1)pfn)(p[0]);  S_ENDCASE
                S_CASE(2)        ret = (*(PFN2)pfn)(p[0],p[1]);  S_ENDCASE
                S_CASE(3)        ret = (*(PFN3)pfn)(p[0],p[1],p[2]);  S_ENDCASE
                S_CASE(4)        ret = (*(PFN4)pfn)(p[0],p[1],p[2],p[3]);  S_ENDCASE
                S_CASE(5)        ret = (*(PFN5)pfn)(p[0],p[1],p[2],p[3],p[4]);  S_ENDCASE
                S_CASE(6)        ret = (*(PFN6)pfn)(p[0],p[1],p[2],p[3],p[4],p[5]);  S_ENDCASE
                S_CASE(7)        ret = (*(PFN7)pfn)(p[0],p[1],p[2],p[3],p[4],p[5],p[6]);  S_ENDCASE
                S_CASE(8)        ret = (*(PFN8)pfn)(p[0],p[1],p[2],p[3],p[4],p[5],p[6],p[7]);  S_ENDCASE
            S_END

            execBlock->LastError = execBlock->fnGetLastError();
            execBlock->ReturnCodeForFunction = ret;
        }
        else
            execBlock->ErrorFunction = 1;
    }

    // free library
    if ( execBlock->bFreeLibrary )
    {
        execBlock->ErrorFree = execBlock->fnFreeLibrary( hModule ) ? 0 : 1;
    }

    execBlock->hModule = hModule;
    
    return 0;
}

/*
// and this is the code we are injecting
DWORD __stdcall RemoteGetCurrentDirectoryThread( RemoteGetCurrentDirectoryThreadBlock* pParam )
{
    pParam->dwReturnCode = (*pParam->fnGetCurrentDirectoryW )( pParam->lpDirectory, SIZEOF_ARRAY(pParam->lpDirectory) );

    return 0;
}
*/
