// MenuHelper.h: interface for the CMenuHelper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENUHELPER_H__37E509B8_7018_4790_832A_AC4900903F53__INCLUDED_)
#define AFX_MENUHELPER_H__37E509B8_7018_4790_832A_AC4900903F53__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


const int nIconSize = 16;

class CMenuHelper
{
public:
    CMenuHelper();
    virtual ~CMenuHelper();

public:
//    LRESULT OnExitMenuLoop(WPARAM wParam, LPARAM lParam);
//    LRESULT OnInitMenu(WPARAM wParam, LPARAM lParam);
    LRESULT OnMeasureItem(WPARAM wParam, LPARAM lParam);
    LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);

    HMENU AttachActiveMenu(HMENU hActiveMenu);
    HMENU DetachActiveMenu();
    HMENU GetAssocMenu() {return m_hActiveMenu; };
    BOOL  IsThisMenuItem(UINT nItemID);

protected:
    int TransMenuIconState(UINT fState, UINT ItemState);
    void DrawTabMenuText(HDC hDC, LPCTSTR lpszText, LPRECT lpTextArea);

protected:
    BOOL m_bIsSysMenu;
    HMENU m_hActiveMenu;
/*
    COLORREF m_crText;
    COLORREF m_crHiText;
    COLORREF m_crBkGnd;
    COLORREF m_crHiBkGnd;
    COLORREF m_crLeftbar;
*/
    int m_nLeftSpace;
    int m_nRightSpace;
};

#endif // !defined(AFX_MENUHELPER_H__37E509B8_7018_4790_832A_AC4900903F53__INCLUDED_)
