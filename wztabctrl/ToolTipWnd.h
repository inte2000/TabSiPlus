#if !defined(AFX_TOOLTIPWND_H__0D900373_E1FE_48BE_BDBE_6851CF9A838D__INCLUDED_)
#define AFX_TOOLTIPWND_H__0D900373_E1FE_48BE_BDBE_6851CF9A838D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolTipWnd.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CToolTipWnd window

class CToolTipWnd : public CWnd
{
// Construction
public:
	CToolTipWnd();
	CToolTipWnd(CWnd *pParentWnd);

// Attributes
public:
	CWnd *m_pParentWnd;
	static TCHAR m_szClassName[16];
	void SetTipTextColor(COLORREF crText);
	COLORREF GetTipTextColor();
	void SetTipBkgndColor(COLORREF crBkgnd);
	COLORREF GetTipBkgndColor();

// Operations
public:
	BOOL PopupTipWnd(LPPOINT ptCur, LPCTSTR lpszText);
	BOOL HideTipWnd();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolTipWnd)
	public:
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CToolTipWnd();
protected:
	COLORREF m_crBackGnd;
	COLORREF m_crText;
	UINT m_uTimerID;
	CString m_strCurTipText;
	// Generated message map functions
protected:
	BOOL Create(CWnd *pParentWnd);
	void DrawTipWnd(CDC *pDC,const RECT& rcClient);
	//{{AFX_MSG(CToolTipWnd)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


inline void CToolTipWnd::SetTipTextColor(COLORREF crText)
{ m_crText = crText; }

inline COLORREF CToolTipWnd::GetTipTextColor()
{ return m_crText; }

inline void CToolTipWnd::SetTipBkgndColor(COLORREF crBkgnd)
{ m_crBackGnd = crBkgnd; }

inline COLORREF CToolTipWnd::GetTipBkgndColor()
{ m_crBackGnd; }

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLTIPWND_H__0D900373_E1FE_48BE_BDBE_6851CF9A838D__INCLUDED_)
