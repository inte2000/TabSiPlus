// CmdIconResource.h: interface for the CCmdIconResource class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CMDICONRESOURCE_H__AF180ECE_051C_401C_B930_1C1A94B98392__INCLUDED_)
#define AFX_CMDICONRESOURCE_H__AF180ECE_051C_401C_B930_1C1A94B98392__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CmdIconMap.h"
#include "IconMapItem.h"

class CCmdIconResource;

CCmdIconResource* GlobalGetCmdIconResource();

class CCmdIconResource  
{
public:
    CCmdIconResource();
    virtual ~CCmdIconResource();

    BOOL LoadIconBitmap(LPCTSTR lpszBmpFile);
    BOOL LoadIconBitmap(HINSTANCE hResInst, UINT uBmpID);
//    BOOL LoadIconMap(LPCTSTR lpszIconMapFile);
    BOOL AddIconMap(CCmdIconMap *pCmdIconMap);
    void ClearIconMap();
    
    HBITMAP GetIconBitmap() { return m_hIconBmp; };

    CCmdIconMap* QueryIconMap(UINT uCmdID);
    CIconMapItem* QueryIconMapItem(UINT uCmdID, int nState);
protected:
    HBITMAP       m_hIconBmp;
    map<UINT, CCmdIconMap *> m_CmdIconMaps;

};

#endif // !defined(AFX_CMDICONRESOURCE_H__AF180ECE_051C_401C_B930_1C1A94B98392__INCLUDED_)
