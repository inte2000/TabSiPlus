#if !defined(AFX_SELECTFOLDERDLG_H__C765D8D9_1957_4EE9_8C70_98C18A226657__INCLUDED_)
#define AFX_SELECTFOLDERDLG_H__C765D8D9_1957_4EE9_8C70_98C18A226657__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectFolderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectFolderDlg dialog

class CSelectFolderDlg : public CDialog
{
// Construction
public:
    CSelectFolderDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CSelectFolderDlg)
    enum { IDD = IDD_SELECT_FOLDER_DLG };
    CString    m_strFolderName;
    BOOL    m_bIncSubFolder;
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CSelectFolderDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:

    // Generated message map functions
    //{{AFX_MSG(CSelectFolderDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnBtnBowrse();
    virtual void OnOK();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTFOLDERDLG_H__C765D8D9_1957_4EE9_8C70_98C18A226657__INCLUDED_)
