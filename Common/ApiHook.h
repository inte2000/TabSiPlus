//---------------------------------------------------------------------------
//
// ApiHook.h
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
//                A newly created object CHookedFunction must be always inserted 
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

#if !defined(_APIHOOK_H_)
#define _APIHOOK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//---------------------------------------------------------------------------
//
// Includes
//
//---------------------------------------------------------------------------
#include <map>
#include <string>
using namespace std;
#include "ModuleInstance.h"
#include "SyncObject.h"

//---------------------------------------------------------------------------
//
// Forward declarations
//
//---------------------------------------------------------------------------

class CHookedFunctions;

//---------------------------------------------------------------------------
//
// class CApiHookMgr
//  
//---------------------------------------------------------------------------
class CApiHookMgr  
{
public:
    CApiHookMgr();
    virtual ~CApiHookMgr();
public:
    //
    // Hook up an API 
    //
    BOOL HookImport(PCSTR pszCalleeModName, PCSTR pszFuncName, PROC  pfnHook);
    //
    // Restore hooked up API function
    //
    BOOL UnHookImport(PCSTR pszCalleeModName, PCSTR pszFuncName);
    // 
    // Hook all needed system functions in order to trap loading libraries
    //
    BOOL HookSystemFuncs();
    // 
    // Unhook all functions and restore original ones
    //
    void UnHookAllFuncs();
    //
    // Indicates whether there is hooked function
    //
    BOOL AreThereHookedFunctions();
private:
    //
    // Let's allow CApiHookMgr to access private methods of CHookedFunction    
    //
    friend class CHookedFunction;
    //
    //
    // Create a critical section on the stack
    //
    static CEventEx sm_CritEvent;
    //
    // Handle to current module
    //
    HMODULE m_hmodThisInstance;
    //
    // Container keeps track of all hacked functions
    // 
    static CHookedFunctions* sm_pHookedFunctions;
    //
    // Determines whether all system functions has been successfuly hacked
    //
    BOOL m_bSystemFuncsHooked;
    //
    // Used when a DLL is newly loaded after hooking a function
    //
    static void WINAPI HackModuleOnLoad(HMODULE hmod, DWORD dwFlags);
    //
    // Used to trap events when DLLs are loaded 
    //
    static HMODULE WINAPI MyLoadLibraryA(PCSTR  pszModuleName);
    static HMODULE WINAPI MyLoadLibraryW(PCWSTR pszModuleName);
    static HMODULE WINAPI MyLoadLibraryExA(PCSTR  pszModuleName, HANDLE hFile, DWORD dwFlags );
    static HMODULE WINAPI MyLoadLibraryExW(PCWSTR pszModuleName, HANDLE hFile, DWORD dwFlags);
    //
    // Returns address of replacement function if hooked function is requested
    //
    static FARPROC WINAPI MyGetProcAddress(HMODULE hmod, PCSTR pszProcName);
    //
    // Returns original address of the API function
    //
    static FARPROC WINAPI GetProcAddressWindows(HMODULE hmod, PCSTR pszProcName);
    //
    // Add a newly intercepted function to the container
    //
    BOOL AddHook(PCSTR  pszCalleeModName, PCSTR pszFuncName, PROC pfnOrig, PROC pfnHook);
    //
    // Remove intercepted function from the container
    //
    BOOL RemoveHook(PCSTR pszCalleeModName, PCSTR pszFuncName);
};


//---------------------------------------------------------------------------
//
// class CHookedFunction
//  
//---------------------------------------------------------------------------
class CHookedFunction  
{
public:
    CHookedFunction(CHookedFunctions* pHookedFunctions,PCSTR pszCalleeModName, PCSTR pszFuncName, PROC pfnOrig,PROC pfnHook);
    virtual ~CHookedFunction();

  PCSTR Get_CalleeModName() const;
    PCSTR Get_FuncName() const;
    PROC Get_pfnHook() const;
    PROC Get_pfnOrig() const;
    //
    // Set up a new hook function
    //
    BOOL HookImport();
    //
    // Restore the original API handler
    //
    BOOL UnHookImport();
    //
    // Replace the address of the function in the IAT of a specific module
    //
    BOOL ReplaceInOneModule(PCSTR pszCalleeModName, PROC pfnCurrent,PROC pfnNew,HMODULE hmodCaller);
    //
    // Indicates whether the hooked function is mandatory one
    //
    BOOL IsMandatory();

private:
    CHookedFunctions* m_pHookedFunctions;
    BOOL              m_bHooked;
    char              m_szCalleeModName[MAX_PATH];
    char              m_szFuncName[MAX_PATH];
    PROC              m_pfnOrig;
    PROC              m_pfnHook;
    //
    // Maximum private memory address
    //
    static  PVOID   sm_pvMaxAppAddr;    
    //
    // Perform actual replacing of function pointers
    // 
    BOOL DoHook(BOOL bHookOrRestore, PROC pfnCurrent, PROC pfnNew);
    //
    // Replace the address of a imported function entry  in all modules
    //
    BOOL ReplaceInAllModules(BOOL bHookOrRestore, PCSTR pszCalleeModName, PROC pfnCurrent, PROC pfnNew);
};


//---------------------------------------------------------------------------
//
// class CNocaseCmp
//
// Implements case-insensitive string compare
//
//---------------------------------------------------------------------------
class CNocaseCmp
{
public:
    //
    // A built-in highly efficient method for case-insensitive string compare.
    // Returns true, when string x is less than string y
    //
    bool operator()(const string& x, const string& y) const
    {
        return ( _stricmp(x.c_str(), y.c_str()) < 0 );
    }
};


//---------------------------------------------------------------------------
//
// class CHookedFunctions
//
//---------------------------------------------------------------------------
class CHookedFunctions: public map<string, CHookedFunction*, CNocaseCmp>
{
public:
    CHookedFunctions(CApiHookMgr* pApiHookMgr);
    virtual ~CHookedFunctions();
public:
    // 
    // Return the address of an CHookedFunction object
    //
    CHookedFunction* GetHookedFunction(PCSTR pszCalleeModName, PCSTR pszFuncName);
    //
    // Return the address of an CHookedFunction object
    //
    CHookedFunction* GetHookedFunction(HMODULE hmod, PCSTR pszFuncName);
    //
    // Add a new object to the container
    //
    BOOL AddHook(CHookedFunction* pHook);
    //
    // Remove exising object pointer from the container
    //
    BOOL RemoveHook(CHookedFunction* pHook);
private:
    //  
    // Return the name of the function from EAT by its ordinal value
    //
    BOOL GetFunctionNameFromExportSection(HMODULE hmodOriginal, DWORD dwFuncOrdinalNum, PSTR pszFuncName); 
    //  
    // Return the name of the function by its ordinal value
    //
    void GetFunctionNameByOrdinal(PCSTR pszCalleeModName, DWORD dwFuncOrdinalNum,    PSTR pszFuncName);

    CApiHookMgr* m_pApiHookMgr;
};

#endif // !defined(_APIHOOK_H_)
//----------------------End of file -----------------------------------------------------
