// PowerModeConfig.cpp : implementation file
//

#include "stdafx.h"
#include "PowerModeConfig.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CPowerModeConfig s_powerCfg;

CPowerModeConfig *GlobalPowerModeConfig()
{
    return &s_powerCfg;
}

CPowerModeConfig::CPowerModeConfig()
{
    m_gravity = 0.085;//0.075;
    m_fade = 0.95;
    m_radius = 3.0;
    m_cMin = 5;
    m_cMax = 15;
    m_initialColor = RGB(255, 255, 255);
    m_colorMode = POWER_COLOR_MODE_STATIC; //fixed color
    m_fadeoutColor = RGB(166, 202, 240);
    m_vxMin = -1.0;
    m_vxMax = 1.0;
    m_vyMin = -3.0;
    m_vyMax = -1.0;
}

CPowerModeConfig::~CPowerModeConfig()
{
}
