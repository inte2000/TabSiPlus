#if !defined(AFX_ABOUTDIALOG_H__2BE1452E_A464_44BA_8A9B_BC5B00F47CDA__INCLUDED_)
#define AFX_ABOUTDIALOG_H__2BE1452E_A464_44BA_8A9B_BC5B00F47CDA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AboutDialog.h : header file
//
#include "HyperLink.h"
/////////////////////////////////////////////////////////////////////////////
// CAboutDialog dialog

class CAboutDialog : public CDialog
{
// Construction
public:
    CAboutDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
    //{{AFX_DATA(CAboutDialog)
    enum { IDD = IDD_DLG_ABOUT };
    CHyperLink    m_stcHome;
        // NOTE: the ClassWizard will add data members here
    //}}AFX_DATA


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDialog)
    public:
    virtual int DoModal();
    virtual BOOL DestroyWindow();
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CAboutDialog)
    virtual BOOL OnInitDialog();
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUTDIALOG_H__2BE1452E_A464_44BA_8A9B_BC5B00F47CDA__INCLUDED_)
