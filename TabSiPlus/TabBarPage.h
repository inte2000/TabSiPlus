/*********************************************************************
*    Author         : orbit
*    Date           : November 15, 2000
*    Last-modified  : June 4, 2002 
*    Contact us     : inte2000@163.com
**********************************************************************/
#if !defined(AFX_TABBARPAGE_H__9081772C_2F08_42BD_AE7E_A2310DFC6EB4__INCLUDED_)
#define AFX_TABBARPAGE_H__9081772C_2F08_42BD_AE7E_A2310DFC6EB4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabBarPage.h : header file
//

typedef struct tagThemeInfo
{
    CString name;
    CString fileName;
    CString fullPathName;
}ThemeInfo;
/////////////////////////////////////////////////////////////////////////////
// CTabBarPage dialog

//##ModelId=431C34C802CE
class CTabBarPage : public CPropertyPage
{
    DECLARE_DYNCREATE(CTabBarPage)

// Construction
public:
    //##ModelId=431C34C802EE
    CTabBarPage();
    //##ModelId=431C34C802EF
    ~CTabBarPage();

// Dialog Data
    //{{AFX_DATA(CTabBarPage)
    enum { IDD = IDD_PROPPAGE_TABBAR };
    BOOL    m_bUseShellMenu;
    BOOL    m_bShowIcon;
    int        m_nAddMode;
    int        m_nDockPos;
    BOOL    m_bUseCustomColor;
    BOOL    m_bSmartLoc;
    UINT    m_uSrhDeep;
    BOOL    m_bGroupSameName;
    CComboBox m_cmb_Theme;
    //}}AFX_DATA

    std::vector<ThemeInfo> m_themeInfo;
// Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CTabBarPage)
    public:
    //##ModelId=431C34C8034B
    virtual BOOL OnApply();
    protected:
    //##ModelId=431C34C8034D
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

// Implementation
protected:
    void ProcessThemeFile(LPCTSTR lpFullName, LPCTSTR lpFileName);
    void FillThemeCombobox();
    // Generated message map functions
    //{{AFX_MSG(CTabBarPage)
    virtual BOOL OnInitDialog();
    afx_msg void OnChkShellmenu();
    afx_msg void OnChkShowicon();
    afx_msg void OnChangeEditMaxwnd();
    afx_msg void OnRadAddb();
    afx_msg void OnRadAddf();
    afx_msg void OnRadBottom();
    afx_msg void OnRadTop();
    afx_msg void OnChkSmartLoc();
    afx_msg void OnBnClickedChkGroupSame();
    afx_msg void OnCbnSelchangeCmbTheme();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
public:
    BOOL m_bShowCloseBtn;
    afx_msg void OnBnClickedChkShowClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBARPAGE_H__9081772C_2F08_42BD_AE7E_A2310DFC6EB4__INCLUDED_)
