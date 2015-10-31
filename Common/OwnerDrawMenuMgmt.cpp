// OwnerDrawMenuMgmt.cpp: implementation of the COwnerDrawMenuMgmt class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "OwnerDrawMenuMgmt.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COwnerDrawMenuMgmt::COwnerDrawMenuMgmt()
{
}

COwnerDrawMenuMgmt::~COwnerDrawMenuMgmt()
{
    RemoveAll();
}

LRESULT COwnerDrawMenuMgmt::OnUninitMenuPopup(WPARAM wParam, LPARAM lParam)
{
    HMENU hMenu = (HMENU)wParam;
    RemoveMenu(hMenu);
    return 0;
}

LRESULT COwnerDrawMenuMgmt::OnInitMenuPopup(WPARAM wParam, LPARAM lParam)
{
    HMENU hMenu = (HMENU)wParam;
    AddMenu(hMenu);
    return 0;
}

LRESULT COwnerDrawMenuMgmt::OnMeasureItem(WPARAM wParam, LPARAM lParam)
{
    UINT CtlID = (UINT)wParam;
    if(CtlID != 0)//不是菜单
        return 0;

    MEASUREITEMSTRUCT *pSM = (MEASUREITEMSTRUCT *)lParam;
    HMENU hActiveMenu = MenuFromID(pSM->itemID);
    if(hActiveMenu == NULL)
        return 0;

    CMenuHelper *pMenu = LookupMenu(hActiveMenu);
    if(!pMenu)
        return 0;
    
    return pMenu->OnMeasureItem(wParam,lParam);
}

LRESULT COwnerDrawMenuMgmt::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
    UINT CtlID = (UINT)wParam;
    if(CtlID != 0)//不是菜单
        return 0;

    DRAWITEMSTRUCT *pSD = (DRAWITEMSTRUCT *)lParam;
    //HMENU hActiveMenu = MenuFromID(pSM->itemID);
    HMENU hActiveMenu = (HMENU)pSD->hwndItem;

    CMenuHelper *pMenu = LookupMenu(hActiveMenu);
    if(!pMenu)
        return 0;
    
    return pMenu->OnDrawItem(wParam,lParam);
}

BOOL COwnerDrawMenuMgmt::AddMenu(HMENU hMenu)
{
    WZ_ASSERT(hMenu);
    
    CMenuHelper *pMenu = LookupMenu(hMenu);
    if(pMenu)
        return TRUE;

    pMenu = new CMenuHelper();
    if(!pMenu)
        return FALSE;
    
    m_mapPopMenus[hMenu] = pMenu;
    pMenu->AttachActiveMenu(hMenu);
    
    return TRUE;
}

BOOL COwnerDrawMenuMgmt::RemoveMenu(HMENU hMenu)
{
    WZ_ASSERT(hMenu);
    map<HMENU, CMenuHelper * >::iterator it = m_mapPopMenus.find(hMenu);
    if(it == m_mapPopMenus.end())
    {
        return FALSE;
    }

    CMenuHelper *pMenu = it->second;
    WZ_ASSERT(pMenu);
    HMENU hOldMenu = pMenu->DetachActiveMenu();
    WZ_ASSERT(hOldMenu == hMenu);
    delete pMenu;
    m_mapPopMenus.erase(it);

    return TRUE;
}

CMenuHelper *COwnerDrawMenuMgmt::LookupMenu(HMENU hMenu)
{
    map<HMENU, CMenuHelper * >::iterator it = m_mapPopMenus.find(hMenu);
    if(it != m_mapPopMenus.end())
    {
        return it->second;
    }

    return NULL;
}

void COwnerDrawMenuMgmt::RemoveAll()
{
    map<HMENU, CMenuHelper * >::iterator it;
    for(it = m_mapPopMenus.begin(); it != m_mapPopMenus.end(); it++)
    {
        CMenuHelper *pMenu = it->second;
        WZ_ASSERT(pMenu);
        HMENU hOldMenu = pMenu->DetachActiveMenu();
        delete pMenu;
    }

    m_mapPopMenus.erase(m_mapPopMenus.begin(),m_mapPopMenus.end());
}

HMENU COwnerDrawMenuMgmt::MenuFromID(UINT uItemID)
{
    HMENU hFind = NULL;
    map<HMENU, CMenuHelper * >::iterator it;
    for(it = m_mapPopMenus.begin(); it != m_mapPopMenus.end(); it++)
    {
        CMenuHelper *pMenu = it->second;
        WZ_ASSERT(pMenu);
        if(pMenu->IsThisMenuItem(uItemID))
        {
            hFind = pMenu->GetAssocMenu();
            break;
        }
    }

    return hFind;
}
