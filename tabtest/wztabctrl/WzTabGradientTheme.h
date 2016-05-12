#ifndef __WZTAB_GRAIDENT_THEME_H__
#define __WZTAB_GRAIDENT_THEME_H__

#include "WzTabTheme.h"

class CWzTabGraidentTheme : public CWzTabTheme
{
public:
	CWzTabGraidentTheme();
	virtual ~CWzTabGraidentTheme();
	
	CImageList *GetImageList() { return m_pImgList; };
	CImageList *SetImageList(CImageList *pImgList);
    BOOL InitConfig(LPCTSTR themeFile);		
    void ReleaseConfig();		

	
	virtual COLORREF GetTabWndBkgndColor();
	virtual CFont *GetTextFont();
	virtual void DrawSubItem(CDC *pDC, int iconIdx, const CString& text, const CRect& rcSubItem, 
	                 TAB_ITEM_STATE baseState, TAB_SUB_ITEM_STATE subState);
	virtual void DrawBkgnd(CDC *pDC, const CRect& rcClient);
	virtual void DrawItemCloseBtn(CDC *pDC, const CRect& rcIcon, int lastHit, int lastState);
	virtual void DrawTabBorder(CDC *pDC, CRect& rcBorder, TAB_ITEM_STATE state);
	virtual void DrawTabBkgnd(CDC *pDC, const CRect& rcBkgnd, TAB_ITEM_STATE state);
	virtual void DrawTabText(CDC *pDC, const CRect& rcText, LPCTSTR lpszText, TAB_ITEM_STATE state);

protected:
	CPen     m_BoderPenNormal;
	CPen     m_BoderPenHover;
	CPen     m_BoderPenSelected;
	CPen     m_BoderPenLastSelected;
	CFont    m_TextFont;
	CImageList *m_pImgList;

	COLORREF m_crTabTextNormal;
	COLORREF m_crTabTextHover;
	COLORREF m_crTabTextSelected;
	COLORREF m_crTabTextLastSelected;

	COLORREF m_crTabBkNormalT;
	COLORREF m_crTabBkNormalC;
	COLORREF m_crTabBkNormalB;
	COLORREF m_crTabBkHoverT;
	COLORREF m_crTabBkHoverC;
	COLORREF m_crTabBkHoverB;
	COLORREF m_crTabBkSelectedT;
	COLORREF m_crTabBkSelectedC;
	COLORREF m_crTabBkSelectedB;
	COLORREF m_crTabBkLastSelectedT;
	COLORREF m_crTabBkLastSelectedC;
	COLORREF m_crTabBkLastSelectedB;

	COLORREF m_crWndBkGnd;
};

#endif /*__WZTAB_GRAIDENT_THEME_H__*/

