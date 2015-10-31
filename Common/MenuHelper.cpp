// MenuHelper.cpp: implementation of the CMenuHelper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MenuHelper.h"
#include "HelperFunction.h"
#include "CmdIconResource.h"
#include "WndUIResource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMenuHelper::CMenuHelper()
{
    m_hActiveMenu = NULL;
/*
    m_crText = RGB(0,0,0);
    m_crHiText = RGB(0,0,0);
    m_crBkGnd = RGB(255,255,220);
    m_crHiBkGnd = RGB(255,238,194);
    m_crLeftbar = RGB(215,241,215);
*/
    m_nLeftSpace = 22;
    m_nRightSpace = 16;
    m_bIsSysMenu = FALSE;
}

CMenuHelper::~CMenuHelper()
{
}

/*
LRESULT CMenuHelper::OnExitMenuLoop(WPARAM wParam, LPARAM lParam)
{
    DetachActiveMenu();
    return 0;
}

LRESULT CMenuHelper::OnInitMenu(WPARAM wParam, LPARAM lParam)
{
    HMENU hMenu = (HMENU)wParam;
    AttachActiveMenu(hMenu);
    return 0;
}
*/

LRESULT CMenuHelper::OnMeasureItem(WPARAM wParam, LPARAM lParam)
{
    UINT uCtrlID = (UINT)wParam;
    LPMEASUREITEMSTRUCT pMI = (LPMEASUREITEMSTRUCT)lParam;
    if(pMI->CtlType == ODT_MENU)//We only want menu
    {
        WZ_ASSERT(m_hActiveMenu != NULL);

        MENUITEMINFO mii;
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_FTYPE | MIIM_STRING;
        mii.dwTypeData = NULL;
        GetMenuItemInfo(m_hActiveMenu,pMI->itemID,FALSE,&mii);//get the string length

        CWndUIResource *pUIRes = GlobalGetUIRes();
        WZ_ASSERT(pUIRes);

        if(mii.fType & MFT_SEPARATOR)
        {
            pMI->itemWidth = 0;
            pMI->itemHeight = GetSystemMetrics(SM_CYMENU) >> 1;
        }
        else
        {
            int nWidth;
            if(m_bIsSysMenu)
            {
                nWidth = m_nLeftSpace + m_nRightSpace + 50;//系统菜单有些奇怪的地方
            }
            else
            {
                nWidth = m_nLeftSpace + m_nRightSpace + 4;//left space : 2; right space 4
            }
            int nHeight = 18;
            TCHAR *pszText = new TCHAR[mii.cch + 1];
            if(pszText != NULL)
            {
                mii.fMask = MIIM_FTYPE | MIIM_STRING;
                mii.dwTypeData = pszText;
                mii.cch += 1;
                GetMenuItemInfo(m_hActiveMenu,pMI->itemID,FALSE,&mii);//get menu text

                HFONT hFont = pUIRes->GetFont(pResDefaultGuiFont);
                HWND hWnd = ::GetDesktopWindow();
                HDC hDC = ::GetDC(hWnd);
                HGDIOBJ of = ::SelectObject(hDC,hFont);
                //rect.left=rect.top=0;
                SIZE size;
                /*
                if(_tcschr(pszText,_T('\t')) != NULL)
                {
                    WZ_TRACE(_T("%s\n"),pszText);
                }
                */
                RECT rect = { 0, 0, 0, 0 };
                size.cy = ::DrawText(hDC,pszText,-1,&rect,DT_SINGLELINE|DT_LEFT|DT_VCENTER|DT_CALCRECT|DT_EXPANDTABS);
                size.cx = rect.right - rect.left + 3;
                ::SelectObject(hDC,of);
                ::ReleaseDC(hWnd,hDC);  // Release the DC
                // Set width and height:
                nWidth += (size.cx + 8);//GAP;
                int temp = GetSystemMetrics(SM_CYMENU);
                nHeight = temp > 20 ? temp : 20;
            
                delete[] pszText;
            }

            pMI->itemWidth = nWidth;
            pMI->itemHeight = nHeight;
        }

        return 1;
    }
    else
        return 0;
}

LRESULT CMenuHelper::OnDrawItem(WPARAM wParam, LPARAM lParam)
{
    UINT uCtrlID = (UINT)wParam;
    LPDRAWITEMSTRUCT pDI = (LPDRAWITEMSTRUCT)lParam;
    
    if(pDI->CtlType == ODT_MENU)//We only want menu
    {
        WZ_ASSERT(m_hActiveMenu != NULL);
        
        HDC dc = pDI->hDC;
        RECT rect = pDI->rcItem;

        CWndUIResource *pUIRes = GlobalGetUIRes();
        WZ_ASSERT(pUIRes);
        CCmdIconResource *pIconRes = GlobalGetCmdIconResource();
        WZ_ASSERT(pIconRes);

        COLORREF crText = pUIRes->GetColor(pResMenuTextNormal);
        COLORREF crHiText = pUIRes->GetColor(pResMenuTextHilight);
        COLORREF crBkGnd = pUIRes->GetColor(pResMenuBkgndNormal);
        COLORREF crHiBkGnd = pUIRes->GetColor(pResMenuBkgndHilight);
        COLORREF crLeftbar = pUIRes->GetColor(pResMebuLeftbar);
        COLORREF crDisableText = pUIRes->GetColor(pResMenuTextDisabled);
        
        MENUITEMINFO mii;
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_FTYPE | MIIM_STRING;
        mii.dwTypeData = NULL;
        GetMenuItemInfo(m_hActiveMenu,pDI->itemID,FALSE,&mii);//get the string length

        if(mii.fType & MFT_SEPARATOR)
        {
            RECT rcLeft = rect;
            rcLeft.right = rcLeft.left + m_nLeftSpace;//bitmap width
            rect.left += m_nLeftSpace;
            FillSolidRect(dc,rcLeft,crLeftbar);
            FillSolidRect(dc,rect,crBkGnd);
            rect.top += ((rect.bottom - rect.top) >> 1);
            DrawEdge(dc,&rect,EDGE_ETCHED,BF_TOP);            
        }
        else
        {
            TCHAR *pszText = new TCHAR[mii.cch + 1];
            if(pszText != NULL)
            {
                mii.fMask = MIIM_FTYPE | MIIM_STRING;
                mii.dwTypeData = pszText;
                mii.cch += 1;
                GetMenuItemInfo(m_hActiveMenu,pDI->itemID,FALSE,&mii);//get the string

                mii.fMask = MIIM_STATE | MIIM_FTYPE | MIIM_BITMAP;
                mii.cch = 0;
                GetMenuItemInfo(m_hActiveMenu,pDI->itemID,FALSE,&mii);//get bitmap handle

                BOOL bDisabled = (mii.fState & MFS_GRAYED) == MFS_GRAYED;
                BOOL bChecked = (mii.fState & MFS_CHECKED) == MFS_CHECKED;
                BOOL bDefaultItem = (mii.fState & MFS_DEFAULT) == MFS_DEFAULT;
                //BOOL bSelected = (mii.fState & MFS_HILITE) == MFS_HILITE;
                BOOL bSelected = pDI->itemState & ODS_SELECTED;
                //BOOL bSelected = pDI->itemState & ODS_SELECTED;

                CCmdIconMap *pIconMap = pIconRes->QueryIconMap(pDI->itemID);

                RECT rcText,rcIcon;
                int dy;
                
                HFONT hFont = pUIRes->GetFont(bDefaultItem ? pResDefaultGuiBoldFont : pResDefaultGuiFont);
                HFONT of = (HFONT)::SelectObject(dc,hFont);
                
                dy = (rect.bottom - rect.top - 4 - 15) / 2;
                dy = (dy < 0) ? 0 : dy;

                SetRect(&rcText,rect.left + m_nLeftSpace + 6,rect.top,rect.right,rect.bottom);
                
                rcIcon.left = rect.left + 2;
                rcIcon.right = rcIcon.left + nIconSize;
                rcIcon.top = rect.top + 1;
                rcIcon.bottom = rcIcon.top + nIconSize;

                if(pDI->itemAction & ODA_DRAWENTIRE)
                {
                    RECT rcLeft = rect;
                    rcLeft.right = rcLeft.left + m_nLeftSpace;//bitmap width
                    rect.left += m_nLeftSpace;
                    FillSolidRect(dc,rcLeft,crLeftbar);
                    FillSolidRect(dc,rect,crBkGnd);

//                    if(mii.hbmpItem != NULL)//have icon
                    if(pIconMap)//有Icon
                    {
                        int nState = TransMenuIconState(mii.fState,pDI->itemState);
                        CIconMapItem *pIconItem = pIconMap->QueryStateItem(nState);
                        if(pIconItem)
                        {
                            pIconItem->DrawBmpObject2(dc,rcIcon,pIconRes->GetIconBitmap());
                        }
                    }
                    else if((mii.fState & MFS_CHECKED) == MFS_CHECKED)
                    {
                        HPEN hCheckPen = pUIRes->GetPen(pResCheckMarkPen,PS_SOLID, 2, crText);
                        HPEN hop = (HPEN)SelectObject(dc,hCheckPen);

                        MoveToEx(dc,rcIcon.left + 4,rcIcon.top + 5,NULL);
                        LineTo(dc,rcIcon.left + 5,rcIcon.top + 10);
                        MoveToEx(dc,rcIcon.left + 5,rcIcon.top + 11,NULL);
                        LineTo(dc,rcIcon.left + 11,rcIcon.top + 5);

                        SelectObject(dc,hop);
                    }

                    RECT rect2;
                    SetRect(&rect2,rcText.left + 4,rcText.top + 1,rcText.right - 2,rcText.bottom - 1);
                    COLORREF oclr = SetTextColor(dc, bDisabled ? crDisableText : crText);
                    int mode = SetBkMode(dc,TRANSPARENT);
                    DrawTabMenuText(dc,pszText,&rect2);
                    //DrawText(dc,pszText,-1,&rect2,DT_LEFT|DT_NOCLIP|DT_SINGLELINE|DT_VCENTER|DT_EXPANDTABS);
                    SetBkMode(dc,mode);
                    SetTextColor(dc,oclr);
                }//if (lpDIS->itemAction & ODA_DRAWENTIRE)

                //Draws the selected item
                if((pDI->itemState & ODS_SELECTED) && (pDI->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
                {
                    RECT rect2;
                    //if(mii.hbmpItem != NULL || (mii.fState & MFS_CHECKED) == MFS_CHECKED)
                    if(pIconMap || (mii.fState & MFS_CHECKED) == MFS_CHECKED)
                    {
                        SetRect(&rect2,rcIcon.left - 1,rcIcon.top - 1,rcIcon.right + 1,rcIcon.bottom + 1);
                        if(!bDisabled)
                        {
                            Draw3dRect(dc,rect2,pUIRes->GetColor(pRes3DHiLight),pUIRes->GetColor(pRes3DDarkShadow));
                        }
                        rect2 = rcText;
                    }
                    else
                    {
                        SetRect(&rect2,rect.left + 2,rect.top + 1,rect.right - 2,rect.bottom - 1);
                    }
                    
                    //draw hightlight background
                    FillSolidRect(dc,rect2,bDisabled ? crBkGnd : crHiBkGnd);
                    COLORREF crFrame = bDisabled ? pUIRes->GetColor(pResCtrlRectFrameDisabled) : pUIRes->GetColor(pResCtrlRectFrameHilight);
                    HPEN penFrame = pUIRes->GetPen((bDisabled ? pResCtrlRectFrameDisabled : pResCtrlRectFrameHilight) ,PS_SOLID, 1, crFrame);
                    HPEN hOP = (HPEN)SelectObject(dc,penFrame);
                    SelectObject(dc,pUIRes->GetBrush(pResNullBrush));
                    Rectangle(dc,rect2.left,rect2.top,rect2.right,rect2.bottom);
                    SelectObject(dc,hOP);
                    
                    //draw highlight text
                    SetRect(&rect2,rcText.left + 4,rcText.top + 1,rcText.right - 2,rcText.bottom - 1);
                    COLORREF oclr = SetTextColor(dc,bDisabled ? crDisableText : crHiText);
                    int mode = SetBkMode(dc,TRANSPARENT);
                    DrawTabMenuText(dc,pszText,&rect2);
                    SetBkMode(dc,mode);
                    SetTextColor(dc,oclr);
                }

                //Draws the deselected item
                if(!(pDI->itemState & ODS_SELECTED) && (pDI->itemAction & ODA_SELECT))
                {
                    RECT rect2;
                    //if(mii.hbmpItem != NULL || (mii.fState & MFS_CHECKED) == MFS_CHECKED)
                    if(pIconMap || (mii.fState & MFS_CHECKED) == MFS_CHECKED)
                    {
                        SetRect(&rect2,rcIcon.left - 1,rcIcon.top - 1,rcIcon.right + 1,rcIcon.bottom + 1);
                        Draw3dRect(dc, rect2,crLeftbar,crLeftbar);
                        rect2 = rcText;
                    }
                    else
                    {
                        RECT rcLeft = rect;
                        rcLeft.right = rcLeft.left + m_nLeftSpace;//bitmap width
                        rect.left += m_nLeftSpace;
                        FillSolidRect(dc,rcLeft,crLeftbar);
                        FillSolidRect(dc,rect,crBkGnd);
                        SetRect(&rect2,rect.left + 2,rect.top + 1,rect.right - 2,rect.bottom - 1);
                    }

                    //draw background
                    FillSolidRect(dc,rect2,crBkGnd);
                    SetRect(&rect2,rcText.left+4,rcText.top+1,rcText.right-2,rcText.bottom-1);
                    //draw Text
                    COLORREF oclr = SetTextColor(dc,bDisabled ? crDisableText : crText);
                    int mode = SetBkMode(dc,TRANSPARENT);
                    DrawTabMenuText(dc,pszText,&rect2);
                    SetBkMode(dc,mode);
                    SetTextColor(dc,oclr);
                }
                
                SelectObject(dc,of);
                
                delete []pszText;
            }//if(pszText)
        }

        return 1;
    }
    else
        return 0;
}

HMENU CMenuHelper::AttachActiveMenu(HMENU hActiveMenu)
{
    HMENU hTmp = m_hActiveMenu;
    m_hActiveMenu = hActiveMenu;
    int nSysItem = 0;
    int Count = GetMenuItemCount(m_hActiveMenu);
    for(int i = 0; i < Count; i++)
    {
        MENUITEMINFO mii;
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_FTYPE|MIIM_ID;
        GetMenuItemInfo(m_hActiveMenu,i,TRUE,&mii);
        if((mii.wID == SC_CLOSE) || (mii.wID == SC_RESTORE) || (mii.wID == SC_MAXIMIZE) || (mii.wID == SC_MINIMIZE) 
            || (mii.wID == SC_MOVE) || (mii.wID == SC_SIZE))
            nSysItem++;
        mii.fMask = MIIM_FTYPE;
        mii.fType |= MFT_OWNERDRAW;
        SetMenuItemInfo(m_hActiveMenu,i,TRUE,&mii);
    }

    m_bIsSysMenu = (nSysItem >= 2);
    return hTmp;
}

HMENU CMenuHelper::DetachActiveMenu()
{
    int Count = GetMenuItemCount(m_hActiveMenu);
    for(int i = 0; i < Count; i++)
    {
        MENUITEMINFO mii;
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_FTYPE;
        GetMenuItemInfo(m_hActiveMenu,i,TRUE,&mii);
        mii.fMask = MIIM_FTYPE;
        mii.fType &= ~MFT_OWNERDRAW;
        SetMenuItemInfo(m_hActiveMenu,i,TRUE,&mii);
    }

    HMENU hTmp = m_hActiveMenu;
    m_hActiveMenu = NULL;
    return hTmp;
}

BOOL CMenuHelper::IsThisMenuItem(UINT nItemID)
{
    BOOL bIsThisItem = FALSE;
    int Count = GetMenuItemCount(m_hActiveMenu);
    for(int i = 0; i < Count; i++)
    {
        MENUITEMINFO mii;
        mii.cbSize = sizeof(MENUITEMINFO);
        mii.fMask = MIIM_ID;
        GetMenuItemInfo(m_hActiveMenu,i,TRUE,&mii);
        if((UINT)mii.wID == nItemID)
        {
            bIsThisItem = TRUE;
            break;
        }
    }

    return bIsThisItem;
}
/*
const int nIconStateNormal   = 1;
const int nIconStateHover    = 2;
const int nIconStateSelected = 3;
const int nIconStateDisabled = 4;
*/
int CMenuHelper::TransMenuIconState(UINT fState, UINT ItemState)
{
    int nState;
    if((fState & MFS_DISABLED) == MFS_DISABLED)
        nState = nIconStateDisabled;
    else
    {
        if((fState & MFS_HILITE) == MFS_HILITE)
            nState = nIconStateHover;
        else if(ItemState & ODS_SELECTED)
            nState = nIconStateSelected;
        else
            nState = nIconStateNormal;
    }
    return nState;
}

void CMenuHelper::DrawTabMenuText(HDC hDC, LPCTSTR lpszText, LPRECT lpTextArea)
{
    RECT rcCli = *lpTextArea;
    rcCli.right -= m_nRightSpace;
    LPCTSTR lpszTab = _tcschr(lpszText,_T('\t'));
    if(lpszTab)
    {
        //TCHAR szFirst[256],szSecond[256];
        int nTotalLen = _tcslen(lpszText);
        int nLeftLen = lpszTab - lpszText;
        int nRightLen = nTotalLen - nLeftLen - 1;
        DrawText(hDC,lpszText,nLeftLen,&rcCli,DT_LEFT|DT_NOCLIP|DT_SINGLELINE|DT_VCENTER);
        DrawText(hDC,lpszTab + 1,nRightLen,&rcCli,DT_RIGHT|DT_NOCLIP|DT_SINGLELINE|DT_VCENTER);
    }
    else
    {
        DrawText(hDC,lpszText,-1,&rcCli,DT_LEFT|DT_NOCLIP|DT_SINGLELINE|DT_VCENTER);
    }
}

