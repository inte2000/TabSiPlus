// CmdIconResource.cpp: implementation of the CCmdIconResource class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CmdIconResource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static CCmdIconResource gIconResource;

CCmdIconResource* GlobalGetCmdIconResource()
{
    return &gIconResource;
}
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCmdIconResource::CCmdIconResource()
{
    m_hIconBmp = NULL;
}

CCmdIconResource::~CCmdIconResource()
{
    ClearIconMap();
}

BOOL CCmdIconResource::LoadIconBitmap(LPCTSTR lpszBmpFile)
{
    m_hIconBmp = (HBITMAP)::LoadImage(NULL,lpszBmpFile,IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION|LR_LOADFROMFILE);
    return (m_hIconBmp != NULL);
}

BOOL CCmdIconResource::LoadIconBitmap(HINSTANCE hResInst, UINT uBmpID)
{
    m_hIconBmp = (HBITMAP)::LoadImage(hResInst,MAKEINTRESOURCE(uBmpID),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);
    return (m_hIconBmp != NULL);
}

BOOL CCmdIconResource::AddIconMap(CCmdIconMap *pCmdIconMap)
{
    WZ_ASSERT(pCmdIconMap);

    map<UINT, CCmdIconMap *>::iterator it = m_CmdIconMaps.find(pCmdIconMap->m_uCmdID);
    if(it != m_CmdIconMaps.end())
    {
        return FALSE;
    }

    m_CmdIconMaps[pCmdIconMap->m_uCmdID] = pCmdIconMap;
    return TRUE;
}

void CCmdIconResource::ClearIconMap()
{
    map<UINT, CCmdIconMap *>::iterator it;
    for(it = m_CmdIconMaps.begin(); it != m_CmdIconMaps.end(); it++)
    {
        CCmdIconMap *pMap = it->second;
        WZ_ASSERT(pMap);
        delete pMap;
    }
    m_CmdIconMaps.erase(m_CmdIconMaps.begin(),m_CmdIconMaps.end());
}

CCmdIconMap* CCmdIconResource::QueryIconMap(UINT uCmdID)
{
    CCmdIconMap *pFindItem = NULL;
    map<UINT, CCmdIconMap *>::iterator it = m_CmdIconMaps.find(uCmdID);
    if(it != m_CmdIconMaps.end())
    {
        pFindItem = it->second;
    }

    return pFindItem;
}

CIconMapItem* CCmdIconResource::QueryIconMapItem(UINT uCmdID, int nState)
{
    CIconMapItem *pFindItem = NULL;
    CCmdIconMap *pMap = QueryIconMap(uCmdID);
    if(pMap)
    {
        pFindItem = pMap->QueryStateItem(nState);
    }

    return pFindItem;
}
