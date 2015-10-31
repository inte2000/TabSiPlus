///////////////////////////////////////////////////////////////////////////
// DirDialog.cpp: implementation of the CDirDialog class.
//
// Original Author : Unknown
// Minor Modifications - Jignesh Patel
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DirDialog.h"
#include "shlobj.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// Callback function called by SHBrowseForFolder's browse control
// after initialization and when selection changes
static int __stdcall BrowseCtrlCallback(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if (uMsg == BFFM_INITIALIZED)
    {
        CDirDialog* pDirDialogObj = (CDirDialog*)lpData;
        if(!pDirDialogObj->m_strSelDir.IsEmpty())
            ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)(LPCTSTR)(pDirDialogObj->m_strSelDir));
    }
    else if (uMsg == BFFM_SELCHANGED)
    {
        //    The item identifier list for the newly selected folder is passed in the lParam and NOT
        //    in the pData as the online documentation claims.
        //    Infact most of the documentation on the BrowseCallbackProc function is incorrect
        //    and have been addressed in the SDK annotations by WDJ.
        //    These can be found at ftp://ftp.mfi.com/pub/windev/wdjnotes/wdjnotes.zip

        CString strSelectedPath;
        LPITEMIDLIST pIdl = (LPITEMIDLIST)lParam;

        if (::SHGetPathFromIDList(pIdl,strSelectedPath.GetBuffer(MAX_PATH)) == FALSE)
        {
            return 0;
        }
        strSelectedPath.ReleaseBuffer();
        if(!strSelectedPath.IsEmpty())
            ::SendMessage(hwnd, BFFM_SETSTATUSTEXT, TRUE, (LPARAM)(LPCTSTR)(strSelectedPath));
    }
    return 0;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDirDialog::CDirDialog()
{
    m_strTitle = _T("");
    m_strInitDir = _T("");
    m_strSelDir = _T("");
}

CDirDialog::~CDirDialog()
{
}

int CDirDialog::DoBrowse(CWnd * pParentWnd, CString strTitle, CString strBaseFolder, CString strInitialSelectedFolder)
{
    m_strInitDir = strBaseFolder;
    m_strSelDir = strInitialSelectedFolder;

    LPMALLOC pMalloc;
    if (SHGetMalloc (&pMalloc)!= NOERROR)
    {
        return 0;
    }
    
    BROWSEINFO bInfo;
    LPITEMIDLIST pidl;
    ZeroMemory ( (PVOID) &bInfo,sizeof (BROWSEINFO));
    
    if (!m_strInitDir.IsEmpty ())
    {
        OLECHAR       olePath[MAX_PATH];
        ULONG         chEaten;
        ULONG         dwAttributes;
        HRESULT       hr;
        LPSHELLFOLDER pDesktopFolder;
        // 
        // Get a pointer to the Desktop's IShellFolder interface. 
        //
        if (SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
        {
            //
            // IShellFolder::ParseDisplayName requires the file name be in Unicode.
            //
            MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, m_strInitDir.GetBuffer(MAX_PATH), -1,
                olePath, MAX_PATH);
            
            m_strInitDir.ReleaseBuffer (-1);
            //
            // Convert the path to an ITEMIDLIST.
            //
            hr = pDesktopFolder->ParseDisplayName(NULL,
                NULL,
                olePath,
                &chEaten,
                &pidl,
                &dwAttributes);
            if (FAILED(hr))
            {
                pMalloc ->Free (pidl);
                pMalloc ->Release ();
                return 0;
            }
            bInfo.pidlRoot = pidl;
            
        }
    }
    if(!pParentWnd)
        bInfo.hwndOwner = NULL;
    else
    {
        bInfo.hwndOwner = pParentWnd->GetSafeHwnd();    
    }

    bInfo.pszDisplayName = m_strPath.GetBuffer (MAX_PATH);
    if(strTitle.IsEmpty())
        bInfo.lpszTitle = "Choose folder.";
    else
        bInfo.lpszTitle = strTitle;
    bInfo.ulFlags = BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS|BIF_STATUSTEXT;
    bInfo.lpfn = BrowseCtrlCallback;  // address of callback function
    bInfo.lParam = (LPARAM)this;      // pass address of object to callback function
    
    if ((pidl = ::SHBrowseForFolder(&bInfo)) == NULL)
    {
        return 0;
    }
    m_strPath.ReleaseBuffer();
    m_iImageIndex = bInfo.iImage;
    
    if (::SHGetPathFromIDList(pidl,m_strPath.GetBuffer(MAX_PATH)) == FALSE)
    {
        pMalloc ->Free(pidl);
        pMalloc ->Release();
        return 0;
    }
    
    m_strPath.ReleaseBuffer();
    
    pMalloc ->Free(pidl);
    pMalloc ->Release();
    
    return 1;
}