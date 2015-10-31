#ifndef __WZTABTHEME_H__
#define __WZTABTHEME_H__

#include "WzTabGlobal.h"

const int themeFilesign = 21897764;

class CWzTabTheme
{
public:
    virtual COLORREF GetTabWndBkgndColor() = 0;
    virtual CFont *GetTextFont() = 0;
    virtual void DrawSubItem(CDC *pDC, int iconIdx, const CString& text, const CRect& rcSubItem, 
                     TAB_ITEM_STATE baseState, TAB_SUB_ITEM_STATE subState) = 0;
    virtual void DrawBkgnd(CDC *pDC, const CRect& rcClient) = 0;
    virtual void DrawItemCloseBtn(CDC *pDC, const CRect& rcIcon, int lastHit, int lastState) = 0;
    virtual void DrawTabBorder(CDC *pDC, CRect& rcBorder, TAB_ITEM_STATE state) = 0;
    virtual void DrawTabBkgnd(CDC *pDC, const CRect& rcBkgnd, TAB_ITEM_STATE state) = 0;
    virtual void DrawTabText(CDC *pDC, const CRect& rcText, LPCTSTR lpszText, TAB_ITEM_STATE state) = 0;

};

#endif /*__WZTABTHEME_H__*/

