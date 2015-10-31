#if !defined(AFX_WZTABCTRL_H__8AB1BCD2_3210_4FA5_A26B_6B473FA9CF1E__INCLUDED_)
#define AFX_WZTABCTRL_H__8AB1BCD2_3210_4FA5_A26B_6B473FA9CF1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WzTabCtrl.h : header file
//

#include <afxtempl.h>
#include "WzTabGlobal.h"
#include "WzTabItem.h"
#include "WzTabTheme.h"

#define WZTAB_EVENT_NOTIFY          WM_USER + 0x102

#define EVENT_GROUP_MODE            1 // 参数是分组模式和改模式下TAB的个数
#define EVENT_ITEM_DELETE           2
#define EVENT_ITEM_L_CLICK          3
#define EVENT_ITEM_L_DBCLICK        4
#define EVENT_ITEM_R_CLICK          5
#define EVENT_TIP_NEED              6
#define EVENT_ROW_COUNT_CHANGE      7
#define EVENT_ITEM_STATE_CHANGE     8

typedef struct tagEventPara
{
    ULONG para1;
    ULONG para2;
}WZTAB_EVENT_PARA;

/////////////////////////////////////////////////////////////////////////////
// CWzTabCtrl window

class CWzTabCtrl  : public CWnd
{
// Construction
public:
    CWzTabCtrl();
    virtual ~CWzTabCtrl();

    BOOL CreateTabCtrl(CWnd *pParent);
    CSize ReCalcAllTabsPosAndWndSize(int nWndCx);

// Attributes
public:
    void SetNotifyWnd(CWnd *pWnd) { m_pNotifyWnd = pWnd; };
    //void SetTabCtrl
    BOOL SendEventNotify(UINT EventCode, ULONG Param1, ULONG Param2);
    void SetGroupSameName(BOOL bGroup);
    BOOL GetGroupSameName() { return m_bGroupSameName; }
    void SetShowItemIcon(BOOL bShow);
    BOOL GetShowItemIcon() { return m_bShowItemIcon; }
    void SetShowCloseBtn(BOOL bShow);
    BOOL GetShowCloseBtn() { return m_bShowCloseBtn; }
    CWzTabTheme *SetTabTheme(CWzTabTheme *theme, BOOL bUpdate);

// Operations
public:
    LONG GetCurSel();
    BOOL SetCurSel(LONG nItemId);
/*
    BOOL SetAllItemSize(const CSize* size);
    BOOL SetItemSize(int nItem, const CSize& size);
    BOOL SetItemSize(int nItem, const int width, const int height);
*/
    BOOL GetItemSize(LONG nItem, CSize& size);
    BOOL GetItemRect(LONG nItem, CRect& rect);
    LPARAM GetItemData(LONG nItem);
    BOOL SetItemData(LONG nItem, DWORD dwData);

    int GetRowCount() { return m_nRowCount; };

    int GetItemCount();
    int GetGroupCount();
    int GetSubItemCount(int groupIdx);
    LPARAM GetItemDataByIndex(int groupIdx, int subIdx);
    LONG GetItemIdByIndex(int groupIdx, int subIdx);
    BOOL SetItemTextByIndex(int groupIdx, int subIdx, LPCTSTR lpszItem);

    LONG InsertItem(LONG nItemId, LPCTSTR lpszItem, int nIcon, LPARAM para);//返回item index
    BOOL SetItemText(LONG nItemId, LPCTSTR lpszItem);
    BOOL DeleteItem(LONG nItemId);
    BOOL DeleteAllItems( );
    BOOL DeleteGroup(int nGroupIdx);
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CWzTabCtrl)
    public:
    virtual BOOL DestroyWindow();
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    protected:
    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
    virtual void PreSubclassWindow();
    virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
    //}}AFX_VIRTUAL

// Implementation
protected:
    BOOL IsValidTabIndex(int nTabIdx) const;
    LONG GetCurrentActiveSubItemId(int TabItemIdx);
    int GetCurrentActiveSubItemIndex(int TabItemIdx);

    void SetActiveSubItemIndex(int TabItemIdx, int SubItemIdx);
    void ResetActiveSubItemIndex(int TabItemIdx, int SubItemIdx);

    BOOL GetTooltipText(LONG uItemId, CString& tooltip);
    BOOL InternalSetItemText(int groupIdx, int subIdx, LPCTSTR lpszItem);
    LONG InternalInsertItem(int nItemIdx, LPCTSTR lpszItem, LPCTSTR lpszSubItem, int nIcon, LPARAM para);
    void InternalDeleteItem(int TabItemIdx, int SubItemIdx);
    BOOL InternalHitTest(const CPoint& point, int& TabIdx, int& SubIdx) const;
    LONG GetSubItemId(int TabItemIdx, int SubItemIdx) const;
    int GetFirstInactiveSubItemIndex(int TabItemIdx);
    BOOL HighlightItem(int TabItemIdx, int SubItemIdx);
    BOOL HighlightItemWithoutNotify(int TabItemIdx, int SubItemIdx);
    BOOL FindItemByItemId(LONG uItemId, int& TabIdx, int& SubIdx);
    BOOL DeleteTabGroup(int TabIdx);
    void FreeAllItems();
    CTabItem *FindSameNameItem(LPCTSTR lpszName);
    int FindSameNameItemIndex(LPCTSTR lpszName);
    int ReGroupAllItems(BOOL bGroup);
    BOOL CombineItem(CTabItem *pItemDest, CTabItem *pItemSrc);
    
    CSize InternalCalcPos(int nWndCx, CDC *pDC);//返回windows的高度和rowcount
    CSize InternalReCalcAllTabsPos(int nWidth = 0);
    void DrawTabItem(CDC *pDC, CTabItem *pItem, int nItem);
    TAB_ITEM_STATE GetTabItemState(int nItem);

    void CheckRowCountChange();
protected:
    CWzTabTheme *m_pTheme;
    CWnd *m_pNotifyWnd;
    BOOL m_bGroupSameName;
    BOOL m_bShowCloseBtn;
    BOOL m_bShowItemIcon;
    CArray<CTabItem*, CTabItem*> m_arItems;
    int m_nCurActive,m_nLastActive,m_nHover,m_nLastHover;

    BOOL m_bTabPosChange;
    int m_nWndWidth;
    int m_nRowCount;

    LONG /*volatile*/ m_lCurAvailItemId;
    // Generated message map functions
protected:
    //{{AFX_MSG(CWzTabCtrl)
    afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
    afx_msg void OnDestroy();
    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
    afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
    afx_msg BOOL OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WZTABCTRL_H__8AB1BCD2_3210_4FA5_A26B_6B473FA9CF1E__INCLUDED_)
