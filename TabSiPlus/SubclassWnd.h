// SubclassWnd.h : header file
//

#if !defined(AFX_SUBCLASSWND_H__BE917122_5C03_11D3_BA1A_000000000000__INCLUDED_)
#define AFX_SUBCLASSWND_H__BE917122_5C03_11D3_BA1A_000000000000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WndHelpFunc.h"
#include "SyncObject.h"

/////////////////////////////////////////////////////////////////////////////
// CSubclassWnd window

class CSubclassWnd
{
// Construction
public:
    CSubclassWnd();
    virtual ~CSubclassWnd();

    static CMap<HWND, HWND, CSubclassWnd*, CSubclassWnd*> m_SubclassWndMap;
    static CSleepEx   *m_CS;
    static bool       m_bCSinit;
    void Attach(HWND hWnd);
    HWND Detach();

public:            
    HWND GetSafeHwnd() const;
    void Invalidate(BOOL bErase = TRUE);
    UINT GetStyle();
    UINT GetExStyle() const;
    LRESULT SendMessage(UINT Msg,WPARAM wParam = 0,LPARAM lParam = 0);
    HWND GetParent() const;
    BOOL IsIconic() const;
    BOOL IsZoomed() const;
    BOOL ShowWindow(int nCmdShow);
    void GetWindowRect(LPRECT lpRect) const;
    void GetClientRect(LPRECT lpRect) const;
    void ClientToScreen(LPPOINT lpPoint) const;
    void ScreenToClient(LPPOINT lpPoint) const;
    HWND GetTopWindow() const;
    void GetWindowText(LPTSTR lpString, UINT nMaxCount) const;

    virtual void PreSubclassWindow();
    virtual void PostNcDestroy();
    // New window procedure (must be implemented by derived classes)
    virtual LRESULT WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    LRESULT CallDefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
protected:
    void Subclass(HWND hWnd);
    WNDPROC Unsubclass();

protected:
    bool m_bIsSubclassed;
    HWND m_hWnd;                   // Handle of the window            
    HWND m_hWndAfter;              // Handle of the window after detach            
    WNDPROC m_oldWndProc;                 // Address of original window procedure
};

inline HWND CSubclassWnd::GetSafeHwnd() const
{ return this == NULL ? NULL : m_hWnd; }

inline void CSubclassWnd::Invalidate(BOOL bErase)
{ ASSERT(::IsWindow(m_hWnd)); ::InvalidateRect(m_hWnd, NULL, bErase); }

inline UINT CSubclassWnd::GetStyle()
{ ASSERT(::IsWindow(m_hWnd)); return ::GetWindowLong(m_hWnd,GWL_STYLE);}

inline UINT CSubclassWnd::GetExStyle() const
{ ASSERT(::IsWindow(m_hWnd)); return ::GetWindowLong(m_hWnd,GWL_EXSTYLE);}

inline LRESULT CSubclassWnd::SendMessage(UINT Msg,WPARAM wParam,LPARAM lParam)
{ ASSERT(::IsWindow(m_hWnd)); return ::SendMessage(m_hWnd,Msg,wParam,lParam);}

inline HWND CSubclassWnd::GetParent() const
{ ASSERT(::IsWindow(m_hWnd)); return ::GetParent(m_hWnd); }

inline BOOL CSubclassWnd::IsIconic() const
{ ASSERT(::IsWindow(m_hWnd)); return ::IsIconic(m_hWnd); }

inline BOOL CSubclassWnd::IsZoomed() const
{ ASSERT(::IsWindow(m_hWnd)); return ::IsZoomed(m_hWnd); }

inline BOOL CSubclassWnd::ShowWindow(int nCmdShow)
{ ASSERT(::IsWindow(m_hWnd)); return ::ShowWindow(m_hWnd, nCmdShow); }

inline void CSubclassWnd::GetWindowRect(LPRECT lpRect) const
{ ASSERT(::IsWindow(m_hWnd)); ::GetWindowRect(m_hWnd, lpRect); }

inline void CSubclassWnd::GetClientRect(LPRECT lpRect) const
{ ASSERT(::IsWindow(m_hWnd)); ::GetClientRect(m_hWnd, lpRect); }

inline void CSubclassWnd::ClientToScreen(LPPOINT lpPoint) const
{ ASSERT(::IsWindow(m_hWnd)); ::ClientToScreen(m_hWnd, lpPoint); }

inline void CSubclassWnd::ScreenToClient(LPPOINT lpPoint) const
{ ASSERT(::IsWindow(m_hWnd)); ::ScreenToClient(m_hWnd, lpPoint); }

inline HWND CSubclassWnd::GetTopWindow() const
{ ASSERT(::IsWindow(m_hWnd)); return ::GetTopWindow(m_hWnd); }

inline void CSubclassWnd::GetWindowText(LPTSTR lpString, UINT nMaxCount) const
{ ASSERT(::IsWindow(m_hWnd)); ::GetWindowText(m_hWnd, lpString, nMaxCount); }

#endif // !defined(AFX_SUBCLASSWND_H__BE917122_5C03_11D3_BA1A_000000000000__INCLUDED_)
