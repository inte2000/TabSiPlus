/*********************************************************************
*    Author         : orbit
*    Date           : November 15, 2000
*    Last-modified  : June 4, 2002 
*    Contact us     : inte2000@163.com
**********************************************************************/
#if !defined(AFX_FLIPPAGE_H__3BCECAFC_D7C6_482C_98AB_74E989A0EC90__INCLUDED_)
#define AFX_FLIPPAGE_H__3BCECAFC_D7C6_482C_98AB_74E989A0EC90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FlipPage.h : header file
//
#include "TabCheckListBox.h"
/////////////////////////////////////////////////////////////////////////////
// CFlipPage dialog

//##ModelId=431C34CB00FA
class CFlipPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CFlipPage)

// Construction
public:
    //##ModelId=431C34CB010A
    CFlipPage();
    //##ModelId=431C34CB010B
    ~CFlipPage();

// Dialog Data
    //{{AFX_DATA(CFlipPage)
    enum { IDD = IDD_PROPPAGE_FLIP };
    //##ModelId=431C34CB0129
    CTabCheckListBox    m_lstPath;
    //}}AFX_DATA
    CButton    m_btnAdd,m_btnDel,m_btnUp,m_btnDown;

// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CFlipPage)
    public:
    //##ModelId=431C34CB012D
    virtual BOOL OnApply();
    protected:
    //##ModelId=431C34CB012F
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
private:
    //##ModelId=431C34CB013A
    TCHAR m_szSelect[64];
    //##ModelId=431C34CB0149
    CStringArray m_strAPath;
// Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CFlipPage)
    //##ModelId=431C34CB0157
    virtual BOOL OnInitDialog();
    //##ModelId=431C34CB0168
    afx_msg void OnItemchangedListFlppath(NMHDR* pNMHDR, LRESULT* pResult);
    //##ModelId=431C34CB0186
    afx_msg void OnItemclickListFlppath(NMHDR* pNMHDR, LRESULT* pResult);
    //##ModelId=431C34CB0197
    afx_msg void OnColumnclickListFlppath(NMHDR* pNMHDR, LRESULT* pResult);
    //##ModelId=431C34CB01B5
    afx_msg void OnTrackListFlppath(NMHDR* pNMHDR, LRESULT* pResult);
    //##ModelId=431C34CB01D4
    afx_msg void OnBtnAddflppath();
    //##ModelId=431C34CB01D6
    afx_msg void OnBtnDelflppath();
    //##ModelId=431C34CB01E4
    afx_msg void OnBtnUpflppath();
    //##ModelId=431C34CB01F5
    afx_msg void OnBtnDownflppath();
    //##ModelId=431C34CB0203
    afx_msg void OnSelchangeListPath();
    //##ModelId=431C34CB0205
    afx_msg void OnChangeEditHeadfile();
    //##ModelId=431C34CB0213
    afx_msg void OnChangeEditCppfile();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLIPPAGE_H__3BCECAFC_D7C6_482C_98AB_74E989A0EC90__INCLUDED_)
