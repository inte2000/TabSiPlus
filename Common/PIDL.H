////////////////////////////////////////////////////////////////////
// PIDL.h: interface for the CPIDL class.
//

#ifndef __PIDL_H
#define __PIDL_H

#include <shlobj.h>

//##ModelId=431C34C903A9
class CPIDL
{
public:
    //##ModelId=431C34C903C9
    LPITEMIDLIST  m_pidl;

// == Construction/Destruction == //

    // Empty object
    //##ModelId=431C34C903D8
    CPIDL() : m_pidl(NULL) {}

    // Copy constructor
    //##ModelId=431C34C903D9
    CPIDL(const CPIDL& cpidl) : m_pidl(NULL) { Set(cpidl); }

    // From path (szPath relevant to the folder psf) - see Set()
    //##ModelId=431C34C903DB
    CPIDL(LPCSTR szPath, LPSHELLFOLDER psf = m_sfDesktop) : 
        m_pidl(NULL) { Set(szPath, psf); }

    // Construct from an ITEMIDLIST pointer - does NOT copy the
    // ITEMIDLIST data (use MakeCopyOf() for that)
    //##ModelId=431C34CA0002
    CPIDL(LPITEMIDLIST pidl) : m_pidl(pidl) {}

    // destructor
    //##ModelId=431C34CA000F
    virtual ~CPIDL();


// == Assignment == //

    // All of the assignment operators *free* the current list.
    // If you we to set a new item list without freeing the old one,
    // you have to set m_pidl to NULL before assigning.

    // Make a copy of cpidl's list data
    //##ModelId=431C34CA001F
    HRESULT Set(const CPIDL& cpidl);

    // Set by path: szPath relative to the folder psf.  Uses
    // the desktop by default, so you can use regular path strings
    // like "c:\windows"
    //##ModelId=431C34CA005D
    HRESULT Set(LPCSTR szPath, LPSHELLFOLDER psf = m_sfDesktop);

    // Points the CPIDL to an existing item list: does *not* 
    // copy the actual data - just the pointer.  To make a copy of 
    // the data, use MakeCopyOf()
    //##ModelId=431C34CA006E
    HRESULT Set(LPITEMIDLIST pidl);

// == Special Assignments == //

    // Takes a pointer to an existing item list and makes a new
    // copy of the list data.
    //##ModelId=431C34CA007D
    HRESULT MakeCopyOf(LPITEMIDLIST pidl);

    // An absolute PIDL is a PIDL with the desktop as a root.  Use
    // this function to create an absolute PIDL of any other pidl
    // rooted at the folder pointed by psf.
    //##ModelId=431C34CA008C
    HRESULT MakeAbsPIDLOf(LPSHELLFOLDER psf, LPITEMIDLIST pidl);


// == Item Access == //

    // Returns a pointer to the first item in the list
    //##ModelId=431C34CA009C
    LPSHITEMID GetFirstItemID() const 
        { return (LPSHITEMID)m_pidl; }

    // Points to the next item in the list
    //##ModelId=431C34CA009E
    void GetNextItemID(LPSHITEMID& pid) const 
        { (LPBYTE &)pid += pid->cb; }


// == General Operations == //

    // Frees the memory used by the item id list
    //##ModelId=431C34CA00BB
    void Free();

    // Counts the amount memory needed to store the id list
    //##ModelId=431C34CA00BC
    UINT GetSize() const;

    // A pidl points to a name-space object such as a file or 
    // directory, such as "c:\windows\system\kernel32.dll"
    // This function will split the pidl into two pidls:
    // 1. the pidl of the object's root
    // 2. the pidl of the object, relative to it's root
    // Example: for the above file
    // root - is set to the pidl of "c:\windows\system"
    // obj  - is set to the pidl of "kernel" (relative to root)
    //##ModelId=431C34CA00BE
    void Split(CPIDL& root, CPIDL& obj) const;

    // Concatenation using the + operator
    //##ModelId=431C34CA00CE
    CPIDL operator + (CPIDL& pidl) const;

    // Concatenation - faster version using references
    //##ModelId=431C34CA00DB
    static void Concat(const CPIDL &a, const CPIDL& b, 
        CPIDL& result);  // (result = a+b)


// == Shell Name-space Access Helper Functions == //

    // IShellFolder->GetUIObjectOf works only for 1 item pidls (i.e.
    // objects that are direct descendants of the folder).
    // This following function allows you to retrieve a UI object of
    // any pidl rooted at the folder psf.
    // Example:
    // psf -> IShellFolder of "c:\windows",
    // pidl -> pidl of "system\kernel32.dll" (relative to psf)
    // 1) Will *not* work psf->GetUIObjectOf(pidl, ... )
    // 2) Will work: pidl.GetUIObjectOf(..., psf)
    //##ModelId=431C34CA00FA
    HRESULT GetUIObjectOf(REFIID riid, LPVOID *ppvOut, 
        HWND hWnd = NULL, LPSHELLFOLDER psf = m_sfDesktop);

    // Helper function: Given an STRRET structure, will ensure that
    // the returned string is moved to the cStr field.  
    // For use after functions like IShellFolder->GetDisplayNameOf()
    // Note: memory used the OLE strings is automatically freed.
    //##ModelId=431C34CA0119
    void ExtractCStr(STRRET& strRet) const;


// == Conversion Operators == //

    //##ModelId=431C34CA0128
    operator LPITEMIDLIST&  () { return m_pidl; }
    //##ModelId=431C34CA0129
    operator LPITEMIDLIST * () { return &m_pidl; }
    //##ModelId=431C34CA012A
    operator LPCITEMIDLIST  () const { return m_pidl; }
    //##ModelId=431C34CA0138
    operator LPCITEMIDLIST* () const 
        { return (LPCITEMIDLIST *)&m_pidl; }

protected:

    //##ModelId=431C34CA013B
    static LPSHELLFOLDER    m_sfDesktop;    // desktop object
    //##ModelId=431C34CA0149
    static LPMALLOC         m_pAllocator;   // system allocator

    // allocate memory for the pidl using the system allocator
    //##ModelId=431C34CA0157
    void AllocMem(int iAllocSize);

    // initializer (used for automatic initialization)
    //##ModelId=431C34CA0177
    static struct pidl_initializer {
        //##ModelId=431C34CA0196
        pidl_initializer();
        //##ModelId=431C34CA0197
        ~pidl_initializer();
    } m_initializer;
    friend struct pidl_initializer;
};

#endif  // __PIDL_H
