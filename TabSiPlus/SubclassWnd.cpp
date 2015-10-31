// SubclassWnd.cpp : implementation file

#include "stdafx.h"
#include "SubclassWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMap<HWND, HWND, CSubclassWnd*, CSubclassWnd*> CSubclassWnd::m_SubclassWndMap;
CSleepEx   *CSubclassWnd::m_CS = NULL;
bool       CSubclassWnd::m_bCSinit = false;

static LRESULT CALLBACK SubclassWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT lr;
    CSubclassWnd *pSubClass = NULL;
    if(CSubclassWnd::m_SubclassWndMap.Lookup(hWnd, pSubClass) && pSubClass != NULL)
    {
        lr = pSubClass->WindowProc(uMsg, wParam, lParam);
    }
    else
    {
        lr = ::DefWindowProc(hWnd, uMsg, wParam, lParam);
    }

    return lr;
}

/////////////////////////////////////////////////////////////////////////////
// CSubclassWnd
CSubclassWnd::CSubclassWnd() : m_bIsSubclassed(false), m_hWnd(NULL)
{
    m_hWndAfter = NULL;
    m_oldWndProc = NULL;
    if(!CSubclassWnd::m_bCSinit)
    {
        CSubclassWnd::m_CS = new CSleepEx(TRUE);
        CSubclassWnd::m_bCSinit = true;
    }
}

CSubclassWnd::~CSubclassWnd()
{
    Unsubclass();
    if(CSubclassWnd::m_SubclassWndMap.GetSize() <= 0)
    {
        delete CSubclassWnd::m_CS;
        CSubclassWnd::m_CS = NULL;
        CSubclassWnd::m_bCSinit = false;
    }
}

void CSubclassWnd::Subclass(HWND hWnd)
{
    ASSERT(hWnd);      // Do you really want to subclass a window that has a NULL handle?
    ASSERT(!m_bIsSubclassed);
    ASSERT(m_hWnd == NULL);

    m_hWnd = hWnd;
    m_hWndAfter = hWnd;
    // Store address of the original window procedure
    m_oldWndProc = (WNDPROC)GetWindowLongPtr(m_hWnd, GWLP_WNDPROC);
    
    ASSERT(m_oldWndProc != SubclassWndProc);
    PreSubclassWindow();
    // And set the new one
    m_oldWndProc = (WNDPROC)(::LONG_PTR)SetWindowLongPtr(m_hWnd,GWLP_WNDPROC,(LONG)(::LONG_PTR)SubclassWndProc);
    CEventLock lock(CSubclassWnd::m_CS);
    CSubclassWnd::m_SubclassWndMap[m_hWnd] = this;
    m_bIsSubclassed = true;
}

WNDPROC CSubclassWnd::Unsubclass()
{
    if(!m_bIsSubclassed)
        return NULL;

    WNDPROC hTmpProc = m_oldWndProc;
    if (::IsWindow(m_hWnd)) /*óDê±?é?ü′°?ú±??ú?ùá?￡?μ?ê??ò????óDμ?μ?í¨?a*/
    {
        if(m_oldWndProc && m_bIsSubclassed)
        {
            if(SubclassWndProc == (WNDPROC)GetWindowLong(m_hWnd, GWL_WNDPROC)) /*?′?′ê?·?ê??ò??subclassμ?*/
            {
                //SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)m_oldWndProc);
                SetWindowLongPtr(m_hWnd,GWLP_WNDPROC,(LONG)(::LONG_PTR)m_oldWndProc);
            }
        }
    }

    CEventLock lock(CSubclassWnd::m_CS);
    CSubclassWnd::m_SubclassWndMap.RemoveKey(m_hWnd);

    m_hWndAfter = m_hWnd;
    m_hWnd = NULL;
    m_oldWndProc = NULL;//::DefWindowProc;
    m_bIsSubclassed = false;
    return hTmpProc;
}

void CSubclassWnd::Attach(HWND hWnd)
{
    ASSERT(hWnd);
    Subclass(hWnd);
}

HWND CSubclassWnd::Detach()
{
    Unsubclass();
    return m_hWndAfter;
}

void CSubclassWnd::PreSubclassWindow()
{
    ASSERT(m_hWnd);
}

void CSubclassWnd::PostNcDestroy()
{
}

LRESULT CSubclassWnd::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ASSERT(m_hWnd);
    ASSERT(m_hWndAfter);
    ASSERT(m_oldWndProc);
#if 0
    if(uMsg == WM_CLOSE) /*有时侯窗口处理完WM_CLOSE就没有WM_NCDESTROY了*/
    {
        WNDPROC hOld = Unsubclass();
        LRESULT lr = ::CallWindowProc(hOld, m_hWndAfter, uMsg, wParam, lParam);
        return lr;
    }
#endif
    if(uMsg == WM_NCDESTROY)//
    {
        LRESULT lr;
        if(m_hWnd != NULL)
        {
            WNDPROC hOld = Unsubclass();
            lr = ::CallWindowProc(hOld, m_hWndAfter, uMsg, wParam, lParam);
        }
        else
        {
            lr = ::DefWindowProc(m_hWndAfter, uMsg, wParam, lParam);
        }
        PostNcDestroy();
        return lr;
    }

    return ::CallWindowProc(m_oldWndProc, m_hWnd, uMsg, wParam, lParam);
}

LRESULT CSubclassWnd::CallDefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ASSERT(m_hWnd);
    ASSERT(m_oldWndProc);
    return ::CallWindowProc(m_oldWndProc, m_hWnd, uMsg, wParam, lParam);
}
