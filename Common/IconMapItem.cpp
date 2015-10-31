// IconMapItem.cpp: implementation of the CIconMapItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IconMapItem.h"
#include "HelperFunction.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIconMapItem::CIconMapItem()
{
    m_nState = nIconStateNormal;
    m_bTransparent = FALSE;
    SetRectEmpty(&m_rcRange);
    SetRectEmpty(&m_rcMargin);
    m_crTransparent = RGB(0,0,0);
}

CIconMapItem::CIconMapItem(int nState, BOOL bTRansparent, const RECT& rcRange, const RECT& rcMargin, COLORREF crTransparent)
{
    m_nState = nState;
    m_bTransparent = bTRansparent;
    m_rcRange = rcRange;
    m_rcMargin = rcMargin;
    m_crTransparent = crTransparent;
}

CIconMapItem::CIconMapItem(IconStateDate *pIconState)
{
    m_nState = pIconState->nState;
    m_bTransparent = pIconState->bTransparent;
    m_rcRange = pIconState->rcRange;
    m_rcMargin = pIconState->rcMargin;
    m_crTransparent = pIconState->crTRansparent;
}

CIconMapItem::~CIconMapItem()
{
}

BOOL CIconMapItem::DrawBmpObject(HDC hDC, const RECT& rcClient, HDC hBmpDC)
{
    DrawBitmapObject(hDC,rcClient,hBmpDC,m_rcRange,m_rcMargin,m_bTransparent,m_crTransparent);
    return TRUE;
}

BOOL CIconMapItem::DrawBmpObject2(HDC hDC, const RECT& rcClient, HBITMAP hIconBmp)
{
    HDC hTempDC = CreateCompatibleDC(hDC);
    HGDIOBJ hOldTempBmp = SelectObject(hTempDC,hIconBmp);
    BOOL bRtn = DrawBmpObject(hDC,rcClient,hTempDC);
    SelectObject(hTempDC,hOldTempBmp);
    DeleteDC(hTempDC);

    return bRtn;
}
