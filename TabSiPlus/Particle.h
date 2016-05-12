#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CParticle
{
// Construction
public:
    CParticle(int x, int y, double vx, double vy, COLORREF color);
    CParticle(const CParticle& p) { Assign(p); }
    virtual ~CParticle();

    CParticle& operator = (const CParticle& p);
// Operations
public:
    void InitParticle(int x, int y, double vx, double vy, COLORREF color);
    BOOL IsVanished() { return (m_alpha <= 0.15) ? TRUE : FALSE; }
    BOOL IsInitialize() { return (m_alpha >= 0.99) ? TRUE : FALSE; }
    void UpdateStatus(double gravity, double fade);
    void Draw(HDC hDC, double r, COLORREF crDest);
    void GetRect(double r, LPRECT lprc);

// Implementation
protected:
    void Assign(const CParticle& p);

protected:
    double m_x;
    double m_y;
    double m_vx;
    double m_vy;
    double m_alpha;
    COLORREF m_color;
};


#endif // __PARTICLE_H__
