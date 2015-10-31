////////////////////////////////////////////////////////////////////
// PIDL.cpp: implementation of the CPIDL class.
//
// By Oz Solomonovich (osolo@bigfoot.com)
#include "stdafx.h"
#include "PIDL.h"

//##ModelId=431C34CA013B
LPSHELLFOLDER CPIDL::m_sfDesktop  = NULL; // cached destkop folder
//##ModelId=431C34CA0149
LPMALLOC      CPIDL::m_pAllocator = NULL; // cahced system allocator

CPIDL::pidl_initializer CPIDL::m_initializer; // automatic init. obj


////////////////////////////////////////////////////////////////////
// Initialization object
////////////////////////////////////////////////////////////////////

// pidl_initializer is used to initialized our static data.  The 
// constructor and destructor are automatically called for us when
// the program starts/ends.

//##ModelId=431C34CA0196
CPIDL::pidl_initializer::pidl_initializer()
{
    SHGetDesktopFolder(&m_sfDesktop); // cache d'top folder obj ptr 
    SHGetMalloc(&m_pAllocator);       // cache sys allocator obj ptr
}

//##ModelId=431C34CA0197
CPIDL::pidl_initializer::~pidl_initializer()
{
    // decrement COM reference counters:
    m_sfDesktop->Release();
    m_pAllocator->Release();
}


////////////////////////////////////////////////////////////////////
// CPIDL Implementation
////////////////////////////////////////////////////////////////////

//##ModelId=431C34CA000F
CPIDL::~CPIDL()
{
    Free();  // just free used memory
}


////////////////////////////////////////////////////////////////////
// Assignment Functions

//##ModelId=431C34CA001F
HRESULT CPIDL::Set(const CPIDL& cpidl)
{
    return MakeCopyOf(cpidl.m_pidl);
}

//##ModelId=431C34CA006E
HRESULT CPIDL::Set(LPITEMIDLIST pidl)
{
    Free();
    m_pidl = pidl;
    return ERROR_SUCCESS;
}

//##ModelId=431C34CA005D
HRESULT CPIDL::Set(LPCSTR szPath, LPSHELLFOLDER psf)
{
    OLECHAR olePath[MAX_PATH];
    ULONG   chEaten;
    ULONG   dwAttributes;
    
    Free();
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szPath, -1, 
        olePath, MAX_PATH);
    return psf->ParseDisplayName(NULL, NULL, olePath, &chEaten, 
        &m_pidl, &dwAttributes);
}

//##ModelId=431C34CA007D
HRESULT CPIDL::MakeCopyOf(LPITEMIDLIST pidl)
{
    Free();
    if (pidl) {
        UINT sz = m_pAllocator->GetSize((LPVOID)pidl);
        AllocMem(sz);
        CopyMemory((LPVOID)m_pidl, (LPVOID)pidl, sz);
    }
    return ERROR_SUCCESS;
}

//##ModelId=431C34CA008C
HRESULT CPIDL::MakeAbsPIDLOf(LPSHELLFOLDER psf, LPITEMIDLIST pidl)
{
    OLECHAR szOleChar[MAX_PATH];
    ULONG   ulEaten, ulAttribs;
    HRESULT hr;
    STRRET  str;
    
    Free();    
    
    hr = psf->GetDisplayNameOf(pidl, SHGDN_FORPARSING, &str);
    if (SUCCEEDED(hr)) 
    {
        ExtractCStr(str);
        MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str.cStr, -1, (LPWSTR)szOleChar, sizeof(szOleChar));
        hr = m_sfDesktop->ParseDisplayName(NULL, NULL, szOleChar, &ulEaten, &m_pidl, &ulAttribs);
    }
    
    return hr;
}


////////////////////////////////////////////////////////////////////
// CPIDL Operations

// Frees memory used by the PIDL.
//##ModelId=431C34CA00BB
void CPIDL::Free()
{
    if (m_pidl) {
        m_pAllocator->Free(m_pidl);
        m_pidl = NULL;
    }
}

//##ModelId=431C34CA00BC
UINT CPIDL::GetSize() const
{
    UINT        cbTotal = 0;
    LPSHITEMID  pid     = GetFirstItemID();
    
    if (pid) {
        do {
            cbTotal += pid->cb;
            GetNextItemID(pid);
        } while (pid->cb);
        cbTotal += sizeof(pid->cb); // count the terminator
    }
    
    return cbTotal;
}

#define CB_SIZE  (sizeof(piid->cb))     // size of terminator

//##ModelId=431C34CA00BE
void CPIDL::Split(CPIDL& root, CPIDL& obj) const
{
    int         size;
    SHITEMID    *piid, *piidLast;
    
    // find last item-id and calculate total size of pidl
    size = 0;
    piid = piidLast = &m_pidl->mkid;
    while (piid->cb)
    {
        piidLast = piid;
        size += (piid->cb);
        piid =  (SHITEMID *)((LPBYTE)piid + (piid->cb));
    }
    
    // copy "root" portion
    size -= piidLast->cb;  // don't count "object" item-id
    root.AllocMem(size + CB_SIZE);
    CopyMemory(root.m_pidl, m_pidl, size);
    ZeroMemory((LPBYTE)root.m_pidl + size, CB_SIZE); // terminator
    
    // copy "object" portion
    size = piidLast->cb + CB_SIZE;
    obj.AllocMem(size);
    CopyMemory(obj.m_pidl, piidLast, size);
}

//##ModelId=431C34CA00CE
CPIDL CPIDL::operator + (CPIDL& pidl) const
{
    CPIDL ret;
    Concat(*this, pidl, ret);
    return ret;
}

//##ModelId=431C34CA00DB
void CPIDL::Concat(const CPIDL &a, const CPIDL& b, CPIDL& result)
{
    result.Free();
    
    // both empty?
    if (a.m_pidl == NULL  &&  b.m_pidl == NULL) return;
    
    // a empty?  return b
    if (a.m_pidl == NULL) { result.Set(b); return; }
    
    // b empty?  return a 
    if (a.m_pidl == NULL) { result.Set(a); return; }
    
    UINT cb1, cb2;
    cb1 = a.GetSize() - sizeof(a.m_pidl->mkid.cb);
    cb2 = b.GetSize(); 
    
    result.AllocMem(cb1 + cb2); // allocate enough memory 
    CopyMemory(result.m_pidl, a.m_pidl, cb1);                 // 1st
    CopyMemory(((LPBYTE)result.m_pidl) + cb1, b.m_pidl, cb2); // 2nd
}

//##ModelId=431C34CA00FA
HRESULT CPIDL::GetUIObjectOf(REFIID riid, LPVOID *ppvOut, 
    HWND hWnd /*= NULL*/, LPSHELLFOLDER psf /*= m_sfDesktop*/)
{
    CPIDL           root, obj;
    LPSHELLFOLDER   psfRoot;
    HRESULT         hr;
    
    Split(root, obj);    
    
    // get the IShellFolder of the root
    hr = psf->BindToObject(root, NULL, IID_IShellFolder, 
        (LPVOID *)&psfRoot);
    
    if (SUCCEEDED(hr)) {
        hr = psfRoot->GetUIObjectOf(hWnd, 1, obj, riid, 0, ppvOut);
        psfRoot->Release();
    }
    
    return hr;
}

//##ModelId=431C34CA0119
void CPIDL::ExtractCStr(STRRET& strRet) const
{
    switch (strRet.uType) 
    {
        case STRRET_WSTR:
        {
            // pOleStr points to a WCHAR string - convert it to ANSI
            LPWSTR pOleStr = strRet.pOleStr;
            WideCharToMultiByte(CP_ACP, 0, pOleStr, -1,
                strRet.cStr, MAX_PATH, NULL, NULL);
            m_pAllocator->Free(pOleStr);
            break;
        }
        
        case STRRET_OFFSET:
            // The string lives inside the pidl, so copy it out.
            lstrcpyn(strRet.cStr, (LPSTR)
                ((LPBYTE)m_pidl + strRet.uOffset), MAX_PATH);
            break;
        
        case STRRET_CSTR:
            // The string already is in the right place.
            break;
    }
}


////////////////////////////////////////////////////////////////////
// CPIDL Private Operations

//##ModelId=431C34CA0157
void CPIDL::AllocMem(int iAllocSize)
{
    Free();
    m_pidl = (LPITEMIDLIST)m_pAllocator->Alloc(iAllocSize);
}
