#if !defined(AFX_TABCHECKLISTBOX_H__9CF9D841_76B6_11D1_A0EF_006097B1745C__INCLUDED_)
#define AFX_TABCHECKLISTBOX_H__9CF9D841_76B6_11D1_A0EF_006097B1745C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TabCheckListBox.h : header file
// by Qing Zhang (qzhang7@tfn.net)
// If you like this code, just don't remove my name from the source.
/*********************************
*    修改了DrawItem的部分代码，使文字在不同状态下显示不同颜色
*    添加了属性设置函数
*    modify by orbit (inte2000@163.com)
*********************************/
/////////////////////////////////////////////////////////////////////////////
// CTabCheckListBox window

class CTabCheckListBox : public CCheckListBox
{
// Construction
public:
    CTabCheckListBox();

// Attributes
public:

protected:
    LPINT m_lpnEachStop;
    int m_nTabStops;
    COLORREF m_clrChkText,m_clrText,m_clrDisableText;
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTabCheckListBox)
    public:
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    //}}AFX_VIRTUAL

// Implementation
public:
    void SetTabStops();
    BOOL SetTabStops( const int& cxEachStop );
    BOOL SetTabStops( int nTabStops, LPINT rgTabStops );
    void SetChkTextColor(COLORREF clrChkText) {m_clrChkText = clrChkText;}
    void SetTextColor(COLORREF clrText) {m_clrText = clrText;}
    void SetdisableTextColor(COLORREF clrDisableText) {m_clrDisableText = clrDisableText;}
    COLORREF GetChkTextColor() {return m_clrChkText;}
    COLORREF GetTextColor() {return m_clrText;}
    COLORREF GetdisableTextColor() {return m_clrDisableText;}

    virtual ~CTabCheckListBox();

protected:
    int GetAverageCharWidths();
    // Generated message map functions
protected:
    //{{AFX_MSG(CTabCheckListBox)
        // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABCHECKLISTBOX_H__9CF9D841_76B6_11D1_A0EF_006097B1745C__INCLUDED_)
