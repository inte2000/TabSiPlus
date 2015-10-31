// WndUIResource.h: interface for the CWndUIResource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WNDUIRESOURCE_H__0D4B522B_FB5E_417E_B815_37DAAA81F726__INCLUDED_)
#define AFX_WNDUIRESOURCE_H__0D4B522B_FB5E_417E_B815_37DAAA81F726__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIResourceName.h"

class CWndUIResource;

CWndUIResource* GlobalGetUIRes();

class CWndUIResource  
{
public:
    CWndUIResource();
    virtual ~CWndUIResource();

    bool PrepareDefaultColors();
    void ReleaseResource();
//Color Ó³Éä²Ù×÷
    COLORREF GetColor(const char *pResName);
    bool SetColor(const char *pResName, COLORREF color);

//Font Ó³Éä²Ù×÷
    HFONT GetFont(const char *pResName, const LOGFONT* lplf = NULL);
    bool SetLogFont(const char *pResName, const LOGFONT* lplf);
    bool SetLogFont(const char *pResName, HFONT hFont);

//Brush Ó³Éä²Ù×÷
    HBRUSH GetBrush(const char *pResName, COLORREF color = RGB(0,0,0));
    bool SetSolidBrush(const char *pResName, COLORREF color);
    bool SetLogBrush(const char *pResName, PLOGBRUSH pLogBrush);
    bool SetBrush(const char *pResName, HBRUSH hBrush);

//PenÓ³Éä²Ù×÷
    HPEN GetPen(const char *pResName, int nStyle = PS_SOLID, int nWidth = 1, COLORREF Color = RGB(0,0,0));
    bool SetPen(const char *pResName, int nStyle, int nWidth, COLORREF Color);
    bool SetPen(const char *pResName, HPEN hPen);

protected:

    bool _GetColor(const char *pResName, COLORREF& Color);
    
protected:
    map<string , COLORREF> m_ColorMap;
    map<string , HBRUSH> m_BrushMap;
    map<string , HPEN> m_PenMap;
    map<string , HFONT> m_FontMap;
};

#endif // !defined(AFX_WNDUIRESOURCE_H__0D4B522B_FB5E_417E_B815_37DAAA81F726__INCLUDED_)
