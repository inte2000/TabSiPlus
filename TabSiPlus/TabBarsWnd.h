#if !defined(AFX_TABBARSWND_H__B581F81B_AD07_4296_A80B_055BF0B1166C__INCLUDED_)
#define AFX_TABBARSWND_H__B581F81B_AD07_4296_A80B_055BF0B1166C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TabBarsWnd.h : header file
//
#include "WzTabCtrl.h"
#include "WzTabGradientTheme.h"
#include "SiWindow.h"
#include "OwnerDrawMenuMgmt.h"
#include "Globals.h"
#include "MdiChildManagment.h"


#define        TABBAR_WINDOW_TIMERID                    0x201

/////////////////////////////////////////////////////////////////////////////
// CTabBarsWnd window

class CTabBarsWnd : public CWnd
{
// Construction
public:
    CTabBarsWnd();
    virtual ~CTabBarsWnd();

// Attributes
public:
    CWzTabCtrl m_TabCtrl;
    CWzTabGraidentTheme *m_pTabTheme;

    CToolBar      m_wndToolBar;
    COwnerDrawMenuMgmt m_ActiveMenus;

    void MoveTabCtrl(LPRECT pTabClient);
    void MoveToolbarCtrl(LPRECT pToolbar);
    int  CalcTabbarWndHeight(LPRECT pMdiPos);
    void MdiClientPosChanging(LPRECT pTabsRect);
    BOOL IsPositionChanged(LPRECT pTabsRect);

    void UpdateTabs(BOOL bForce);
    LONG FindTabItemId(HWND hWnd);
    void ActivateTabItem(HWND hWnd);
    int AddTabItem(HWND hWnd);
    void ChangeTabItemText(HWND hWnd, LPCTSTR lpszTitle);
    void ActivateWindow(CSiWindow*& pWindow, BOOL bActive);
    void CloseSIWindow(CSiWindow*& pWindow);


// Operations
public:
    void OnSiWindowTitleChanged(HWND hWnd, LPCTSTR lpszTitle);
    void OnSiWindowActivated(HWND hActived, HWND hDeactived);
    void OnMdiWindowActivated(HWND hActived);
    void OnWindowClosing(HWND hWnd);
    void OnWindowOpened(HWND hWnd);


// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTabBarsWnd)
    public:
    virtual BOOL DestroyWindow();
    protected:
    virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
    //}}AFX_VIRTUAL

// Implementation
protected:
    void CreateTabWndCtrl();
    void CalcPosition(const CRect& rcClient, CRect& rcToolbarPos, CRect& rcTabPos);
    int InternalAddTabItem(ChildObject *pObj);
    BOOL FlipFile(LPCTSTR lpszFileName, LPCTSTR lpszSiProgram);

    void OnWaTabItemDelete(WZTAB_EVENT_PARA *pPara);
    void OnWaTabItemLDbClick(WZTAB_EVENT_PARA *pPara);
    void OnWaTabItemRClick(WZTAB_EVENT_PARA *pPara);
    void OnWaTabRowCountChange(WZTAB_EVENT_PARA *pPara);
    void OnWaTabItemStateChange(WZTAB_EVENT_PARA *pPara);

    // Generated message map functions
protected:
    //{{AFX_MSG(CTabBarsWnd)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnPaint();
    afx_msg void OnDestroy();
    afx_msg void OnToolbarDropDown(NMHDR* pnmhdr, LRESULT* plRes);
    afx_msg void OnToolbarHeadFlip();
    afx_msg void OnMainHeadFlip();
    afx_msg void OnMainOption();
    afx_msg void OnMainHelp();
    afx_msg void OnMainSupport();
    afx_msg void OnMainShutdown();
    afx_msg void OnMainAbout();
    afx_msg void OnRbtSelect();
    afx_msg void OnRbtClose();
    afx_msg void OnRbtCloseAllExcept();
    afx_msg void OnRbtCloseAll();
    afx_msg void OnRbtHeaderFlip();
    afx_msg void OnSetreadonly();
    afx_msg void OnProperity();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    //}}AFX_MSG
    afx_msg void OnToolTipTabCtrl  (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnToolTipTabCtrlW (NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg LRESULT OnOptionTabWndPosChange(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnOptionThemeChange(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnOptionShowIcon(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnOptionGroupSame(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnOptionShowClose(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnWzTabEventNotify(WPARAM wParam, LPARAM lParam);
DECLARE_MESSAGE_MAP();

    HIMAGELIST    m_TabImages;
    CImageList    m_TabImgList;
    HIMAGELIST    m_ToolbarImages;
    HIMAGELIST    m_ToolbarImagesS;

    HWND          m_hLastActive;
    int m_nCurRtbTab;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABBARSWND_H__B581F81B_AD07_4296_A80B_055BF0B1166C__INCLUDED_)
