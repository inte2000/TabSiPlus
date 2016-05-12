// PowerMode.cpp : implementation file
//

#include "stdafx.h"
#include "PowerMode.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

inline double GetRandRange(double vmin, double vmax)
{
    return (double)rand() / (RAND_MAX + 1) * (vmax - vmin) + vmin;
}

CPowerMode::CPowerMode(CPowerModeConfig *pCfg)
{
    m_pmCfg = pCfg;

    m_hAttachWnd = NULL;
    srand((unsigned)time(NULL));
    ::InitializeCriticalSection(&m_cs);
}

CPowerMode::~CPowerMode()
{
    ::DeleteCriticalSection(&m_cs);
}
/*
COLORREF GetWindowsBkColor(HWND hWnd)
{
    COLORREF color = RGB(192, 192, 192);
    HDC hDC = ::GetDC(hWnd);
    if(hDC != NULL)
    {
        RECT rcClient = { 0 };
        ::GetClientRect(hWnd, &rcClient);
        color = ::GetPixel(hDC, rcClient.right - 30, rcClient.top + 25);
        ::ReleaseDC(hWnd, hDC);
    }

    return color;
}
*/
COLORREF GetWindowsBkColor(HWND hWnd)
{
    COLORREF color = RGB(192, 192, 192);
    HDC hdcSrc = ::GetDC(hWnd);
    if(hdcSrc != NULL)
    {
        RECT rcWnd= { 0 };
        ::GetWindowRect(hWnd, &rcWnd);
        int width = rcWnd.right - rcWnd.left;
        int height = rcWnd.bottom - rcWnd.top;

        HDC hdcDest = ::CreateCompatibleDC(hdcSrc);
        HBITMAP hBitmap = ::CreateCompatibleBitmap(hdcSrc, width, height);
        HBITMAP hOldBmp = (HBITMAP)::SelectObject(hdcDest, hBitmap);
        ::BitBlt(hdcDest, 0, 0, width, height, hdcSrc, 0, 0, SRCCOPY);

        color = ::GetPixel(hdcDest, width - 40, rcWnd.top + 10);

        ::SelectObject(hdcDest, hOldBmp);
        ::DeleteDC(hdcDest);
        ::DeleteObject(hBitmap);

        ::ReleaseDC(hWnd, hdcSrc);
    }

    return color;
}

BOOL CPowerMode::Attach(HWND hWnd)
{
    if((hWnd == NULL) || !::IsWindow(hWnd))
    {
        return FALSE;
    }
    ::EnterCriticalSection(&m_cs);
    m_hAttachWnd = hWnd;
    m_bkColor = GetWindowsBkColor(m_hAttachWnd);
    ::LeaveCriticalSection(&m_cs);
    return TRUE;
}

void CPowerMode::Detach()
{
    ::EnterCriticalSection(&m_cs);
    m_hAttachWnd = NULL;
    ::LeaveCriticalSection(&m_cs);
}

BOOL CPowerMode::AddParticles(int x, int y)
{
    ASSERT(m_pmCfg != NULL);

    int count = (int)GetRandRange((double)m_pmCfg->GetMinParticles(), (double)m_pmCfg->GetMaxParticles());

    ::EnterCriticalSection(&m_cs);
    for(int i = 0; i < count; i++)
    {
        double vx = GetRandRange(m_pmCfg->GetVxMin(), m_pmCfg->GetVxMax());
        double vy = GetRandRange(m_pmCfg->GetVyMin(), m_pmCfg->GetVyMax());
        if(m_pmCfg->GetColorMode() == POWER_COLOR_MODE_STATIC)
        {
            m_particles.push_back(CParticle(x, y, vx, vy, m_pmCfg->GetInitialColor()));
        }
        else
        {
            int cr = (int)GetRandRange(1, 255);
            int cg = (int)GetRandRange(1, 255);
            int cb = (int)GetRandRange(1, 255);
            m_particles.push_back(CParticle(x, y, vx, vy, RGB(cr, cg, cb)));
        }
    }
    ::LeaveCriticalSection(&m_cs);

    return TRUE;
}

void CPowerMode::ShowParticles()
{
    ::EnterCriticalSection(&m_cs);
    if(m_hAttachWnd != NULL)
    {
        ClearParticles(m_hAttachWnd);
        UpdateParticles();
        DrawParticles(m_hAttachWnd);
    }
    ::LeaveCriticalSection(&m_cs);
}

void CPowerMode::UpdateParticles()
{
    ASSERT(m_pmCfg != NULL);

    std::list<CParticle>::iterator it = m_particles.begin();
    while(it != m_particles.end())
    {
        it->UpdateStatus(m_pmCfg->GetGravity(), m_pmCfg->GetFadeRate());
        if(it->IsVanished())
        {
            it = m_particles.erase(it);
        }
        else
        {
            it++;
        }
    }
}

void CPowerMode::DrawParticles(HWND hWnd)
{
    ASSERT(m_pmCfg != NULL);

    HDC hDC = ::GetDC(hWnd);
    if(hDC == NULL)
    {
        return;
    }
    int storedDC = ::SaveDC(hDC);

    std::list<CParticle>::iterator it;
    for(it = m_particles.begin(); it != m_particles.end(); ++it)
    {
        it->Draw(hDC, m_pmCfg->GetRadius(), m_pmCfg->GetFadeoutColor());
    }

    ::RestoreDC(hDC, storedDC);
    ::ReleaseDC(hWnd, hDC);
}

void CPowerMode::ClearParticles(HWND hWnd)
{
    ASSERT(m_pmCfg != NULL);

    std::list<CParticle>::iterator it;
    for(it = m_particles.begin(); it != m_particles.end(); ++it)
    {
        RECT rcPart;
        it->GetRect(m_pmCfg->GetRadius(), &rcPart);
        ::RedrawWindow(hWnd, &rcPart, NULL, RDW_INVALIDATE | RDW_ERASE | RDW_UPDATENOW);
    }
}
