#ifndef __WZTAB_ITEM_H__
#define __WZTAB_ITEM_H__

#include "WzTabGlobal.h"
#include "WzTabSubItem.h"

class CTabItem
{
public:
	CTabItem();
	~CTabItem();
	int GetSubItemCount();
	int GetHoverSubItem() { return m_nHoverSubTab; };
	CSize CalcItemSize(CDC *pDC, BOOL bShowClose, BOOL bShowIcon);
	int HitTest(const CPoint& pt, BOOL bShowClose);
	int GetSubItemIndexById(LONG nSubItemId);
	CTabSubItem* GetSubItemByIndex(int index);
	LONG GetSubItemId(int index);
    int GetConceivableActiveSubItemIndex();
    LONG GetConceivableActiveSubItemId();
    TAB_SUB_ITEM_STATE GetSubItemStateByIndex(int index);
    BOOL RemoveSubItemByIndex(int index);
    
public:
	int m_nLastHit;
	int m_nLastState;
	CPoint m_ptTopLeft;
	CString m_strItemText;
	int m_nActiveSubTab;
	int m_nLastActiveSubTab;
	int m_nHoverSubTab;
	CRect m_rcBaseArea;
	CRect m_rcItem; //坐标相对于tab窗口的客户区

	CArray<CTabSubItem, CTabSubItem&> m_arSubItems;
};

#endif /*__WZTAB_ITEM_H__*/
