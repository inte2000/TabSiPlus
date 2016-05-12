#ifndef __POWER_MODE_CONFIG_H__
#define __POWER_MODE_CONFIG_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define POWER_COLOR_MODE_STATIC   0
#define POWER_COLOR_MODE_RANDOM   1

class CPowerModeConfig
{
// Construction
public:
    CPowerModeConfig();
    virtual ~CPowerModeConfig();

// Operations
public:
    double GetGravity() { return m_gravity; }
    void SetGravity(double gravity) { m_gravity = gravity; }
    double GetFadeRate() { return m_fade; }
    void SetFadeRate(double fade) { m_fade = fade; }
    double GetRadius() { return m_radius; }
    void SetRadius(double radius) { m_radius = radius; }
    int GetMaxParticles() { return m_cMax; }
    void SetMaxParticles(int count) { m_cMax = count; }
    int GetMinParticles() { return m_cMin; }
    void SetMinParticles(int count) { m_cMin = count; }
    int GetColorMode() { return m_colorMode; }
    void SetColorMode(int mode) { m_colorMode = mode; }
    double GetVxMin() { return m_vxMin; }
    void SetVxMin(double vx) { m_vxMin = vx; }
    double GetVxMax() { return m_vxMax; }
    void SetVxMax(double vx) { m_vxMax = vx; }
    double GetVyMin() { return m_vyMin; }
    void SetVyMin(double vy) { m_vyMin = vy; }
    double GetVyMax() { return m_vyMax; }
    void SetVyMax(double vy) { m_vyMax = vy; }

    COLORREF GetInitialColor() { return m_initialColor; }
    void SetInitialColor(COLORREF color) { m_initialColor = color; }
    COLORREF GetFadeoutColor() { return m_fadeoutColor; }
    void SetFadeoutColor(COLORREF color) { m_fadeoutColor = color; }

    // Implementation
protected:

    // Generated message map functions
protected:
    double m_gravity;
    double m_fade;
    double m_radius;
    int m_cMin;
    int m_cMax;
    double m_vxMin;
    double m_vxMax;
    double m_vyMin;
    double m_vyMax;
    COLORREF m_initialColor;
    int m_colorMode;
    COLORREF m_fadeoutColor;
};

CPowerModeConfig *GlobalPowerModeConfig();

#endif // __POWER_MODE_CONFIG_H__
