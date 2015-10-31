// CmdIconMap.h: interface for the CCmdIconMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDICONMAP_H__C012441C_49B1_497E_A9D7_16FCBAB80D0C__INCLUDED_)
#define AFX_CMDICONMAP_H__C012441C_49B1_497E_A9D7_16FCBAB80D0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IconMapItem.h"

class CCmdIconMap  
{
public:
    CCmdIconMap();
    CCmdIconMap(UINT uCmdID);
    virtual ~CCmdIconMap();

    BOOL AddStateItem(int nState, BOOL bTRansparent, const RECT& rcRange, const RECT& rcMargin, COLORREF crTransparent);
    BOOL AddStateItems(IconStateDate *pIconStates, int nCount);
    void ClearAllItems();
    CIconMapItem* QueryStateItem(int nState);

    UINT m_uCmdID;
protected:
    map<int, CIconMapItem *> m_IconsStateMap;
};

#endif // !defined(AFX_CMDICONMAP_H__C012441C_49B1_497E_A9D7_16FCBAB80D0C__INCLUDED_)
