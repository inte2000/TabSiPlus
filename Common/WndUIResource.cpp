// WndUIResource.cpp: implementation of the CWndUIResource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "WndUIResource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static CWndUIResource g_WndUIRes;

CWndUIResource* GlobalGetUIRes()
{
    return &g_WndUIRes;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWndUIResource::CWndUIResource()
{
}

CWndUIResource::~CWndUIResource()
{
    ReleaseResource();
}


bool CWndUIResource::PrepareDefaultColors()
{
    for(int i = 0; i < DefaultColorCount; i++)
    {
        m_ColorMap[g_DefColor[i].pResName] = g_DefColor[i].Color;
    }

    m_ColorMap[pRes3DHiLight] = ::GetSysColor(COLOR_3DHILIGHT);
    m_ColorMap[pRes3DDarkShadow] = ::GetSysColor(COLOR_3DDKSHADOW);
    m_ColorMap[pResBtnFace] = ::GetSysColor(COLOR_3DFACE);

    m_BrushMap[pResNullBrush] = (HBRUSH)GetStockObject(NULL_BRUSH);

    HFONT hGuiFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
    m_FontMap[pResDefaultGuiFont] = hGuiFont;
    LOGFONT logfont;
    GetObject(hGuiFont,sizeof(LOGFONT),&logfont);
    logfont.lfWeight = FW_BOLD;
    
    HFONT hGuiBoldFont = ::CreateFontIndirect(&logfont);
    if(hGuiBoldFont)
    {
        m_FontMap[pResDefaultGuiBoldFont] = hGuiBoldFont;
    }

    return true;
}

void CWndUIResource::ReleaseResource()
{
    m_ColorMap.erase(m_ColorMap.begin(),m_ColorMap.end());

    map<string , HBRUSH>::iterator brit;
    for(brit = m_BrushMap.begin(); brit != m_BrushMap.end(); brit++)
    {
        if(!IsSystemUiObject(brit->first.c_str()))
        {
            DeleteObject(brit->second);
        }
    }
    m_BrushMap.erase(m_BrushMap.begin(),m_BrushMap.end());

    map<string , HPEN>::iterator pit;
    for(pit = m_PenMap.begin(); pit != m_PenMap.end(); pit++)
    {
        if(!IsSystemUiObject(pit->first.c_str()))
        {
            DeleteObject(pit->second);
        }
    }
    m_PenMap.erase(m_PenMap.begin(),m_PenMap.end());

    map<string , HFONT>::iterator fit;
    for(fit = m_FontMap.begin(); fit != m_FontMap.end(); fit++)
    {
        if(!IsSystemUiObject(fit->first.c_str()))
        {
            DeleteObject(fit->second);
        }
    }
    m_FontMap.erase(m_FontMap.begin(),m_FontMap.end());
}

//Color 映射操作
COLORREF CWndUIResource::GetColor(const char *pResName)
{
    WZ_ASSERT(pResName);

    COLORREF Color;
    if(_GetColor(pResName,Color))
    {
        return Color;
    }
    
    return RGB(0,0,255);
}

bool CWndUIResource::SetColor(const char *pResName, COLORREF color)
{
    WZ_ASSERT(pResName);
    m_ColorMap[pResName] = color;
    
    return true;
}

//Font 映射操作
HFONT CWndUIResource::GetFont(const char *pResName, const LOGFONT* lplf)
{
    WZ_ASSERT(pResName);
    map<string , HFONT>::iterator it = m_FontMap.find(pResName);
    if(it != m_FontMap.end())
    {
        return it->second;
    }
    //没有找到匹配，尝试创建
    if(lplf)
    {
        HFONT hFont = CreateFontIndirect(lplf);
        if(hFont)
        {
            m_FontMap[pResName] = hFont;
            return hFont;
        }
    }

    return NULL;
}

bool CWndUIResource::SetLogFont(const char *pResName, const LOGFONT* lplf)
{
    WZ_ASSERT(pResName);
    WZ_ASSERT(lplf);
    HFONT hFont = CreateFontIndirect(lplf);
    if(hFont)
    {
        m_FontMap[pResName] = hFont;
        return true;
    }

    return false;
}

bool CWndUIResource::SetLogFont(const char *pResName, HFONT hFont)
{
    WZ_ASSERT(pResName);
    WZ_ASSERT(hFont);
    m_FontMap[pResName] = hFont;

    return true;
}

//Brush 映射操作
HBRUSH CWndUIResource::GetBrush(const char *pResName, COLORREF color)
{
    WZ_ASSERT(pResName);
    map<string , HBRUSH>::iterator it = m_BrushMap.find(pResName);
    if(it != m_BrushMap.end())
    {
        return it->second;
    }

    //如果失败，尝试从已知颜色中创建一个Solod Brush
    COLORREF brColor;
    if(!_GetColor(pResName,brColor))
    {
        brColor = color;
    }

    HBRUSH hBrush = CreateSolidBrush(brColor);
    if(hBrush)
    {
        m_BrushMap[pResName] = hBrush;
        return hBrush;
    }

    return NULL;
}

bool CWndUIResource::SetSolidBrush(const char *pResName, COLORREF color)
{
    WZ_ASSERT(pResName);
    HBRUSH hBrush = CreateSolidBrush(color);
    if(hBrush)
    {
        m_BrushMap[pResName] = hBrush;
        return true;
    }

    return false;
}

bool CWndUIResource::SetLogBrush(const char *pResName, PLOGBRUSH pLogBrush)
{
    WZ_ASSERT(pResName);
    WZ_ASSERT(pLogBrush);
    HBRUSH hBrush = CreateBrushIndirect(pLogBrush);
    if(hBrush)
    {
        m_BrushMap[pResName] = hBrush;
        return true;
    }

    return false;
}

bool CWndUIResource::SetBrush(const char *pResName, HBRUSH hBrush)
{
    WZ_ASSERT(pResName);
    WZ_ASSERT(hBrush);

    m_BrushMap[pResName] = hBrush;
    return true;
}

//Pen映射操作
HPEN CWndUIResource::GetPen(const char *pResName, int nStyle, int nWidth, COLORREF Color)
{
    WZ_ASSERT(pResName);

    map<string , HPEN>::iterator it = m_PenMap.find(pResName);
    if(it != m_PenMap.end())
    {
        return it->second;
    }

    //如果失败，尝试从已知颜色中创建一个细实线画笔
    COLORREF PenColor;
    if(!_GetColor(pResName,PenColor))
    {
        PenColor = Color;
    }

    HPEN hPen = CreatePen(nStyle,nWidth,PenColor);
    if(hPen)
    {
        m_PenMap[pResName] = hPen;
        return hPen;
    }

    return NULL;
}

bool CWndUIResource::SetPen(const char *pResName, int nStyle, int nWidth, COLORREF Color)
{
    WZ_ASSERT(pResName);
    HPEN hPen = CreatePen(nStyle,nWidth,Color);
    if(hPen)
    {
        m_PenMap[pResName] = hPen;
        return true;
    }

    return false;
}

bool CWndUIResource::SetPen(const char *pResName, HPEN hPen)
{
    WZ_ASSERT(pResName);
    WZ_ASSERT(hPen);

    m_PenMap[pResName] = hPen;
    return true;
}


bool CWndUIResource::_GetColor(const char *pResName, COLORREF& Color)
{
    map<string , COLORREF>::iterator it = m_ColorMap.find(pResName);
    if(it != m_ColorMap.end())
    {
        Color = it->second;
        return true;
    }

    return false;
}
