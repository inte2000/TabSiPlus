#include "stdafx.h"
#include "WndHelpFunc.h"

UINT GetStyle(HWND hWnd)
{ 
    ASSERT(::IsWindow(hWnd)); 
    return ::GetWindowLong(hWnd,GWL_STYLE);
}

UINT GetExStyle(HWND hWnd)
{ 
    ASSERT(::IsWindow(hWnd)); 
    return ::GetWindowLong(hWnd,GWL_EXSTYLE);
}

void SwapLeftRight(LPRECT lpRect)
{ 
    LONG temp = lpRect->left; 
    lpRect->left = lpRect->right; 
    lpRect->right = temp; 
}

//WINBUG: some mirroring stuff will be in winuser.h someday
#ifndef WS_EX_LAYOUTRTL
#define WS_EX_LAYOUTRTL                    0x00400000L
#endif

void ScreenToClient(HWND hWnd, LPRECT lpRect)
{
    ASSERT(::IsWindow(hWnd));
    ::ScreenToClient(hWnd, (LPPOINT)lpRect);
    ::ScreenToClient(hWnd, ((LPPOINT)lpRect)+1);
    if (GetExStyle(hWnd) & WS_EX_LAYOUTRTL)
        SwapLeftRight(lpRect);
}

void ClientToScreen(HWND hWnd, LPRECT lpRect)
{
    ASSERT(::IsWindow(hWnd));
    ::ClientToScreen(hWnd, (LPPOINT)lpRect);
    ::ClientToScreen(hWnd, ((LPPOINT)lpRect)+1);
    if (GetExStyle(hWnd) & WS_EX_LAYOUTRTL)
        SwapLeftRight(lpRect);
}

void MoveWindow(HWND hWnd, LPRECT lpRect, BOOL bRepaint)
{
  ::MoveWindow(hWnd,lpRect->left,lpRect->top,lpRect->right - lpRect->left,lpRect->bottom - lpRect->top,bRepaint);
}

void DeflateRect(LPRECT lpRect, int x, int y)
{ 
    ::InflateRect(lpRect, -x, -y); 
}

void DeflateRect(LPRECT lpRect, SIZE size)
{ 
    ::InflateRect(lpRect, -size.cx, -size.cy); 
}

void DeflateRect(LPRECT lpSrc, LPCRECT lpRect)
{
    lpSrc->left += lpRect->left;
    lpSrc->top += lpRect->top;
    lpSrc->right -= lpRect->right;
    lpSrc->bottom -= lpRect->bottom;
}

void DeflateRect(LPRECT lpSrc, int l, int t, int r, int b)
{
    lpSrc->left += l;
    lpSrc->top += t;
    lpSrc->right -= r;
    lpSrc->bottom -= b;
}
