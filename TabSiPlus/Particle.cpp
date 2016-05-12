// Particle.cpp : implementation file
//

#include "stdafx.h"
#include "Particle.h"
#include "HelperFunction.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


inline void CalcRectRange(double x, double y, double r, LPRECT lprc)
{
    lprc->left = static_cast<int>(x - r + 0.5);
    lprc->top = static_cast<int>(y - r + 0.5);
    lprc->right = static_cast<int>(x + r + 0.5);
    lprc->bottom = static_cast<int>(y + r + 0.5);
}

CParticle::CParticle(int x, int y, double vx, double vy, COLORREF color)
{
    m_x = x;
    m_y = y;
    m_vx = vx;
    m_vy = vy;
    m_alpha = 1.0;
    m_color = color;
}

CParticle::~CParticle()
{
}

CParticle& CParticle::operator = (const CParticle& p)
{
    if(this != &p)
    {
        Assign(p);
    }

    return *this;
}

void CParticle::InitParticle(int x, int y, double vx, double vy, COLORREF color)
{
    m_x = x;
    m_y = y;
    m_vx = vx;
    m_vy = vy;
    m_alpha = 1.0;
    m_color = color;
}

void CParticle::UpdateStatus(double gravity, double fade)
{
    m_vy += gravity;
    m_x += m_vx;
    m_y += m_vy;
    m_alpha *= fade;
    //m_color = ;
}

void CParticle::Draw(HDC hDC, double r, COLORREF crDest)
{
//    int ptx = static_cast<int>(m_x + 0.5);
//    int pty = static_cast<int>(m_y + 0.5);

    RECT rcRange;
    CalcRectRange(m_x, m_y, r, &rcRange);
    COLORREF color = MixedColor(m_color, crDest, m_alpha);

    FillSolidRound(hDC, rcRange, color);
}

void CParticle::GetRect(double r, LPRECT lprc)
{
    CalcRectRange(m_x, m_y, r, lprc);
}

void CParticle::Assign(const CParticle& p)
{
    m_x = p.m_x;
    m_y = p.m_y;
    m_vx = p.m_vx;
    m_vy = p.m_vy;
    m_alpha = p.m_alpha;
    m_color = p.m_color;
}
