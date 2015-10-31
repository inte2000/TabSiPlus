// OwnerDrawMenuMgmt.h: interface for the COwnerDrawMenuMgmt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OWNERDRAWMENUMGMT_H__19E18352_62FF_4E93_B71D_7D5F806B2CCA__INCLUDED_)
#define AFX_OWNERDRAWMENUMGMT_H__19E18352_62FF_4E93_B71D_7D5F806B2CCA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MenuHelper.h"

class COwnerDrawMenuMgmt  
{
public:
    COwnerDrawMenuMgmt();
    virtual ~COwnerDrawMenuMgmt();

    LRESULT OnUninitMenuPopup(WPARAM wParam, LPARAM lParam);
    LRESULT OnInitMenuPopup(WPARAM wParam, LPARAM lParam);
    LRESULT OnMeasureItem(WPARAM wParam, LPARAM lParam);
    LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);

protected:
    BOOL AddMenu(HMENU hMenu);
    BOOL RemoveMenu(HMENU hMenu);
    CMenuHelper *LookupMenu(HMENU hMenu);
    void RemoveAll();
    HMENU MenuFromID(UINT uItemID);

protected:
    map<HMENU, CMenuHelper * > m_mapPopMenus;
};

#endif // !defined(AFX_OWNERDRAWMENUMGMT_H__19E18352_62FF_4E93_B71D_7D5F806B2CCA__INCLUDED_)
