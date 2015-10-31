//---------------------------------------------------------------------------
// ModuleInstance.h
//
// AUTHOR:        Ivo Ivanov (ivopi@hotmail.com)
// ver 1.02
// 05 September 2001
//---------------------------------------------------------------------------

#if !defined(_MODULEINSTANCE_H_)
#define _MODULEINSTANCE_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//---------------------------------------------------------------------------
//
// Standard includes
//
//---------------------------------------------------------------------------
#include <iostream>
#include <vector>
#include <windows.h>
#include <tlhelp32.h>

//---------------------------------------------------------------------------
//
// Forward declaration
//
//---------------------------------------------------------------------------
class CModuleInstance;
class CModuleList;
class CRunningProcesses;
class CExeModuleInstance;

//---------------------------------------------------------------------------
//
//                   typedefs for ToolHelp32 functions 
//
// We must link to these functions of Kernel32.DLL explicitly. Otherwise 
// a module using this code would fail to load under Windows NT, which does not 
// have the Toolhelp32 functions in the Kernel 32.
//
//---------------------------------------------------------------------------
typedef HANDLE (WINAPI * PFNCREATETOOLHELP32SNAPSHOT)(
    DWORD dwFlags,
    DWORD th32ProcessID
    );

typedef BOOL (WINAPI * PFNPROCESS32FIRST)(
    HANDLE hSnapshot,
    LPPROCESSENTRY32 lppe
    );

typedef BOOL (WINAPI * PFNPROCESS32NEXT)(
    HANDLE hSnapshot,
    LPPROCESSENTRY32 lppe
    );

typedef BOOL (WINAPI * PFNMODULE32FIRST)(
    HANDLE hSnapshot,
    LPMODULEENTRY32 lpme
    );

typedef BOOL (WINAPI * PFNMODULE32NEXT)(
    HANDLE hSnapshot,
    LPMODULEENTRY32 lpme
    );


//---------------------------------------------------------------------------
//
//                   typedefs for PSAPI.DLL functions 
//
//---------------------------------------------------------------------------
typedef BOOL (WINAPI * PFNENUMPROCESSES)(
    DWORD * lpidProcess,
    DWORD   cb,
    DWORD * cbNeeded
    );

typedef BOOL (WINAPI * PFNENUMPROCESSMODULES)(
    HANDLE hProcess,
    HMODULE *lphModule,
    DWORD cb,
    LPDWORD lpcbNeeded
    );

typedef DWORD (WINAPI * PFNGETMODULEFILENAMEEXA)(
    HANDLE hProcess,
    HMODULE hModule,
    LPSTR lpFilename,
    DWORD nSize
    );


//---------------------------------------------------------------------------
//
// class CModuleList
//
// A simple container for maintaining CModuleInstance objects 
// 
//---------------------------------------------------------------------------
class CModuleList: private std::vector<CModuleInstance*>  
{
public:
    CModuleList();
    virtual ~CModuleList();
    //
    // Add new object to the container
    //
    void Add(CModuleInstance &moduleInstance);
    // 
    // Release all objects and clear the list
    //
    void ReleaseAll();
    //
    // Return a module object by its index (Pascal-like style)
    //
    CModuleInstance* GetModule(DWORD dwIndex) const;
    //
    // Return number of items in the container
    //
    DWORD GetCount() const;
};

//---------------------------------------------------------------------------
//
// class CElements
//
//---------------------------------------------------------------------------
class CElements: public CModuleList  
{
public:
    CElements();
    virtual ~CElements();
};

//---------------------------------------------------------------------------
//
// class CLibHandler
//
//---------------------------------------------------------------------------
class CLibHandler  
{
public:
    CLibHandler(CRunningProcesses* pProcesses);
    virtual ~CLibHandler();
    
    virtual BOOL PopulateModules(CModuleInstance* pProcess) = 0;
    virtual BOOL PopulateProcesses(BOOL bPopulateModules) = 0;
    virtual BOOL PopulateProcess(DWORD dwProcessId, BOOL bPopulateModules) = 0;
protected:
    CRunningProcesses* m_pProcesses;
};

//---------------------------------------------------------------------------
//
// class CTaskManager
//
//---------------------------------------------------------------------------
class CTaskManager  
{
public:
    CTaskManager();
    virtual ~CTaskManager();

    BOOL Populate(BOOL bPopulateModules);
    BOOL PopulateProcess(DWORD dwProcessId, BOOL bPopulateModules);
    DWORD GetProcessCount() const;
    CExeModuleInstance* GetProcessByIndex(DWORD dwIndex);
    CExeModuleInstance* GetProcessById(DWORD dwProcessId);
private:
    CLibHandler       *m_pLibHandler;
    CRunningProcesses *m_pProcesses;
};


//---------------------------------------------------------------------------
//
// class CLoadedModules
//
//---------------------------------------------------------------------------
class CLoadedModules: public CElements
{
public:
    CLoadedModules(DWORD dwProcessId);
    virtual ~CLoadedModules();
protected:
    DWORD m_dwProcessId;
};

//---------------------------------------------------------------------------
//
// class CRunningProcesses
//
//---------------------------------------------------------------------------
class CRunningProcesses: public CElements
{
public:
    CRunningProcesses();
    virtual ~CRunningProcesses();
    CExeModuleInstance* GetProcessById(DWORD dwProcessId);
};


//---------------------------------------------------------------------------
//
// class CPsapiHandler
//
//---------------------------------------------------------------------------
class CPsapiHandler: public CLibHandler
{
public:
    CPsapiHandler(CRunningProcesses* pProcesses);
    virtual ~CPsapiHandler();

    BOOL Initialize();
    void Finalize();
    virtual BOOL PopulateModules(CModuleInstance* pProcess);
    virtual BOOL PopulateProcesses(BOOL bPopulateModules);
    virtual BOOL PopulateProcess(DWORD dwProcessId, BOOL bPopulateModules);
private:
    HMODULE                 m_hModPSAPI;
    PFNENUMPROCESSES        m_pfnEnumProcesses;
    PFNENUMPROCESSMODULES   m_pfnEnumProcessModules;
    PFNGETMODULEFILENAMEEXA m_pfnGetModuleFileNameExA;
};

//---------------------------------------------------------------------------
//
// class CToolhelpHandler
//
//---------------------------------------------------------------------------
class CToolhelpHandler: public CLibHandler
{
public:
    CToolhelpHandler(CRunningProcesses* pProcesses);
    virtual ~CToolhelpHandler();

    BOOL Initialize();
    virtual BOOL PopulateModules(CModuleInstance* pProcess);
    virtual BOOL PopulateProcesses(BOOL bPopulateModules);
    virtual BOOL PopulateProcess(DWORD dwProcessId, BOOL bPopulateModules);
private:
    BOOL ModuleFirst(HANDLE hSnapshot, PMODULEENTRY32 pme) const;
    BOOL ModuleNext(HANDLE hSnapshot, PMODULEENTRY32 pme) const;
    BOOL ProcessFirst(HANDLE hSnapshot, PROCESSENTRY32* pe32) const;
    BOOL ProcessNext(HANDLE hSnapshot, PROCESSENTRY32* pe32) const;
    //
    // ToolHelp function pointers
    //
    PFNCREATETOOLHELP32SNAPSHOT m_pfnCreateToolhelp32Snapshot;
    PFNPROCESS32FIRST           m_pfnProcess32First;
    PFNPROCESS32NEXT            m_pfnProcess32Next;
    PFNMODULE32FIRST            m_pfnModule32First;
    PFNMODULE32NEXT             m_pfnModule32Next;
};

//---------------------------------------------------------------------------
//
// class CModuleInstance
//
// Represents exactly one loaded module
// 
//---------------------------------------------------------------------------
class CModuleInstance  
{
public:
    CModuleInstance(char *pszName, HMODULE hModule);
    virtual ~CModuleInstance();

    void AddModule(CModuleInstance* pModuleInstance);
    void ReleaseModules();

    // Returns Full path and filename of the executable file for the process or DLL
    char*    Get_Name() const;
    // Sets Full path and filename of the executable file for the process or DLL
    void     Set_Name(char *pszName);
    // Returns module handle
    HMODULE  Get_Module() const;
    void     Set_Module(HMODULE module);
    // Returns only the filename of the executable file for the process or DLL
    char*   GetBaseName() const;

private:
    char        *m_pszName;
    HMODULE      m_hModule;
protected:
    CModuleList *m_pInternalList;
};

//---------------------------------------------------------------------------
//
// class CExeModuleInstance
//
// Represents exactly one loaded EXE module
// 
//---------------------------------------------------------------------------
class CExeModuleInstance: public CModuleInstance
{
public:
    CExeModuleInstance(
        CLibHandler* pLibHandler,
        char*        pszName, 
        HMODULE      hModule, 
        DWORD        dwProcessId
        );
    virtual ~CExeModuleInstance();
    // Returns process id
    DWORD Get_ProcessId() const;
    BOOL PopulateModules();
    DWORD GetModuleCount();
    CModuleInstance* GetModuleByIndex(DWORD dwIndex);
private:
    DWORD        m_dwProcessId;
    CLibHandler* m_pLibHandler;
};


#endif // !defined(_MODULEINSTANCE_H_)
//--------------------- End of the file -------------------------------------