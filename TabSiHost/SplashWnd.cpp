//////////////////////////////////////////////////////////////
// File:        // SplashWnd.cpp
// File time:   // 21.04.2005    11:15
// Version:     // 1.0
// Author:      // Sergey Kolomenkin <register@tut.by>
// Copyright:   // (c) Sergey Kolomenkin, Minsk, 2005
// Description: //////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// Version:     // 1.1
// Time:        // 21.11.2007   
// Author:      // Simon Wang <inte2000@163.com>
// Description: // According to the need to adjust some of the code, add some function

#include "stdafx.h"
#include "SplashWnd.h"
#include "Resource.h"
#include "../Common/Localization.h"
#include "../Common/SourceInsight.h"



static int g_shadowThick = 4;
static int g_shadowMargin = 4;
static BOOL g_isDrawShadow    = FALSE;
static HBITMAP splashBmpHandle = NULL;

DWORD g_splashWndTimeToLive    = 0;
UINT g_uTimerID = 0;

HFONT MakeFont( int CharWidth, int CharHeight )
{
    HFONT hf = ::CreateFont( CharHeight, CharWidth, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, ANSI_CHARSET, 
        OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH,
        NULL);
    return hf;
}

// Following function was cut from MFC sources and adapted.
void CenterWindow( HWND m_hWnd )
{
    ASSERT(::IsWindow(m_hWnd));

    // determine owner window to center against
    DWORD dwStyle = ::GetWindowLong( m_hWnd, GWL_STYLE );
    HWND hWndCenter = NULL;

    if (dwStyle & WS_CHILD)
        hWndCenter = ::GetParent(m_hWnd);
    else
        hWndCenter = ::GetWindow(m_hWnd, GW_OWNER);

    // get coordinates of the window relative to its parent
    RECT rcDlg;
    ::GetWindowRect( m_hWnd, &rcDlg );
    RECT rcArea;
    RECT rcCenter;
    HWND hWndParent;
    if (!(dwStyle & WS_CHILD))
    {
        // don't center against invisible or minimized windows
        if (hWndCenter != NULL)
        {
            DWORD dwStyle = ::GetWindowLong(hWndCenter, GWL_STYLE);
            if (!(dwStyle & WS_VISIBLE) || (dwStyle & WS_MINIMIZE))
                hWndCenter = NULL;
        }

        MONITORINFO mi;
        mi.cbSize = sizeof(mi);

        // center within appropriate monitor coordinates
        if (hWndCenter == NULL)
        {
            HWND hwDefault = m_hWnd;
            ::GetMonitorInfo(MonitorFromWindow(hwDefault, MONITOR_DEFAULTTOPRIMARY), &mi);
            rcCenter = mi.rcWork;
            rcArea = mi.rcWork;
        }
        else
        {
            ::GetWindowRect(hWndCenter, &rcCenter);
            ::GetMonitorInfo(::MonitorFromWindow(hWndCenter, MONITOR_DEFAULTTONEAREST), &mi);
            rcArea = mi.rcWork;
        }
    }
    else
    {
        // center within parent client coordinates
        hWndParent = ::GetParent(m_hWnd);
        ASSERT(::IsWindow(hWndParent));

        ::GetClientRect(hWndParent, &rcArea);
        ASSERT(::IsWindow(hWndCenter));
        ::GetClientRect(hWndCenter, &rcCenter);
        ::MapWindowPoints(hWndCenter, hWndParent, (POINT*)&rcCenter, 2);
    }

    int rcDlg_Width = rcDlg.right - rcDlg.left;
    int rcDlg_Height = rcDlg.bottom - rcDlg.top;

    // find dialog's upper left based on rcCenter
    int xLeft = (rcCenter.left + rcCenter.right) / 2 - rcDlg_Width / 2;
    int yTop = (rcCenter.top + rcCenter.bottom) / 2 - rcDlg_Height / 2;

    // if the dialog is outside the screen, move it inside
    if (xLeft < rcArea.left)
        xLeft = rcArea.left;
    else if (xLeft + rcDlg_Width > rcArea.right)
        xLeft = rcArea.right - rcDlg_Width;

    if (yTop < rcArea.top)
        yTop = rcArea.top;
    else if (yTop + rcDlg_Height > rcArea.bottom)
        yTop = rcArea.bottom - rcDlg_Height;

    // map screen coordinates to child coordinates
    ::SetWindowPos( m_hWnd, NULL, xLeft, yTop, -1, -1, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
}

void DrawSplashBitmap(HDC hDC, RECT& rc, HBITMAP hBmp, LPCTSTR szVer, LPCTSTR szWeb)
{
    BITMAP bm;
    ::GetObject(hBmp, sizeof(BITMAP), &bm);

    HDC hMemDC = ::CreateCompatibleDC(hDC);
    if(hMemDC == NULL)
    {
        return;
    }

    HBITMAP hOldBmp = (HBITMAP)::SelectObject(hMemDC, hBmp);
    ::SetBkMode(hMemDC, TRANSPARENT);
    ::SetTextColor(hMemDC, RGB(200,128,255));
    ::TextOut(hMemDC, 2, 2, szVer, lstrlen(szVer));
    ::SetTextColor(hMemDC, RGB(0,0,255));
    ::TextOut(hMemDC, 90, 160, szWeb, lstrlen(szWeb));

    int nBlock = bm.bmHeight / 10;
    for(int i = 0; i < nBlock; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            int mm = j * nBlock + i;
            ::StretchBlt(hDC, rc.left + 1, rc.top + 1 + mm, bm.bmWidth,
                         1, hMemDC, 0, mm, bm.bmWidth, 1, SRCCOPY);
        }
        ::Sleep(20);
    }
    ::BitBlt(hDC, rc.left + 1, rc.top + 1, bm.bmWidth, bm.bmHeight, hMemDC, 0, 0, SRCCOPY);

    ::SelectObject(hMemDC, hOldBmp);
    ::DeleteDC(hMemDC);
}

void OnPaint(HWND hWnd)
{
    TCHAR szSoftVer[128], szWebUrl[128];
    DWORD dwBufSize = SIZEOF_ARRAY(szSoftVer);
    LocLoadString(IDS_SOFTVERNAME, szSoftVer, &dwBufSize );
    dwBufSize = SIZEOF_ARRAY(szWebUrl);
    LocLoadString(IDS_WEBURL, szWebUrl, &dwBufSize );

    PAINTSTRUCT ps;
    HDC hdc = ::BeginPaint(hWnd, &ps);

    RECT    rt;
    ::GetClientRect(hWnd, &rt);

    //HFONT MakeFont( int CharWidth, int CharHeight )
    DrawSplashBitmap(hdc, rt, splashBmpHandle, szSoftVer, szWebUrl);

    ::EndPaint(hWnd, &ps);
}

void DrawRectBorder(HDC hDC, int left, int top, int right, int bottom)
{
    HPEN hPen = ::CreatePen(PS_SOLID, 1, RGB(0,0,255));
    if(hPen != NULL)
    {
        HPEN hOldPen = (HPEN)::SelectObject(hDC, hPen);
        HBRUSH hOldBr = (HBRUSH)::SelectObject(hDC, ::GetStockObject(NULL_BRUSH));
        ::Rectangle(hDC, left, top, right, bottom);
        ::SelectObject(hDC, hOldBr);
        ::SelectObject(hDC, hOldPen);
        ::DeleteObject(hPen);
    }
}

void DrawShadow(HDC hDC, RECT& rcWnd, int margin, int thick)
{
    COLORREF sColor[4] = {RGB(64,64,64),RGB(96,96,96),RGB(128,128,128),RGB(160,160,160)};
    for(int i = 0; i < thick; i++)
    {
        HPEN hShadowPen = ::CreatePen(PS_SOLID, 1, sColor[i]);
        if(hShadowPen != NULL)
        {
            HPEN hOldPen = (HPEN)::SelectObject(hDC,hShadowPen);
            ::MoveToEx(hDC,rcWnd.left + margin,rcWnd.bottom - thick + i,NULL);
            ::LineTo(hDC,rcWnd.right - thick + i + 1,rcWnd.bottom - thick + i);
            ::MoveToEx(hDC,rcWnd.right - thick + i,rcWnd.top + margin,NULL);
            ::LineTo(hDC,rcWnd.right - thick + i,rcWnd.bottom - thick + i);
            ::SelectObject(hDC,hOldPen);
            ::DeleteObject(hShadowPen);
        }
    }
}

void OnEraseBkgnd(HWND hWnd, HDC hDC)
{
    RECT rcWnd;
    ::GetClientRect(hWnd,&rcWnd);

    if(g_isDrawShadow)
    {
        DrawRectBorder(hDC, rcWnd.left, rcWnd.top, 
                       rcWnd.right - g_shadowThick, rcWnd.bottom - g_shadowThick);

        DrawShadow(hDC, rcWnd, g_shadowMargin, g_shadowThick);
    }
    else
    {
        DrawRectBorder(hDC, rcWnd.left, rcWnd.top, rcWnd.right, rcWnd.bottom);
    }
}

BOOL CreateShadowRange(HWND hWnd)
{
    RECT    rt = { 0 };
    ::GetClientRect(hWnd, &rt);
    HRGN hWndRgn = ::CreateRectRgn(rt.left,rt.top,rt.right - g_shadowThick + 1,
                                   rt.bottom - g_shadowThick + 1);
    if(hWndRgn == NULL)
    {
        return FALSE;
    }
    HRGN hRgnBottom = ::CreateRectRgn(rt.left + g_shadowMargin,
                                      rt.bottom - g_shadowThick + 1, rt.right, rt.bottom);
    if(hRgnBottom != NULL)
    {
        HRGN hRgnRight = ::CreateRectRgn(rt.right - g_shadowThick + 1,
                                         rt.top + 1, rt.right, rt.bottom);
        if(hRgnRight != NULL)
        {
            ::CombineRgn(hWndRgn,hWndRgn,hRgnBottom,RGN_OR);
            ::CombineRgn(hWndRgn,hWndRgn,hRgnRight,RGN_OR);
            ::DeleteObject(hRgnRight);
        }
        ::DeleteObject(hRgnBottom);
    }
    
    //if hRgnBottom and hRgnRight failed, a full range will be used 
    ::SetWindowRgn(hWnd,hWndRgn,FALSE);

    return TRUE;
}

BOOL OnCreate(HWND hWnd)
{
    if(g_isDrawShadow)
    {
        if(!CreateShadowRange(hWnd))
        {
            return FALSE;
        }
    }
    
    g_uTimerID = ::SetTimer(hWnd, 0x01, g_splashWndTimeToLive, NULL);
    if(g_uTimerID == 0)
        return FALSE;

    CenterWindow(hWnd);
    ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));

    return TRUE;
}

void OnTimer(HWND hWnd, UINT uTimerID)
{
    if(uTimerID == g_uTimerID)
    {
        ::DestroyWindow(hWnd);
    }
}

void OnDestroy(HWND hWnd)
{
    if(g_uTimerID != 0)
    {
        ::KillTimer(hWnd,g_uTimerID);
    }
    if(splashBmpHandle != NULL)
    {
        ::DeleteObject(splashBmpHandle);
        splashBmpHandle = NULL;
    }
}

LRESULT CALLBACK SplashWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
    {
    case WM_CREATE:
        if(!OnCreate(hWnd))
            return -1;
        break;

    case WM_PAINT:
        OnPaint(hWnd);
        break;
    case WM_ERASEBKGND:
        OnEraseBkgnd(hWnd, (HDC)wParam);
        return 1;

    case WM_LBUTTONDOWN:
    case WM_MBUTTONDOWN:
    case WM_RBUTTONDOWN:
    case WM_KEYDOWN:
        ::DestroyWindow(hWnd);
        break;

    case WM_TIMER:
        OnTimer(hWnd, (UINT)wParam);
        break;

    case WM_DESTROY:
        OnDestroy(hWnd);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

void RegisterWndClass( LPCTSTR szWindowClass )
{
    WNDCLASSEX wcex;
    wcex.cbSize            = sizeof(wcex); 
    wcex.style            = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = (WNDPROC)SplashWndProc;
    wcex.cbClsExtra        = 0;
    wcex.cbWndExtra        = 0;
    wcex.hInstance        = NULL;
    wcex.hIcon            = NULL;
    wcex.hCursor        = NULL;
    wcex.hbrBackground    = NULL;
    wcex.lpszMenuName    = NULL;
    wcex.lpszClassName    = szWindowClass;
    wcex.hIconSm        = NULL;

    ::RegisterClassEx(&wcex);
}

BOOL CreateSplashScreen(HBITMAP hBkgnd, BOOL bShadow, DWORD milliseconds)
{
    BITMAP bm;
    if((hBkgnd == NULL) || (::GetObject(hBkgnd, sizeof(BITMAP), &bm) == 0))
    {
        return FALSE;
    }

    splashBmpHandle = hBkgnd;
    g_isDrawShadow = bShadow;
    g_splashWndTimeToLive = milliseconds;

    int nWndWidth = bm.bmWidth + 2; //Windows frame rect
    int nWndHeight = bm.bmHeight + 2;
    if(g_isDrawShadow)
    {
        nWndWidth += g_shadowThick;
        nWndHeight += g_shadowThick;
    }

    HWND hDesktopWnd = ::GetDesktopWindow();
    ::RegisterWndClass(splashWndClassName);
    HWND hWndUser = ::CreateWindowEx(WS_EX_TOPMOST, splashWndClassName,splashWndText, WS_VISIBLE|WS_POPUP,    
                                     0, 0, nWndWidth, nWndHeight, hDesktopWnd, NULL, NULL, NULL);

    if(hWndUser != NULL)
    {
        ::ShowWindow(hWndUser, SW_SHOW);
    }

    return (hWndUser != NULL);
}

//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
