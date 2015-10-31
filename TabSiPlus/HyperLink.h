// HyperLink.h : header file

#if !defined(AFX_HYPERLINK_H__D1625061_574B_11D1_ABBA_00A0243D1382__INCLUDED_)
#define AFX_HYPERLINK_H__D1625061_574B_11D1_ABBA_00A0243D1382__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// CHyperLink window

class CHyperLink : public CStatic
{
// Construction/destruction
public:
  CHyperLink();
  virtual ~CHyperLink();

// Attributes
public:

// Operations
public:

  void SetURL(LPCTSTR lpszURL);
  void SetColours(COLORREF crLinkColour,COLORREF crHoverColour);
  void SetLinkCursor(HINSTANCE hResInst, UINT uResID);
  static HINSTANCE GotoURL(LPCTSTR url, int showcmd = SW_SHOW);
  static LONG GetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata);

// Overrides
  // ClassWizard generated virtual function overrides
  //{{AFX_VIRTUAL(CHyperLink)
  protected:
  virtual void PreSubclassWindow();
  //}}AFX_VIRTUAL

// Implementation
protected:

// Protected attributes
protected:
  COLORREF m_crLinkColour;     // Hyperlink colours
  COLORREF m_crHoverColour;                       // Hover colour
  bool     m_bOverControl;                        // cursor over control?
  CString  m_strURL;                              // hyperlink URL
  CFont    m_Font;                                // Underline font if necessary
  HCURSOR  m_hLinkCursor;                         // Cursor for hyperlink

  // Generated message map functions
protected:
  //{{AFX_MSG(CHyperLink)
  afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
  afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnPaint();
    //}}AFX_MSG
  afx_msg void OnClicked();
  DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HYPERLINK_H__D1625061_574B_11D1_ABBA_00A0243D1382__INCLUDED_)
