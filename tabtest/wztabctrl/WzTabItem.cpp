#include "stdafx.h"
#include "WzTabItem.h"

CTabItem::CTabItem()
{
	m_nLastActiveSubTab = 0;
	m_nActiveSubTab = -1;
	m_nHoverSubTab = -1;
	m_nLastHit = HIT_TEST_NONE;
	m_nLastState = 0;
}

CTabItem::~CTabItem()
{
	m_arSubItems.RemoveAll();
}

int CTabItem::GetSubItemCount()
{
	return m_arSubItems.GetSize();
}

CSize CTabItem::CalcItemSize(CDC *pDC, BOOL bShowClose, BOOL bShowIcon)
{
	ASSERT(pDC);
	int nWidth = 0;
	int nHeight = 0;
	
    CRect rcTextBox = CRect(0,0,0,0);
	int nTextHeight = pDC->DrawText(m_strItemText,&rcTextBox,DT_CALCRECT|DT_LEFT|DT_SINGLELINE|DT_VCENTER);
    nTextHeight *= 2;
	if((nTextHeight + 2 * BORDER_MARGIN) < DEFAULT_ITEM_HEIGHT)
		nHeight = DEFAULT_ITEM_HEIGHT;
	else 
		nHeight = nTextHeight + 2 * BORDER_MARGIN;

	m_rcBaseArea.top = 0;
	m_rcBaseArea.bottom = nHeight;
	m_rcBaseArea.left = 0;
	m_rcBaseArea.right = rcTextBox.Width() + TEXT_AREA_PAD;
	if(bShowClose)
		m_rcBaseArea.right += ICON_AREA_SIZE;

	m_rcItem = m_rcBaseArea;
	for(int i = 0; i < GetSubItemCount(); i++)
	{
		CSize subSize = m_arSubItems[i].CalcSubItemSize(pDC, bShowIcon);
		m_rcItem.right += subSize.cx;
		if(nHeight < subSize.cy)
			nHeight = subSize.cy;
	}
	m_rcBaseArea.bottom = nHeight; //调整高度
	nWidth = m_rcItem.Width();

	return CSize(nWidth,nHeight);
}

int CTabItem::HitTest(const CPoint& pt, BOOL bShowClose)
{
	CRect rcItem = m_rcItem;
	rcItem.OffsetRect(m_ptTopLeft);
	if(rcItem.PtInRect(pt))
	{
		if(bShowClose)
		{
			if((pt.x >= rcItem.left) && (pt.x <= (rcItem.left + ICON_SIZE)))
				return HIT_TEST_CLOSEBTN;
		}

		CRect rcText = m_rcBaseArea;
		rcText.OffsetRect(m_ptTopLeft);
		if(rcText.PtInRect(pt))
			return HIT_TEST_TEXTAREA;

		CRect rcSubItem;
		rcSubItem.top = rcText.top;
		rcSubItem.bottom = rcText.bottom;
		rcSubItem.left = rcText.right;
		for(int i = 0; i < GetSubItemCount(); i++)
		{
			rcSubItem.right = rcSubItem.left + m_arSubItems[i].m_nWidth;
			if(rcSubItem.PtInRect(pt))
				return (HIT_TEST_SUBITEMSTART + i);
			rcSubItem.left = rcSubItem.left + m_arSubItems[i].m_nWidth;
		}

		ASSERT(FALSE);
	}

	return HIT_TEST_NONE;
}

int CTabItem::GetSubItemIndexById(LONG nSubItemId)
{
	for(int i = 0; i < GetSubItemCount(); i++)
	{
		if(m_arSubItems[i].m_lSubItemId == nSubItemId)
			return i;
	}

	return -1;
}

CTabSubItem* CTabItem::GetSubItemByIndex(int index)
{
	if((index < 0) || (index >= GetSubItemCount()))
	{
		return NULL;
	}

	return &m_arSubItems[index];
}

LONG CTabItem::GetSubItemId(int index)
{
	if((index < 0) || (index >= GetSubItemCount()))
	{
		return -1;
	}

	return m_arSubItems[index].m_lSubItemId;
}

int CTabItem::GetConceivableActiveSubItemIndex()
{
	if(m_nActiveSubTab >= 0)
		return m_nActiveSubTab;
	if(m_nLastActiveSubTab >= 0)
		return m_nLastActiveSubTab;

	return 0;
}

LONG CTabItem::GetConceivableActiveSubItemId()
{
	if(m_nActiveSubTab >= 0)
		return m_arSubItems[m_nActiveSubTab].m_lSubItemId;
	if(m_nLastActiveSubTab >= 0)
		return m_arSubItems[m_nLastActiveSubTab].m_lSubItemId;

	return m_arSubItems[0].m_lSubItemId; /*默认是第一个*/
}

TAB_SUB_ITEM_STATE CTabItem::GetSubItemStateByIndex(int index)
{
	if((index < 0) || (index >= GetSubItemCount()))
	{
		return SUB_NORMAL_STATE;
	}

    if(m_nHoverSubTab == index)
    {
        return SUB_HOVER_STATE;
    }

    if(m_nActiveSubTab == index)
    {
        return SUB_ACTIVE_STATE;
    }
    
    if(m_nLastActiveSubTab == index)
    {
        return SUB_LAST_ACTIVE_STATE;
    }

    return SUB_NORMAL_STATE;
}

BOOL CTabItem::RemoveSubItemByIndex(int index)
{
	if((index < 0) || (index >= GetSubItemCount()))
	{
		return FALSE;
	}

	m_arSubItems.RemoveAt(index);
	if(GetSubItemCount() > 0) /*这个item还有其它子项*/
	{
        if(m_nActiveSubTab != -1) /*当前激活的item，要保证有效的m_nActiveSubTab*/
        {
            if(m_nActiveSubTab == index) /*是活动的子项*/
            {
                m_nActiveSubTab = 0;    /*默认使用第一个*/
                m_nLastActiveSubTab = -1;
            }
            else
            {
                if(m_nActiveSubTab > index)
                    m_nActiveSubTab--;
                if((m_nLastActiveSubTab > 0) && (m_nLastActiveSubTab >= index))
                    m_nLastActiveSubTab--;
            }
        }
        else /*当前非激活的item，m_nActiveSubTab必须无效，但是m_nLastActiveSubTab需要指定*/
        {
            if((m_nLastActiveSubTab > 0) && (m_nLastActiveSubTab >= index))
                m_nLastActiveSubTab--;
            else
                m_nLastActiveSubTab = 0;
        }
	}

	return TRUE;
}

  
