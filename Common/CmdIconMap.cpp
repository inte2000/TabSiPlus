// CmdIconMap.cpp: implementation of the CCmdIconMap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdIconMap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdIconMap::CCmdIconMap()
{
    m_uCmdID = 0;
}

CCmdIconMap::CCmdIconMap(UINT uCmdID)
{
    m_uCmdID = uCmdID;
}

CCmdIconMap::~CCmdIconMap()
{
    ClearAllItems();
}

BOOL CCmdIconMap::AddStateItem(int nState, BOOL bTRansparent, const RECT& rcRange, const RECT& rcMargin, COLORREF crTransparent)
{
    CIconMapItem *pItem = new CIconMapItem(nState,bTRansparent,rcRange,rcMargin,crTransparent);
    if(pItem)
    {
        m_IconsStateMap[nState] = pItem;
        return TRUE;
    }

    return FALSE;
}

BOOL CCmdIconMap::AddStateItems(IconStateDate *pIconStates, int nCount)
{
    for(int i = 0; i < nCount; i++)
    {
        CIconMapItem *pItem = new CIconMapItem(&pIconStates[i]);
        if(pItem)
        {
            m_IconsStateMap[pIconStates[i].nState] = pItem;
        }
    }

    return TRUE;
}

void CCmdIconMap::ClearAllItems()
{
    map<int, CIconMapItem *>::iterator it;
    for(it = m_IconsStateMap.begin(); it != m_IconsStateMap.end(); it++)
    {
        CIconMapItem *pItem = it->second;
        WZ_ASSERT(pItem);
        delete pItem;
    }
    m_IconsStateMap.erase(m_IconsStateMap.begin(),m_IconsStateMap.end());
}

CIconMapItem* CCmdIconMap::QueryStateItem(int nState)
{
    CIconMapItem *pFindItem = NULL;
    map<int, CIconMapItem *>::iterator it = m_IconsStateMap.find(nState);
    if(it != m_IconsStateMap.end())
    {
        pFindItem = it->second;
    }
    else
    {
        //try to load normal state icon
        it = m_IconsStateMap.find(nIconStateNormal);
        if(it != m_IconsStateMap.end())
        {
            pFindItem = it->second;
        }
    }

    return pFindItem;
}

