//---------------------------------------------------------------------------
//
// ApiHook.cpp
//
// SUBSYSTEM: 
//                API Hooking system
// MODULE:    
//                ApiHook is the main module of the hooking system
//
// DESCRIPTION:
//
// AUTHOR:        Ivo Ivanov (ivopi@hotmail.com) 
//              Some part of ReplaceInOneModule() implementation is based 
//              on code published by Jeffrey Richter and John Robbins.
//
// DATE:        2000 May 05,  version 1.0 
//              2001 June 29, version 2.0
//              2002 November 30. version 2.1
//
// FIXES:
//              - 2002 May 08
//                Fixed bug that used to cause an access violation 
//                if a hooked application calls GetProcAddress() API with an 
//                ordinal value rather than string function name. For more 
//                details see the implementation of 
//                CHookedFunction* CHookedFunctions::GetHookedFunction( 
//                    PCSTR pszCalleeModName, PCSTR pszFuncName )
//              - 2002 July 25
//                In the implementation of CApiHookMgr::AddHook() method
//                a newly created object CHookedFunction must be always inserted 
//                in the collection, even the HookImport() method returns FALSE. 
//                This solve the problem with dynamically imported functions that 
//                haven't been found in the IAT during the initial call to AddHook() 
//              - 2002 August 27
//                Fixed bug related to storing incorrect values of the full path 
//                name of the DLL in the map. In fact we need to get only the name 
//                and extension parts of the full file name.
//              - 2002 November 30
//                Added two methods of class CHookedFunctions 
//                (GetFunctionNameFromExportSection() and GetFunctionNameByOrdinal()) 
//                for handling scenarios where the function for spying is imported by 
//                number. This should allow a user of the library to easily intercept 
//                Windows socket APIs that are usually imported by ordinal value.
//                Added critical section in CApiHookMgr::MyGetProcAddress() method 
//                                                                         
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// Includes
//  
//---------------------------------------------------------------------------

#include "stdafx.h"
#include "ApiHook.h"
#include "SysUtils.h"
#include <imagehlp.h>
#pragma comment(lib, "imagehlp.lib")

//---------------------------------------------------------------------------
//
// class CApiHookMgr
//  
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//
// File scope constants and typedefs
//
//---------------------------------------------------------------------------
typedef struct 
{
    char szCalleeModName[MAX_PATH]; 
    char szFuncName[MAX_PATH];    
} API_FUNC_ID;

const API_FUNC_ID MANDATORY_API_FUNCS[] =
{
    {"Kernel32.dll", "LoadLibraryA"},
    {"Kernel32.dll", "LoadLibraryW"},
    {"Kernel32.dll", "LoadLibraryExA"},
    {"Kernel32.dll", "LoadLibraryExW"},
    {"Kernel32.dll", "GetProcAddress"}
};

// This macro evaluates to the number of elements in MANDATORY_API_FUNCS
#define NUMBER_OF_MANDATORY_API_FUNCS (sizeof(MANDATORY_API_FUNCS) / sizeof(MANDATORY_API_FUNCS[0])) 

//---------------------------------------------------------------------------
//
// Static members
//
//---------------------------------------------------------------------------

CHookedFunctions* CApiHookMgr::sm_pHookedFunctions = NULL;
CEventEx CApiHookMgr::sm_CritEvent(FALSE, TRUE);
//---------------------------------------------------------------------------
// CApiHookMgr
// 
// Ctor
//---------------------------------------------------------------------------
CApiHookMgr::CApiHookMgr()
{
    // Obtain the handle to the DLL which code executes
    //
    m_hmodThisInstance   = ModuleFromAddress(CApiHookMgr::MyGetProcAddress);
    //
    // No system functions have been hooked up yet
    //
    m_bSystemFuncsHooked = FALSE;
    //
    // Create an instance of the map container
    //
    sm_pHookedFunctions  = new CHookedFunctions(this); 
}

//---------------------------------------------------------------------------
// ~CApiHookMgr
// 
// Dtor
//---------------------------------------------------------------------------

CApiHookMgr::~CApiHookMgr()
{
    UnHookAllFuncs();
    delete sm_pHookedFunctions;
}

//---------------------------------------------------------------------------
// HookSystemFuncs
// 
// Hook all needed system functions in order to trap loading libraries
//---------------------------------------------------------------------------
BOOL CApiHookMgr::HookSystemFuncs()
{
    BOOL bResult;

    if (TRUE != m_bSystemFuncsHooked)
    {
        bResult = HookImport("Kernel32.dll", "LoadLibraryA", (PROC) CApiHookMgr::MyLoadLibraryA);
        bResult = HookImport("Kernel32.dll", "LoadLibraryW", (PROC) CApiHookMgr::MyLoadLibraryW) || bResult;
        bResult = HookImport("Kernel32.dll", "LoadLibraryExA", (PROC) CApiHookMgr::MyLoadLibraryExA) || bResult;
        bResult = HookImport("Kernel32.dll", "LoadLibraryExW", (PROC) CApiHookMgr::MyLoadLibraryExW) || bResult;
        bResult = HookImport("Kernel32.dll", "GetProcAddress", (PROC) CApiHookMgr::MyGetProcAddress) || bResult;
        m_bSystemFuncsHooked = bResult;
    } // if

    return m_bSystemFuncsHooked;
}

//---------------------------------------------------------------------------
// UnHookAllFuncs
// 
// Unhook all functions and restore original ones
//---------------------------------------------------------------------------
void CApiHookMgr::UnHookAllFuncs()
{
    if (TRUE == m_bSystemFuncsHooked)
    {
        CHookedFunction* pHook;
        CHookedFunctions::const_iterator itr;
        for (itr = sm_pHookedFunctions->begin(); itr != sm_pHookedFunctions->end();    ++itr)
        {
            pHook = itr->second;
            pHook->UnHookImport();
            delete pHook;
        } // for
        sm_pHookedFunctions->clear();
        m_bSystemFuncsHooked = FALSE;
    } // if
}

//
// Indicates whether there is hooked function
//
BOOL CApiHookMgr::AreThereHookedFunctions()
{
    return (sm_pHookedFunctions->size() > 0);
}


//---------------------------------------------------------------------------
// HookImport
//
// Hook up an API function
//---------------------------------------------------------------------------
BOOL CApiHookMgr::HookImport(PCSTR pszCalleeModName, PCSTR pszFuncName, PROC  pfnHook)
{
    CEventLock lock(&sm_CritEvent);

    BOOL                  bResult = FALSE;
    PROC                  pfnOrig = NULL;
    try
    {
        if (!sm_pHookedFunctions->GetHookedFunction(pszCalleeModName, pszFuncName))
        {
            pfnOrig = GetProcAddressWindows(::GetModuleHandleA(pszCalleeModName),    pszFuncName);
            //
            // It's possible that the requested module is not loaded yet
            // so lets try to load it.
            //
            if (NULL == pfnOrig)
            {
                HMODULE hmod = ::LoadLibraryA(pszCalleeModName);
                if (NULL != hmod)
                    pfnOrig = GetProcAddressWindows(::GetModuleHandleA(pszCalleeModName),    pszFuncName);
            } // if
            if (NULL != pfnOrig)
                bResult = AddHook(pszCalleeModName,    pszFuncName, pfnOrig,    pfnHook);
        } // if
    }
    catch(...)
    {
    } // try..catch

    return bResult;
}

//---------------------------------------------------------------------------
// UnHookImport
//
// Restores original API function address in IAT
//---------------------------------------------------------------------------
BOOL CApiHookMgr::UnHookImport(PCSTR pszCalleeModName, PCSTR pszFuncName)
{
    CEventLock lock(&sm_CritEvent);

    BOOL bResult = TRUE;
    try
    {
        bResult = RemoveHook(pszCalleeModName, pszFuncName);
    }
    catch (...)
    {
    }
    return bResult;
}

//---------------------------------------------------------------------------
// AddHook
//
// Add a hook to the internally supported container
//---------------------------------------------------------------------------
BOOL CApiHookMgr::AddHook(PCSTR pszCalleeModName, PCSTR pszFuncName, PROC  pfnOrig, PROC  pfnHook)
{
    BOOL             bResult = FALSE;
    CHookedFunction* pHook   = NULL;

    if (!sm_pHookedFunctions->GetHookedFunction(pszCalleeModName, pszFuncName))
    {
        pHook = new CHookedFunction(sm_pHookedFunctions,pszCalleeModName,pszFuncName,pfnOrig,pfnHook);
        // We must create the hook and insert it in the container
        pHook->HookImport();
        bResult = sm_pHookedFunctions->AddHook(pHook);
    } // if

    return bResult;
}

//---------------------------------------------------------------------------
// RemoveHook
//
// Remove a hook from the internally supported container
//---------------------------------------------------------------------------
BOOL CApiHookMgr::RemoveHook(PCSTR pszCalleeModName, PCSTR pszFuncName)
{
    BOOL             bResult = FALSE;
    CHookedFunction *pHook   = NULL;

    pHook = sm_pHookedFunctions->GetHookedFunction(pszCalleeModName,pszFuncName);
    if ( NULL != pHook )
    {
        bResult = pHook->UnHookImport();
        if ( bResult )
        {
            bResult = sm_pHookedFunctions->RemoveHook( pHook );
            if ( bResult )
                delete pHook;
        } // if
    } // if

    return bResult;
}


//---------------------------------------------------------------------------
// HackModuleOnLoad
//
// Used when a DLL is newly loaded after hooking a function
//---------------------------------------------------------------------------
void WINAPI CApiHookMgr::HackModuleOnLoad(HMODULE hmod, DWORD dwFlags)
{
    //
    // If a new module is loaded, just hook it
    //
    if ((hmod != NULL) && ((dwFlags & LOAD_LIBRARY_AS_DATAFILE) == 0)) 
    {
        CEventLock lock(&sm_CritEvent);
        
        CHookedFunction* pHook;
        CHookedFunctions::const_iterator itr;
        for (itr = sm_pHookedFunctions->begin(); itr != sm_pHookedFunctions->end();    ++itr)
        {
            pHook = itr->second;
            pHook->ReplaceInOneModule(pHook->Get_CalleeModName(),    pHook->Get_pfnOrig(),    pHook->Get_pfnHook(),    hmod);
        } // for
    } // if
}

//---------------------------------------------------------------------------
//
// System API hooks prototypes
//
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
// MyLoadLibraryA
//
// 
//---------------------------------------------------------------------------
HMODULE WINAPI CApiHookMgr::MyLoadLibraryA(PCSTR pszModuleName)
{
    HMODULE hmod = ::LoadLibraryA(pszModuleName);
    HackModuleOnLoad(hmod, 0);

    return hmod;
}

//---------------------------------------------------------------------------
// MyLoadLibraryW
//
// 
//---------------------------------------------------------------------------
HMODULE WINAPI CApiHookMgr::MyLoadLibraryW(PCWSTR pszModuleName)
{
    HMODULE hmod = ::LoadLibraryW(pszModuleName);
    HackModuleOnLoad(hmod, 0);

    return hmod;
}

//---------------------------------------------------------------------------
// MyLoadLibraryExA
//
// 
//---------------------------------------------------------------------------
HMODULE WINAPI CApiHookMgr::MyLoadLibraryExA(
    PCSTR  pszModuleName, 
    HANDLE hFile, 
    DWORD dwFlags)
{
    HMODULE hmod = ::LoadLibraryExA(pszModuleName, hFile, dwFlags);
    HackModuleOnLoad(hmod, 0);

    return hmod;
}

//---------------------------------------------------------------------------
// MyLoadLibraryExW
//
// 
//---------------------------------------------------------------------------
HMODULE WINAPI CApiHookMgr::MyLoadLibraryExW(
    PCWSTR pszModuleName, 
    HANDLE hFile, 
    DWORD dwFlags)
{
    HMODULE hmod = ::LoadLibraryExW(pszModuleName, hFile, dwFlags);
    HackModuleOnLoad(hmod, 0);

    return hmod;
}

//---------------------------------------------------------------------------
// MyGetProcAddress
//
// 
//---------------------------------------------------------------------------
FARPROC WINAPI CApiHookMgr::MyGetProcAddress(HMODULE hmod, PCSTR pszProcName)
{
    // It is possible that multiple threads will call hooked GetProcAddress() 
    // API, therefore we should make it thread safe because it accesses sm_pHookedFunctions 
    // shared container.
    CEventLock lock(&sm_CritEvent);
    //
    // Get the original address of the function
    //
    FARPROC pfn = GetProcAddressWindows(hmod, pszProcName);
    //
    // Attempt to locate if the function has been hijacked
    //
    CHookedFunction* pFuncHook = sm_pHookedFunctions->GetHookedFunction(hmod, pszProcName);
    if (NULL != pFuncHook)
        //
        // The address to return matches an address we want to hook
        // Return the hook function address instead
        //
        pfn = pFuncHook->Get_pfnHook();

    return pfn;
}

//---------------------------------------------------------------------------
// GetProcAddressWindows
//
// Returns original address of the API function
//---------------------------------------------------------------------------
FARPROC WINAPI CApiHookMgr::GetProcAddressWindows(HMODULE hmod, PCSTR pszProcName)
{
    return ::GetProcAddress(hmod, pszProcName);
}

//---------------------------------------------------------------------------
//
// class CHookedFunction
//  
//---------------------------------------------------------------------------

//
// The highest private memory address (used for Windows 9x only)
//
PVOID CHookedFunction::sm_pvMaxAppAddr = NULL;
//
// The PUSH opcode on x86 platforms
//
const BYTE cPushOpCode = 0x68;   

//---------------------------------------------------------------------------
// CHookedFunction
//  
//
//---------------------------------------------------------------------------
CHookedFunction::CHookedFunction(CHookedFunctions* pHookedFunctions,PCSTR pszCalleeModName, 
    PCSTR pszFuncName, PROC pfnOrig, PROC pfnHook) :    m_pHookedFunctions(pHookedFunctions),    m_bHooked(FALSE),
        m_pfnOrig(pfnOrig),    m_pfnHook(pfnHook)
{
    strcpy(m_szCalleeModName, pszCalleeModName); 
    strcpy(m_szFuncName, pszFuncName);    

    if (sm_pvMaxAppAddr == NULL) 
    {
        //
        // Functions with address above lpMaximumApplicationAddress require
        // special processing (Windows 9x only)
        //
        SYSTEM_INFO si;
        GetSystemInfo(&si);
        sm_pvMaxAppAddr = si.lpMaximumApplicationAddress;
    } // if
   
    if (m_pfnOrig > sm_pvMaxAppAddr) 
    {
        //
        // The address is in a shared DLL; the address needs fixing up 
        //
        PBYTE pb = (PBYTE) m_pfnOrig;
        if (pb[0] == cPushOpCode) 
        {
            //
            // Skip over the PUSH op code and grab the real address
            //
            PVOID pv = * (PVOID*) &pb[1];
            m_pfnOrig = (PROC) pv;
        } // if
    } // if
}


//---------------------------------------------------------------------------
// ~CHookedFunction
//  
//
//---------------------------------------------------------------------------
CHookedFunction::~CHookedFunction()
{
    UnHookImport();
}


PCSTR CHookedFunction::Get_CalleeModName() const
{
    return const_cast<PCSTR>(m_szCalleeModName);
}

PCSTR CHookedFunction::Get_FuncName() const
{
    return const_cast<PCSTR>(m_szFuncName);
}

PROC CHookedFunction::Get_pfnHook() const
{
    return m_pfnHook;
}

PROC CHookedFunction::Get_pfnOrig() const
{
    return m_pfnOrig;
}

//---------------------------------------------------------------------------
// HookImport
//  
// Set up a new hook function
//---------------------------------------------------------------------------
BOOL CHookedFunction::HookImport()
{
    m_bHooked = DoHook(TRUE, m_pfnOrig, m_pfnHook);
    return m_bHooked;
}

//---------------------------------------------------------------------------
// UnHookImport
//  
// Restore the original API handler
//---------------------------------------------------------------------------
BOOL CHookedFunction::UnHookImport()
{
    if (m_bHooked)
        m_bHooked = !DoHook(FALSE, m_pfnHook, m_pfnOrig);
    return !m_bHooked;
}

//---------------------------------------------------------------------------
// ReplaceInAllModules
//  
// Replace the address of a imported function entry  in all modules
//---------------------------------------------------------------------------
BOOL CHookedFunction::ReplaceInAllModules(BOOL  bHookOrRestore,PCSTR pszCalleeModName, PROC  pfnCurrent,PROC  pfnNew) 
{
    BOOL bResult = FALSE;

    if ((NULL != pfnCurrent) && (NULL != pfnNew))
    {
        BOOL                bReplace  = FALSE;
        CExeModuleInstance  *pProcess = NULL;
        CTaskManager        taskManager; 
        CModuleInstance     *pModule;
        //
        // Retrieves information about current process and modules. 
        // The taskManager dynamically decides whether to use ToolHelp 
        // library or PSAPI
        //
        taskManager.PopulateProcess(::GetCurrentProcessId(), TRUE);
        pProcess = taskManager.GetProcessById(::GetCurrentProcessId());
        if (NULL != pProcess)
        {
            // Enumerates all modules loaded by (pProcess) process
            for (DWORD i = 0; i < pProcess->GetModuleCount(); i++)
            {
                pModule = pProcess->GetModuleByIndex(i);
                bReplace = (pModule->Get_Module() != ModuleFromAddress(CApiHookMgr::MyLoadLibraryA)); 

                // We don't hook functions in our own modules
                if (bReplace)
                    // Hook this function in this module
                    bResult = ReplaceInOneModule(pszCalleeModName, pfnCurrent, pfnNew, pModule->Get_Module()) || bResult;

            } // for
            // Hook this function in the executable as well
            bResult = ReplaceInOneModule(pszCalleeModName, pfnCurrent, pfnNew, pProcess->Get_Module()) || bResult;
        } // if
    } // if
    return bResult;
}


//---------------------------------------------------------------------------
// ReplaceInOneModule
//  
// Replace the address of the function in the IAT of a specific module
//---------------------------------------------------------------------------
BOOL CHookedFunction::ReplaceInOneModule(PCSTR pszCalleeModName,PROC pfnCurrent,PROC pfnNew, HMODULE hmodCaller) 
{
    BOOL bResult = FALSE;
    __try
    {
        ULONG ulSize;
        // Get the address of the module's import section
        PIMAGE_IMPORT_DESCRIPTOR pImportDesc = (PIMAGE_IMPORT_DESCRIPTOR)ImageDirectoryEntryToData(hmodCaller,TRUE,IMAGE_DIRECTORY_ENTRY_IMPORT,&ulSize);
        // Does this module has import section ?
        if (pImportDesc == NULL)
            __leave;  
        // Loop through all descriptors and
        // find the import descriptor containing references to callee's functions
        while (pImportDesc->Name)
        {
            PSTR pszModName = (PSTR)((PBYTE) hmodCaller + pImportDesc->Name);
            if (_stricmp(pszModName, pszCalleeModName) == 0) 
                break;   // Found
            pImportDesc++;
        } // while
        // Does this module import any functions from this callee ?
        if (pImportDesc->Name == 0)
            __leave;  
        // Get caller's IAT 
        PIMAGE_THUNK_DATA pThunk = 
            (PIMAGE_THUNK_DATA)( (PBYTE) hmodCaller + pImportDesc->FirstThunk );
        // Replace current function address with new one
        while (pThunk->u1.Function)
        {
            // Get the address of the function address
            PROC* ppfn = (PROC*) &pThunk->u1.Function;
            // Is this the function we're looking for?
            BOOL bFound = (*ppfn == pfnCurrent);
            // Is this Windows 9x
            if (!bFound && (*ppfn > sm_pvMaxAppAddr)) 
            {
                PBYTE pbInFunc = (PBYTE) *ppfn;
                // Is this a wrapper (debug thunk) represented by PUSH instruction?
                if (pbInFunc[0] == cPushOpCode) 
                {
                    ppfn = (PROC*) &pbInFunc[1];
                    // Is this the function we're looking for?
                    bFound = (*ppfn == pfnCurrent);
                } // if
            } // if

            if (bFound) 
            {
                MEMORY_BASIC_INFORMATION mbi;
                ::VirtualQuery(ppfn, &mbi, sizeof(MEMORY_BASIC_INFORMATION));
                // In order to provide writable access to this part of the 
                // memory we need to change the memory protection
                if (FALSE == ::VirtualProtect(mbi.BaseAddress,mbi.RegionSize,PAGE_READWRITE,&mbi.Protect)
                    )
                    __leave;
                // Hook the function.
        *ppfn = *pfnNew;
                bResult = TRUE;
                // Restore the protection back
        DWORD dwOldProtect;
                ::VirtualProtect(mbi.BaseAddress,mbi.RegionSize,mbi.Protect,&dwOldProtect);
                break;
            } // if
            pThunk++;
        } // while
    }
    __finally
    {
        // do nothing
    }
    // This function is not in the caller's import section
    return bResult;
}

//---------------------------------------------------------------------------
// DoHook
//  
// Perform actual replacing of function pointers
//---------------------------------------------------------------------------
BOOL CHookedFunction::DoHook(BOOL bHookOrRestore, PROC pfnCurrent, PROC pfnNew)
{
    // Hook this function in all currently loaded modules
    return ReplaceInAllModules(bHookOrRestore, m_szCalleeModName, pfnCurrent, pfnNew);
}

//
// Indicates whether the hooked function is mandatory one
//
BOOL CHookedFunction::IsMandatory()
{
    BOOL bResult = FALSE;
    API_FUNC_ID apiFuncId;
    for (int i = 0; i < NUMBER_OF_MANDATORY_API_FUNCS; i++)
    {
        apiFuncId = MANDATORY_API_FUNCS[i];
        if ( (0==_stricmp(apiFuncId.szCalleeModName, m_szCalleeModName)) && (0==_stricmp(apiFuncId.szFuncName, m_szFuncName)) )
        {
            bResult = TRUE;
            break;
        } // if
    } // for

    return bResult;
}

//---------------------------------------------------------------------------
// 
// class CHookedFunctions 
//
//---------------------------------------------------------------------------

CHookedFunctions::CHookedFunctions(CApiHookMgr* pApiHookMgr) :    m_pApiHookMgr(pApiHookMgr)
{

}

CHookedFunctions::~CHookedFunctions()
{

}


//---------------------------------------------------------------------------
// GetHookedFunction
//  
// Return the address of an CHookedFunction object
//---------------------------------------------------------------------------
CHookedFunction* CHookedFunctions::GetHookedFunction(HMODULE hmodOriginal, PCSTR   pszFuncName)
{
    char szFileName[MAX_PATH];
    ::GetModuleFileName(hmodOriginal, szFileName, MAX_PATH);
    // We must extract only the name and the extension
    ExtractModuleFileName(szFileName);

    return GetHookedFunction(szFileName, pszFuncName);
}

//---------------------------------------------------------------------------
// GetFunctionNameFromExportSection
//  
// Return the name of the function from EAT by its ordinal value
//---------------------------------------------------------------------------
BOOL CHookedFunctions::GetFunctionNameFromExportSection(HMODULE hmodOriginal,DWORD dwFuncOrdinalNum,PSTR pszFuncName) 
{
    BOOL bResult = FALSE;
    // Make sure we return a valid string (atleast an empty one)
    strcpy(pszFuncName, "\0");
    __try
    {
        ULONG ulSize;
        // Get the address of the module's export section
        PIMAGE_EXPORT_DIRECTORY pExportDir = (PIMAGE_EXPORT_DIRECTORY)ImageDirectoryEntryToData(hmodOriginal, TRUE,    IMAGE_DIRECTORY_ENTRY_EXPORT,    &ulSize);
        // Does this module has export section ?
        if (pExportDir == NULL)
            __leave;  
        // Get the name of the DLL
        PSTR pszDllName = reinterpret_cast<PSTR>( pExportDir->Name + (DWORD)hmodOriginal);
        // Get the starting ordinal value. By default is 1, but
        // is not required to be so
        DWORD dwFuncNumber = pExportDir->Base;
        // The number of entries in the EAT
        DWORD dwNumberOfExported = pExportDir->NumberOfFunctions;
        // Get the address of the ENT
        PDWORD pdwFunctions = (PDWORD)( pExportDir->AddressOfFunctions + (DWORD)hmodOriginal);
        //  Get the export ordinal table
        PWORD pwOrdinals = (PWORD)(pExportDir->AddressOfNameOrdinals + (DWORD)hmodOriginal);
        // Get the address of the array with all names
        DWORD *pszFuncNames =    (DWORD *)(pExportDir->AddressOfNames + (DWORD)hmodOriginal);

        PSTR pszExpFunName;

        // Walk through all of the entries and try to locate the
        // one we are looking for
        for (DWORD i = 0; i < dwNumberOfExported; i++, pdwFunctions++)
        {
            DWORD entryPointRVA = *pdwFunctions;
            if ( entryPointRVA == 0 )   // Skip over gaps in exported function
                continue;               // ordinals (the entrypoint is 0 for
                                        // these functions).
            // See if this function has an associated name exported for it.
            for (DWORD j=0; j < pExportDir->NumberOfNames; j++ )
            {
                // Note that pwOrdinals[x] return values starting form 0.. (not from 1)
                if ( pwOrdinals[j] == i  )
                {
                    pszExpFunName = (PSTR)(pszFuncNames[j] + (DWORD)hmodOriginal);
                    // Is this the same ordinal value ?
                    // Notice that we need to add 1 to pwOrdinals[j] to get actual 
                    // number
                    if (dwFuncOrdinalNum == pwOrdinals[j] + 1)
                    {
                        if ((pszExpFunName != NULL) && (strlen(pszExpFunName) > 0))
                            strcpy(pszFuncName, pszExpFunName);
                        __leave;
                    }
                }
            }
        } // for
    }
    __finally
    {
        // do nothing
    }
    // This function is not in the caller's import section
    return bResult;
}

//---------------------------------------------------------------------------
// GetFunctionNameByOrdinal
//  
// Return the name of the function by its ordinal value
//---------------------------------------------------------------------------
void CHookedFunctions::GetFunctionNameByOrdinal(PCSTR pszCalleeModName, DWORD dwFuncOrdinalNum, PSTR pszFuncName)
{
    HMODULE hmodOriginal = ::GetModuleHandle(pszCalleeModName);
    // Take the name from the export section of the DLL
    GetFunctionNameFromExportSection(hmodOriginal, dwFuncOrdinalNum, pszFuncName);
}



//---------------------------------------------------------------------------
// GetHookedFunction
//  
// Return the address of an CHookedFunction object
//---------------------------------------------------------------------------
CHookedFunction* CHookedFunctions::GetHookedFunction(PCSTR pszCalleeModName,PCSTR pszFuncName)
{
    CHookedFunction* pHook = NULL;
    char szFuncName[MAX_PATH];
    //
    // Prevent accessing invalid pointers and examine values 
    // for APIs exported by ordinal
    //
    if ( (pszFuncName) && ((DWORD)pszFuncName > 0xFFFF) &&  strlen(pszFuncName) ) 
    {
        strcpy(szFuncName, pszFuncName);
    } // if
    else
    {
        GetFunctionNameByOrdinal(pszCalleeModName, (DWORD)pszFuncName, szFuncName);
    }
    // Search in the map only if we have found the name of the requested function
    if (strlen(szFuncName) > 0)
    {
        char szKey[MAX_PATH];
        sprintf(szKey, "<%s><%s>", pszCalleeModName, szFuncName);
        // iterators can be used to check if an entry is in the map
        CHookedFunctions::const_iterator citr = find( szKey );
        if ( citr != end() )
            pHook = citr->second;
    } // if

    return pHook;
}



//---------------------------------------------------------------------------
// AddHook
//  
// Add a new object to the container
//---------------------------------------------------------------------------
BOOL CHookedFunctions::AddHook(CHookedFunction* pHook)
{
    BOOL bResult = FALSE;
    if (NULL != pHook)
    {
        char szKey[MAX_PATH];
        sprintf(szKey, "<%s><%s>", pHook->Get_CalleeModName(), pHook->Get_FuncName());
        // Find where szKey is or should be
        CHookedFunctions::iterator lb = lower_bound(szKey);
        //
        // when an "add" is performed, insert() is more efficient
        // than operator[].
        // For more details see -item 24 page 109 "Effective STL" by Meyers
        //
        // Adds pair(pszKey, pObject) to the map
        insert( lb, value_type(szKey, pHook) );
        //
        // added to the map
        //
        bResult = TRUE;
    } // if
    return bResult;
}

//---------------------------------------------------------------------------
// RemoveHook
//  
// Remove exising object pointer from the container
//---------------------------------------------------------------------------
BOOL CHookedFunctions::RemoveHook(CHookedFunction* pHook)
{
    BOOL bResult = FALSE;
    try
    {
        if (NULL != pHook)
        {
            char szKey[MAX_PATH];
            sprintf(szKey, "<%s><%s>", pHook->Get_CalleeModName(), pHook->Get_FuncName());
            //
            // Find where szKey is located 
            //
            CHookedFunctions::iterator itr = find(szKey);
            if (itr != end())
            {
                //delete itr->second;
                erase(itr);
            }
            bResult = TRUE;
        } // if
    }
    catch (...)
    {
        bResult = FALSE;
    }
    return bResult;
}
//----------------------End of file -----------------------------------------
