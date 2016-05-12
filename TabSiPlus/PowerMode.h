#ifndef __POWER_MODE_H__
#define __POWER_MODE_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PowerModeConfig.h"
#include "Particle.h"
#include <vector>
#include <list>

class CPowerMode
{
// Construction
public:
    CPowerMode(CPowerModeConfig *pCfg);
    virtual ~CPowerMode();

// Operations
public:
    BOOL Attach(HWND hWnd);
    void Detach();
    BOOL AddParticles(int x, int y);
    void ShowParticles();

    CPowerModeConfig *GetConfig() { return m_pmCfg; }
    void SetConfig(CPowerModeConfig *pCfg) { m_pmCfg = pCfg; }
// Implementation
protected:
    void UpdateParticles();
    void DrawParticles(HWND hWnd);
    void ClearParticles(HWND hWnd);

    // Generated message map functions
protected:
    CPowerModeConfig *m_pmCfg;
    HWND m_hAttachWnd;
    double m_gravity;
    double m_fade;
    double m_radius;
    int m_cMin;
    int m_cMax;
    double m_vxMin;
    double m_vxMax;
    double m_vyMin;
    double m_vyMax;
    COLORREF m_fixedColor;
    int m_colorMode;
    COLORREF m_bkColor;

    CRITICAL_SECTION m_cs;
    std::list<CParticle> m_particles;
};


#endif // __POWER_MODE_H__
