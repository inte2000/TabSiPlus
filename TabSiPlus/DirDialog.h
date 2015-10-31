////////////////////////////////////////////////////////////////////////
// DirDialog.h: interface for the CDirDialog class.
//
// Original Author : Unknown
// Minor Modifications - Jignesh Patel
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DIRDIALOG_H__62FFAC92_1DEE_11D1_B87A_0060979CDF6D__INCLUDED_)
#define AFX_DIRDIALOG_H__62FFAC92_1DEE_11D1_B87A_0060979CDF6D__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CDirDialog
{
public:
    CDirDialog();
    virtual ~CDirDialog();
    int DoBrowse(CWnd * pParentWnd = NULL, CString strTitle = _T(""), CString strBaseFolder = _T(""), CString strInitialSelectedFolder = _T(""));
    
    CString m_strPath;
    CString m_strSelDir;
private:
    CString m_strInitDir;
    CString m_strTitle;
    int  m_iImageIndex;
};

#endif // !defined(AFX_DIRDIALOG_H__62FFAC92_1DEE_11D1_B87A_0060979CDF6D__INCLUDED_)


