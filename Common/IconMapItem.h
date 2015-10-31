// IconMapItem.h: interface for the CIconMapItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICONMAPITEM_H__028EA8F7_6276_47F4_B0EB_ECA16A9252A7__INCLUDED_)
#define AFX_ICONMAPITEM_H__028EA8F7_6276_47F4_B0EB_ECA16A9252A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

const int nIconStateNormal   = 1;
const int nIconStateHover    = 2;
const int nIconStateSelected = 3;
const int nIconStateDisabled = 4;

typedef struct tagIconStateDate
{
    int nState;
    BOOL bTransparent;
    COLORREF crTRansparent;
    RECT rcRange;
    RECT rcMargin;
}IconStateDate;

class CIconMapItem
{
public:
    CIconMapItem();
    CIconMapItem(int nState, BOOL bTRansparent, const RECT& rcRange, const RECT& rcMargin, COLORREF crTransparent);
    CIconMapItem(IconStateDate *pIconState);
    virtual ~CIconMapItem();
    UINT GetState();
    BOOL IsTransparent();
    COLORREF GetTransparentColor();
    RECT GetBmpRange();
    RECT GetBmpMargin();

    BOOL DrawBmpObject(HDC hDC, const RECT& rcClient, HDC hBmpDC);
    BOOL DrawBmpObject2(HDC hDC, const RECT& rcClient, HBITMAP hIconBmp);
protected:
    int       m_nState;
    BOOL      m_bTransparent;
    RECT      m_rcRange;
    RECT      m_rcMargin;
    COLORREF  m_crTransparent;
};

inline UINT CIconMapItem::GetState()
{ return m_nState; }

inline BOOL CIconMapItem::IsTransparent()
{ return m_bTransparent; }

inline COLORREF CIconMapItem::GetTransparentColor()
{ return m_crTransparent; }

inline RECT CIconMapItem::GetBmpRange()
{ return m_rcRange; }

inline RECT CIconMapItem::GetBmpMargin()
{ return m_rcMargin; }

#endif // !defined(AFX_ICONMAPITEM_H__028EA8F7_6276_47F4_B0EB_ECA16A9252A7__INCLUDED_)
