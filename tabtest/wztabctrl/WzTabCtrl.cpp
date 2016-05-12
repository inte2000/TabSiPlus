// WzTabCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "WzTabCtrl.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CWzTabCtrl

CWzTabCtrl::CWzTabCtrl()
{
	m_nCurActive = -1;
	m_nLastActive = -1;
	m_nHover = -1;
	m_nLastHover = -1;

	m_bGroupSameName = FALSE;
	m_bShowCloseBtn = FALSE;
	m_pNotifyWnd = NULL;
    m_pTheme = NULL;

	m_nWndWidth = 0;
	m_nRowCount = 1;
	m_bTabPosChange = TRUE;
	m_lCurAvailItemId = 0;
}

CWzTabCtrl::~CWzTabCtrl()
{
	FreeAllItems();
}

BEGIN_MESSAGE_MAP(CWzTabCtrl, CWnd)
	//{{AFX_MSG_MAP(CWzTabCtrl)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_RBUTTONDOWN()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
    ON_WM_RBUTTONUP()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX(TTN_NEEDTEXTA, 0, OnToolNeedText)
	ON_NOTIFY_EX(TTN_NEEDTEXTW, 0, OnToolNeedText)
END_MESSAGE_MAP()

BOOL CWzTabCtrl::CreateTabCtrl(CWnd *pParent)
{
	LPCTSTR lpszClassName = AfxRegisterWndClass(CS_DBLCLKS|CS_VREDRAW|CS_HREDRAW,::LoadCursor(NULL, IDC_ARROW),NULL,NULL);
	return Create(lpszClassName,_T(""),WS_CHILD|WS_VISIBLE,CRect(0,0,0,0),pParent,1001,NULL);
}

void CWzTabCtrl::SetGroupSameName(BOOL bGroup)
{
	if(m_bGroupSameName != bGroup)
	{
		m_bGroupSameName = bGroup;
		int nCount = ReGroupAllItems(m_bGroupSameName);
		SendEventNotify(EVENT_GROUP_MODE,(WPARAM)m_bGroupSameName,(LPARAM)nCount);
		if(GetSafeHwnd())
		{
			CheckRowCountChange();
		}
	}
}

void CWzTabCtrl::SetShowItemIcon(BOOL bShow)
{
	if(m_bShowItemIcon != bShow)
	{
		m_bShowItemIcon = bShow;
		if(GetSafeHwnd())
		{
			CheckRowCountChange();
		}
	}
}

void CWzTabCtrl::SetShowCloseBtn(BOOL bShow)
{
	if(m_bShowCloseBtn != bShow)
	{
		m_bShowCloseBtn = bShow;
		if(GetSafeHwnd())
		{
			CheckRowCountChange();
		}
	}
}

CWzTabTheme *CWzTabCtrl::SetTabTheme(CWzTabTheme *theme, BOOL bUpdate)
{
    CWzTabTheme *pOldTheme = m_pTheme;
    m_pTheme = theme;
    if(bUpdate && GetSafeHwnd())
    {
    	CheckRowCountChange();
        Invalidate();
    }
    return pOldTheme;
}

LONG CWzTabCtrl::GetCurSel()
{
	return GetCurrentActiveSubItemId(m_nCurActive);
}

BOOL CWzTabCtrl::SetCurSel(LONG nItemId)
{
	int TabIdx, SubIdx;
	if(FindItemByItemId(nItemId, TabIdx, SubIdx))
	{
		HighlightItemWithoutNotify(TabIdx,SubIdx);
		return TRUE;
	}

	return FALSE;
}

BOOL CWzTabCtrl::GetItemSize(LONG nItem, CSize& size)
{
	int TabIdx, SubIdx;
	if(FindItemByItemId(nItem, TabIdx, SubIdx))
	{
		CTabItem *pItem = m_arItems[TabIdx];
		ASSERT(pItem);
		
		size.cx = pItem->m_rcItem.Width();
		size.cy = pItem->m_rcItem.Height();

		return TRUE;
	}
	
	return FALSE;
}

BOOL CWzTabCtrl::GetItemRect(LONG nItem, CRect& rect)
{
	int TabIdx, SubIdx;
	if(FindItemByItemId(nItem, TabIdx, SubIdx))
	{
		CTabItem *pItem = m_arItems[TabIdx];
		ASSERT(pItem);
		
		rect.left = pItem->m_ptTopLeft.x;
		rect.top = pItem->m_ptTopLeft.y;
		rect.right = rect.left + pItem->m_rcItem.Width();
		rect.bottom = rect.top + pItem->m_rcItem.Height();

		return TRUE;
	}
	
	return FALSE;
}

LPARAM CWzTabCtrl::GetItemData(LONG nItem)
{
	int TabIdx, SubIdx;
	if(FindItemByItemId(nItem, TabIdx, SubIdx))
	{
		CTabItem *pItem = m_arItems[TabIdx];
		ASSERT(pItem);

		CTabSubItem *pSubItem = pItem->GetSubItemByIndex(SubIdx);
		if(pSubItem != NULL)
		{
			return pSubItem->lpData;
		}
	}
	
	return NULL;
}

BOOL CWzTabCtrl::SetItemData(LONG nItem, DWORD dwData)
{
	int TabIdx, SubIdx;
	if(FindItemByItemId(nItem, TabIdx, SubIdx))
	{
		CTabItem *pItem = m_arItems[TabIdx];
		ASSERT(pItem);

		CTabSubItem *pSubItem = pItem->GetSubItemByIndex(SubIdx);
		if(pSubItem != NULL)
		{
			pSubItem->lpData = (LPARAM)dwData;
            return TRUE;
		}
	}
	
	return FALSE;
}

CSize CWzTabCtrl::ReCalcAllTabsPosAndWndSize(int nWndCx)
{
    ASSERT(m_pTheme != NULL);
	m_nWndWidth = nWndCx;
	int nWndHeight = DEFAULT_ITEM_HEIGHT;
	m_nRowCount = 1;
	CSize szPos;

	CWnd *pDeskTopWnd = CWnd::GetDesktopWindow();
	CDC *pDC = pDeskTopWnd->GetDC();
	if(pDC)
	{
		CDC memDC;
		if(memDC.CreateCompatibleDC(pDC))
		{
			memDC.SelectObject(m_pTheme->GetTextFont());
			szPos = InternalCalcPos(m_nWndWidth,&memDC);
			nWndHeight = szPos.cx;
			m_nRowCount = szPos.cy;
			memDC.DeleteDC();
		}
		
		pDeskTopWnd->ReleaseDC(pDC);
	}

	return CSize(m_nWndWidth,nWndHeight);
}

CSize CWzTabCtrl::InternalReCalcAllTabsPos(int nWidth)
{
	ASSERT(GetSafeHwnd());
	CSize size;

	if(nWidth == 0)
	{
		CRect rcClient;
		GetClientRect(rcClient);
		nWidth = rcClient.Width();
	}
	size = ReCalcAllTabsPosAndWndSize(nWidth);
	TRACE(_T("InternalReCalcAllTabsPos, m_bTabPosChange = %d\r\n"),m_bTabPosChange);
	RedrawWindow();

	return size;
}

CSize CWzTabCtrl::InternalCalcPos(int nWndCx, CDC *pDC)
{
	int nItemHeight = 0;
	int nWndHeight = 0;
	int nRowCount = 1;
	int nCurLeft = 0;//BORDER_MARGIN;
	int nCurTop = 0;//BORDER_MARGIN;
	CPoint ptTmp;

	int nTabItemCount = m_arItems.GetSize();
	if(nTabItemCount <= 0)
	{
		return CSize(DEFAULT_ROW_HEIGHT,1);
	}

	BOOL bFirstCol = TRUE;
	int i = 0;
	while(i < nTabItemCount)
	{
		CSize ItemSize = m_arItems[i]->CalcItemSize(pDC, m_bShowCloseBtn, m_bShowItemIcon);
		if(nItemHeight < ItemSize.cy)
			nItemHeight = ItemSize.cy;
		int nCurRight = nCurLeft + ItemSize.cx;
		if(nCurRight > nWndCx)/*到这这一行的结尾了*/
		{
			if(bFirstCol)/*如果是第一列，说明窗口宽度太小了，只能这样显示了*/
			{
				ptTmp.x = nCurLeft;
				ptTmp.y = nCurTop;
				if(m_arItems[i]->m_ptTopLeft != ptTmp)
				{
					m_arItems[i]->m_ptTopLeft = ptTmp;
					m_bTabPosChange = TRUE;
				}
				nCurLeft = 0;//BORDER_MARGIN;

				nCurTop += nItemHeight;
				//nCurTop += 1;
				if(i != (nTabItemCount - 1))/*如果是最后一个就不增加行了*/
					nRowCount++;
				i++;
			}
			else
			{
				bFirstCol = TRUE;
				nCurLeft = 0;//BORDER_MARGIN;

				nCurTop += nItemHeight;
				//nCurTop += 1;
				nRowCount++;

				continue;
			}
		}
		else
		{
			bFirstCol = FALSE;
			ptTmp.x = nCurLeft;
			ptTmp.y = nCurTop;
			if(m_arItems[i]->m_ptTopLeft != ptTmp)
			{
				m_arItems[i]->m_ptTopLeft = ptTmp;
				m_bTabPosChange = TRUE;
			}
/*
			m_arItems[i]->m_ptTopLeft.x = nCurLeft;
			m_arItems[i]->m_ptTopLeft.y = nCurTop;
*/
			nCurLeft = nCurRight + 1;
			i++;
		}
	}
	
	//nWndHeight = DEFAULT_ROW_HEIGHT * nRowCount;
	nWndHeight = nItemHeight * nRowCount;

	return CSize(nWndHeight,nRowCount);
}

int CWzTabCtrl::GetItemCount()
{ 
	int ntotalItems = 0;
	for(int i = 0; i < m_arItems.GetSize(); i++)
	{
		CTabItem *pItem = m_arItems[i];
		ASSERT(pItem);
		ntotalItems += pItem->GetSubItemCount();
	}

	return ntotalItems;
}

int CWzTabCtrl::GetGroupCount()
{
	return m_arItems.GetSize();
}

int CWzTabCtrl::GetSubItemCount(int groupIdx)
{
	if( (groupIdx < 0) || (groupIdx >= m_arItems.GetSize()) )
	{
		return 0;
	}

	CTabItem *pItem = m_arItems[groupIdx];
	ASSERT(pItem);
	return pItem->GetSubItemCount();
}

LPARAM CWzTabCtrl::GetItemDataByIndex(int groupIdx, int subIdx)
{
	if( (groupIdx < 0) || (groupIdx >= m_arItems.GetSize()) )
	{
		return NULL;
	}
	CTabItem *pItem = m_arItems[groupIdx];
	ASSERT(pItem);
	
	CTabSubItem *psubItem = pItem->GetSubItemByIndex(subIdx);
	if(psubItem == NULL)
	{
		return NULL;
	}

	return psubItem->lpData;
}

LONG CWzTabCtrl::GetItemIdByIndex(int groupIdx, int subIdx)
{
	if( (groupIdx < 0) || (groupIdx >= m_arItems.GetSize()) )
	{
		return -1;
	}
	CTabItem *pItem = m_arItems[groupIdx];
	ASSERT(pItem);
	
	CTabSubItem *psubItem = pItem->GetSubItemByIndex(subIdx);
	if(psubItem == NULL)
	{
		return -1;
	}

	return psubItem->m_lSubItemId;
}

BOOL CWzTabCtrl::InternalSetItemText(int groupIdx, int subIdx, LPCTSTR lpszItem)
{
	CString strSubItem = _T("");
	CString strItemName = lpszItem;
	int nPos = strItemName.ReverseFind(_T('.'));
	if(nPos >= 0) /*发现扩展名*/
	{
		strSubItem = strItemName.Mid(nPos);
		strItemName = strItemName.Left(nPos);
	}
	
	CTabItem *pItem = m_arItems[groupIdx];
	if(pItem->GetSubItemCount() == 1) /*只有一个，或者是没有分组状态*/
	{
		pItem->m_strItemText = strItemName;
		pItem->m_arSubItems[subIdx].m_strSubItemName = strSubItem;
	}
	else
	{
		if(pItem->m_strItemText.CompareNoCase(strItemName) == 0) /*分组的名称相同，只需要修改subitem名称*/
		{
			pItem->m_arSubItems[subIdx].m_strSubItemName = strSubItem;
		}
		else/*分组状态时不允许将item的主name修改*/
		{
			return FALSE;
		}
	}
	CheckRowCountChange();
	return TRUE;
}

BOOL CWzTabCtrl::SetItemTextByIndex(int groupIdx, int subIdx, LPCTSTR lpszItem)
{
	if( (groupIdx < 0) || (groupIdx > m_arItems.GetSize()) )
	{
		return FALSE;
	}
	CTabItem *pItem = m_arItems[groupIdx];
	ASSERT(pItem);
	if( (subIdx < 0) || (subIdx > pItem->GetSubItemCount()) )
	{
		return FALSE;
	}

	return InternalSetItemText(groupIdx, subIdx, lpszItem);
}

LONG CWzTabCtrl::InsertItem(LONG nItemId, LPCTSTR lpszItem, int nIcon, LPARAM para)
{
	CString strSubItem = _T("");
	CString strItemName = lpszItem;
	int nPos = strItemName.ReverseFind(_T('.'));
	if(nPos >= 0) /*发现扩展名*/
	{
		strSubItem = strItemName.Mid(nPos);
		strItemName = strItemName.Left(nPos);
	}
	
	if(m_bGroupSameName)
	{
		int nItemIdx = FindSameNameItemIndex(strItemName);
		CTabItem *pItem;
		if(IsValidTabIndex(nItemIdx))
		{
			pItem = m_arItems[nItemIdx];

			CTabSubItem subItem;
			subItem.m_strSubItemName = strSubItem;
			subItem.m_lSubItemId = InterlockedIncrement(&m_lCurAvailItemId);
			subItem.m_nIconIdx = nIcon;
			subItem.lpData = para;
			int nSubIdx = pItem->m_arSubItems.Add(subItem);

			//HighlightItem(nItemIdx,nSubIdx); /*包括发送通知消息*/

			/*此处应该重新计算位置*/
			CheckRowCountChange();
			return subItem.m_lSubItemId;
		}
	}
	/*其它情况，要添加一个新的Item*/
    LONG nNewItemId = -1;
    int TabItemIdx, SubItemIdx;
	if(FindItemByItemId(nItemId, TabItemIdx, SubItemIdx))
    {
        nNewItemId = InternalInsertItem(TabItemIdx, strItemName, strSubItem, nIcon, para);
    }
    else
    {
        nNewItemId = InternalInsertItem(0, strItemName, strSubItem, nIcon, para);
    }

    if(nNewItemId != -1)
    {
	    /*此处应该重新计算位置*/
		CheckRowCountChange();
    }

	return nNewItemId;
}

LONG CWzTabCtrl::InternalInsertItem(int nItemIdx, LPCTSTR lpszItem, LPCTSTR lpszSubItem, int nIcon, LPARAM para)
{
	CTabItem *pItem = new CTabItem();
	if(!pItem)
		return -1;

	CTabSubItem subItem;
	subItem.m_strSubItemName = lpszSubItem;
	subItem.m_lSubItemId = InterlockedIncrement(&m_lCurAvailItemId);
	subItem.m_nIconIdx = nIcon;
	subItem.lpData = para;

	pItem->m_strItemText = lpszItem;
	pItem->m_arSubItems.Add(subItem);

    if(nItemIdx >= m_arItems.GetCount())
        nItemIdx = m_arItems.GetCount();

    m_arItems.InsertAt(nItemIdx, pItem);

    int TabItemIdx, SubItemIdx;
	if(FindItemByItemId(subItem.m_lSubItemId, TabItemIdx, SubItemIdx)) /*找到刚才添加的item*/
    {
	    if((m_nLastActive != -1) && (m_nLastActive >= nItemIdx)) /*fix bug!!!*/
		    m_nLastActive++;
	    if((m_nCurActive != -1) && (m_nCurActive >= nItemIdx)) /*fix bug!!!*/
		    m_nCurActive++;

	    //HighlightItem(TabItemIdx, SubItemIdx); /*包括发送通知消息*/
        return subItem.m_lSubItemId;
    }

    return -1;
}

BOOL CWzTabCtrl::SetItemText(LONG nItemId, LPCTSTR lpszItem)
{
	int tabIdx, subIdx;
	if(FindItemByItemId(nItemId, tabIdx, subIdx))
	{
		return InternalSetItemText(tabIdx, subIdx, lpszItem);
	}

	return FALSE;
}

BOOL CWzTabCtrl::DeleteItem(LONG nItemId)
{
	int TabIdx, SubIdx;
	if(FindItemByItemId(nItemId, TabIdx, SubIdx))
	{
		InternalDeleteItem(TabIdx, SubIdx);
		return TRUE;
	}

	return FALSE;
}

void CWzTabCtrl::InternalDeleteItem(int TabItemIdx, int SubItemIdx)
{
	CTabItem *pItem = m_arItems[TabItemIdx];
	ASSERT(pItem);

	if(TabItemIdx == m_nCurActive) /*删除的item是当前激活的item*/
	{
		if(GetCurrentActiveSubItemId(m_nCurActive) == GetSubItemId(TabItemIdx, SubItemIdx))/*是否是当前激活的子项？*/
		{
			//SendEventNotify(EVENT_ITEM_STATE_CHANGE,GetSubItemId(TabItemIdx, SubItemIdx),FALSE);/*去激活这个子项*/
		}

		pItem->RemoveSubItemByIndex(SubItemIdx);
		if(pItem->GetSubItemCount() > 0) /*这个item还有其它子项*/
		{
			int subActIdx = pItem->GetConceivableActiveSubItemIndex();
			SetActiveSubItemIndex(TabItemIdx, subActIdx);
			//SendEventNotify(EVENT_ITEM_STATE_CHANGE, GetSubItemId(TabItemIdx, subActIdx),TRUE);/*激活新的子项*/
		}
	}
	else
	{
		//SendEventNotify(EVENT_ITEM_DELETE,GetSubItemId(TabItemIdx, SubItemIdx),0);
		pItem->RemoveSubItemByIndex(SubItemIdx);
	}

	if(pItem->GetSubItemCount() <= 0)/*这个tabitem即将被删除，需要将激活tabitem和子项调整到其它tabitem上*/
	{
		if(m_arItems.GetSize() == 1) /*已经是最后一个item了*/
		{
			m_nCurActive = -1;
			m_nLastActive = -1;
		}
		else
		{
			if(TabItemIdx == m_nCurActive) /*当前是激活的tabitem*/
			{
				if(m_nLastActive == -1)
					m_nLastActive = 0;

				if(m_nLastActive == m_nCurActive)
					m_nLastActive = (m_nCurActive != 0) ? 0 : m_arItems.GetSize() - 1;

				/*
				  当前激活的item要被删除，需要激活上一个激活的item。注意此时item还没有从list中删除，
                  标签的位置还不能调整
				*/
				int subActIdx = m_arItems[m_nLastActive]->GetConceivableActiveSubItemIndex();
				SetActiveSubItemIndex(m_nLastActive, subActIdx);
				//SendEventNotify(EVENT_ITEM_STATE_CHANGE,GetSubItemId(m_nLastActive,subActIdx),TRUE);

				if(m_nLastActive > TabItemIdx)
					m_nLastActive--;

				m_nCurActive = m_nLastActive;
				m_nLastActive = (m_nCurActive > 1) ? m_nCurActive - 1 : 0;
			}
			else if(TabItemIdx == m_nLastActive) /*当前是上一个激活项*/
			{
				/*假设成第一项，或者是当前m_nLastActive的前一项。因为此处能告保证还有至少一个tabitem*/
				m_nLastActive = (m_nLastActive == 0) ? 0 : m_nLastActive - 1;
				if(m_nCurActive > TabItemIdx)
					m_nCurActive--;
			}
			else /*什么都不是，但是删除此项可能需要调整m_nCurActive和m_nLastActive*/
			{
				if(m_nLastActive > TabItemIdx)
					m_nLastActive--;
				if(m_nCurActive > TabItemIdx)
					m_nCurActive--;
			}
		}
		
		m_arItems.RemoveAt(TabItemIdx);
		delete pItem;	
	}

	m_nLastHover = m_nHover = -1;

	CheckRowCountChange();
}

BOOL CWzTabCtrl::InternalHitTest(const CPoint& point, int& TabIdx, int& SubIdx) const
{
	int nCount = m_arItems.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		int nHit = m_arItems[i]->HitTest(point,m_bShowCloseBtn);
		if((nHit == HIT_TEST_TEXTAREA) || (nHit == HIT_TEST_CLOSEBTN))
		{
			TabIdx = i;
			SubIdx = m_arItems[i]->GetConceivableActiveSubItemIndex();
			return TRUE;
		}
		if(nHit >= HIT_TEST_SUBITEMSTART)
		{
			TabIdx = i;
			SubIdx = nHit - HIT_TEST_SUBITEMSTART;
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CWzTabCtrl::DeleteAllItems()
{
	int nSize = m_arItems.GetSize();
	if(nSize <= 0)
		return TRUE;

	for(int i = 0; i < nSize; i++)
	{
		CTabItem *pItem = m_arItems[i];
		ASSERT(pItem);
		DeleteTabGroup(i);
		delete pItem;
	}

	m_arItems.RemoveAll();
	
	m_nLastHover = m_nHover = -1;
	m_nLastActive = m_nCurActive = -1;

	//更新窗口显示
	CheckRowCountChange();

	return TRUE;
}

BOOL CWzTabCtrl::DeleteGroup(int nGroupIdx)
{
	BOOL bSuccess = FALSE;
	if(IsValidTabIndex(nGroupIdx))
	{
		bSuccess = DeleteTabGroup(nGroupIdx);
		if(bSuccess)
		{
			CTabItem *pItem = m_arItems[nGroupIdx];
			ASSERT(pItem);
			m_arItems.RemoveAt(nGroupIdx);
			delete pItem;

			if(m_arItems.GetSize() > 0)
			{
				if(nGroupIdx == m_nCurActive)
				{
					if(m_nLastActive == -1)
						m_nLastActive = 0;

					if(m_nLastActive > nGroupIdx)
						m_nLastActive--;

					m_nCurActive = m_nLastActive;
					m_nLastActive = (m_nCurActive > 1) ? m_nCurActive - 1 : m_arItems.GetSize() - 1;
					SendEventNotify(EVENT_ITEM_STATE_CHANGE,m_arItems[m_nCurActive]->GetConceivableActiveSubItemId(),TRUE);
				}
				else if(nGroupIdx == m_nLastActive)
				{
					if(m_nCurActive > nGroupIdx)
						m_nCurActive--;

					m_nLastActive = (nGroupIdx > 1) ? nGroupIdx - 1 : 0;
				}
				else
				{
					if(m_nCurActive > nGroupIdx)
						m_nCurActive--;
					if(m_nLastActive > nGroupIdx)
						m_nLastActive--;
				}
			}
			else
			{
				m_nCurActive = m_nLastActive = -1;
			}

			m_nHover = m_nLastHover = -1;
			CheckRowCountChange();
		}
	}

	return bSuccess;
}

BOOL CWzTabCtrl::IsValidTabIndex(int nTabIdx) const
{
    if((nTabIdx >= 0) && (nTabIdx < m_arItems.GetSize()))
		return TRUE;

	return FALSE;
}

LONG CWzTabCtrl::GetCurrentActiveSubItemId(int TabItemIdx)
{
	if(IsValidTabIndex(TabItemIdx))
	{
		CTabItem *pItem = m_arItems[TabItemIdx];
		int nActiveSubTab = pItem->m_nActiveSubTab;
		ASSERT((nActiveSubTab >= 0) && (nActiveSubTab < pItem->m_arSubItems.GetSize()));
		if((nActiveSubTab >= 0) && (nActiveSubTab < pItem->m_arSubItems.GetSize()))
			return pItem->m_arSubItems[nActiveSubTab].m_lSubItemId;
	}

	return -1;
}

int CWzTabCtrl::GetCurrentActiveSubItemIndex(int TabItemIdx)
{
	if(IsValidTabIndex(TabItemIdx))
	{
		CTabItem *pItem = m_arItems[TabItemIdx];
		ASSERT((pItem->m_nActiveSubTab >= 0) && (pItem->m_nActiveSubTab < pItem->m_arSubItems.GetSize()));
		return pItem->m_nActiveSubTab;
	}

	return -1;
}

void CWzTabCtrl::SetActiveSubItemIndex(int TabItemIdx, int SubItemIdx)
{
	if(IsValidTabIndex(TabItemIdx))
	{
		m_arItems[TabItemIdx]->m_nLastActiveSubTab = m_arItems[TabItemIdx]->m_nActiveSubTab;
		m_arItems[TabItemIdx]->m_nActiveSubTab = SubItemIdx;
	}
}

void CWzTabCtrl::ResetActiveSubItemIndex(int TabItemIdx, int SubItemIdx)
{
	if(IsValidTabIndex(TabItemIdx))
	{
		m_arItems[TabItemIdx]->m_nLastActiveSubTab = m_arItems[TabItemIdx]->m_nActiveSubTab;
		m_arItems[TabItemIdx]->m_nActiveSubTab = -1;
	}
}


LONG CWzTabCtrl::GetSubItemId(int TabItemIdx, int SubItemIdx) const
{
	if(IsValidTabIndex(TabItemIdx))
	{
		CTabItem *pItem = m_arItems[TabItemIdx];
		CTabSubItem* pSubItem = pItem->GetSubItemByIndex(SubItemIdx);
		if(pSubItem != NULL)
			return pSubItem->m_lSubItemId;
	}

	return -1;
}

int CWzTabCtrl::GetFirstInactiveSubItemIndex(int TabItemIdx)
{
	int nIdx = -1;
	if(IsValidTabIndex(TabItemIdx))
	{
		for(int i = 0; i < m_arItems[TabItemIdx]->GetSubItemCount(); i++)
		{
			if(i != m_arItems[TabItemIdx]->m_nActiveSubTab)
			{
				nIdx = i;
				break;
			}
		}
	}

	return nIdx;
}

BOOL CWzTabCtrl::HighlightItem(int TabItemIdx, int SubItemIdx)
{
	int curSubActItemIdx = GetCurrentActiveSubItemIndex(m_nCurActive);
	if((TabItemIdx == m_nCurActive) && (SubItemIdx == curSubActItemIdx))
		return TRUE;

	if(TabItemIdx == m_nCurActive)
	{
		SendEventNotify(EVENT_ITEM_STATE_CHANGE,GetSubItemId(m_nCurActive, curSubActItemIdx),FALSE);
		SetActiveSubItemIndex(TabItemIdx, SubItemIdx);
		SendEventNotify(EVENT_ITEM_STATE_CHANGE,GetSubItemId(TabItemIdx,SubItemIdx),TRUE);
	}
	else
	{
		SendEventNotify(EVENT_ITEM_STATE_CHANGE,GetSubItemId(m_nCurActive, curSubActItemIdx),FALSE);
		ResetActiveSubItemIndex(m_nCurActive, curSubActItemIdx);
		m_nLastActive = m_nCurActive;
		m_nCurActive = TabItemIdx;
		SetActiveSubItemIndex(TabItemIdx, SubItemIdx);
		SendEventNotify(EVENT_ITEM_STATE_CHANGE,GetSubItemId(TabItemIdx, SubItemIdx),TRUE);
	}
	Invalidate();
	return TRUE;
}

BOOL CWzTabCtrl::HighlightItemWithoutNotify(int TabItemIdx, int SubItemIdx)
{
	int curSubActItemIdx = GetCurrentActiveSubItemIndex(m_nCurActive);
	if((TabItemIdx == m_nCurActive) && (SubItemIdx == curSubActItemIdx))
		return TRUE;

	if(TabItemIdx == m_nCurActive)
	{
		SetActiveSubItemIndex(TabItemIdx, SubItemIdx);
	}
	else
	{
		ResetActiveSubItemIndex(m_nCurActive, curSubActItemIdx);
		m_nLastActive = m_nCurActive;
		m_nCurActive = TabItemIdx;
		SetActiveSubItemIndex(TabItemIdx, SubItemIdx);
	}
	Invalidate();
	return TRUE;
}

BOOL CWzTabCtrl::DeleteTabGroup(int TabIdx)
{
	if(IsValidTabIndex(TabIdx))
	{
		CTabItem *pItem = m_arItems[TabIdx];
		ASSERT(pItem);
		for(int i = 0; i < pItem->GetSubItemCount(); i++)
		{
			//SendEventNotify(EVENT_ITEM_DELETE,GetSubItemId(TabIdx,i),0);
		}
		pItem->m_arSubItems.RemoveAll();

		return TRUE;
	}

	return FALSE;
}

void CWzTabCtrl::FreeAllItems()
{
	int nSize = m_arItems.GetSize();
	for(int i = 0; i < nSize; i++)
	{
		CTabItem *pItem = m_arItems[i];
		ASSERT(pItem);
		delete pItem;
	}

	m_arItems.RemoveAll();
}

BOOL CWzTabCtrl::FindItemByItemId(LONG uItemId, int& TabIdx, int& SubIdx)
{
	TabIdx = -1;
	SubIdx = -1;
	int nSize = m_arItems.GetSize();
	for(int i = 0; i < nSize; i++)
	{
		CTabItem *pItem = m_arItems[i];
		ASSERT(pItem);

		SubIdx = pItem->GetSubItemIndexById(uItemId);
		if(SubIdx >= 0)
		{
			TabIdx = i;
			return TRUE;
		}
	}

	return FALSE;
}

CTabItem *CWzTabCtrl::FindSameNameItem(LPCTSTR lpszName)
{
	int nIndex = FindSameNameItemIndex(lpszName);
	if(IsValidTabIndex(nIndex))
	{
		return m_arItems[nIndex];
	}

	return NULL;
}

int CWzTabCtrl::FindSameNameItemIndex(LPCTSTR lpszName)
{
	int nIndex = -1;
	int nSize = m_arItems.GetSize();
	for(int i = 0; i < nSize; i++)
	{
		CTabItem *pItem = m_arItems[i];
		ASSERT(pItem);
		if(pItem->m_strItemText.CompareNoCase(lpszName) == 0)
		{
			nIndex = i;
			break;
		}
	}

	return nIndex;
}

int CWzTabCtrl::ReGroupAllItems(BOOL bGroup)
{
	int nTotalCount = m_arItems.GetSize();
	if(nTotalCount <= 0)
		return 0;

	LONG nCurActiveSubId = GetCurrentActiveSubItemId(m_nCurActive);
	m_nLastHover = m_nHover = -1;
	m_nLastActive = m_nCurActive = -1;
	
	if(bGroup)/*合并组*/
	{
		int nCompareFirst = 0;
		while(nCompareFirst < (nTotalCount - 1))
		{
			int i = nCompareFirst + 1;
			while(i < nTotalCount)
			{
				if(m_arItems[nCompareFirst]->m_strItemText == m_arItems[i]->m_strItemText)
				{
					BOOL bCombin = CombineItem(m_arItems[nCompareFirst],m_arItems[i]);
					if(bCombin)
					{
						CTabItem *pItem = m_arItems[i];
						m_arItems.RemoveAt(i);
						delete pItem;
						nTotalCount--;
					}
				}
				else
					i++;
			}

			nTotalCount = m_arItems.GetSize();
			nCompareFirst++;
		}
	}
	else/*拆分组*/
	{
		int nSplitFirst = 0;
		while(nSplitFirst < nTotalCount)
		{
			CTabItem *pSplitItem = m_arItems[nSplitFirst];
			int nSubCount = pSplitItem->GetSubItemCount();
			
			int i = 1;
			while(i < nSubCount)
			{
				CTabItem *pItem = new CTabItem();
				if(pItem)
				{
					pItem->m_strItemText = pSplitItem->m_strItemText;
					pItem->m_arSubItems.Add(pSplitItem->m_arSubItems[i]);
					//pItem->m_nActiveSubTab = 0;
					//pItem->m_nHoverSubTab = -1;
					m_arItems.InsertAt(nSplitFirst,pItem);
					nSplitFirst++;

					pSplitItem->m_arSubItems.RemoveAt(i);
				}
				else
					i++;

				nSubCount = pSplitItem->GetSubItemCount();
			}

			nSplitFirst++;
			nTotalCount = m_arItems.GetSize();
		}
	}

	/*重新确定激活关系*/
	int nNewCount = m_arItems.GetSize();
	for(int i = 0; i < nNewCount; i++)
	{
		int nSubIdx = m_arItems[i]->GetSubItemIndexById(nCurActiveSubId);
		if(nSubIdx != -1) /*在这个组中*/
		{
			m_nCurActive = i;
            m_nLastActive = -1;
			m_arItems[i]->m_nActiveSubTab = nSubIdx;
            m_arItems[i]->m_nLastActiveSubTab = -1;
		}
		else
        {
			m_arItems[i]->m_nActiveSubTab = -1;
            m_arItems[i]->m_nLastActiveSubTab = 0;
        }
	}

	return nNewCount;
}

BOOL CWzTabCtrl::CombineItem(CTabItem *pItemDest, CTabItem *pItemSrc)
{
	if(pItemDest == NULL || pItemSrc == NULL)
		return FALSE;

	ASSERT(pItemDest->m_strItemText == pItemSrc->m_strItemText);
	if(pItemDest->m_strItemText != pItemSrc->m_strItemText)
		return FALSE;

	int nSubCount = pItemSrc->GetSubItemCount();
	for(int i = 0; i < nSubCount; i++)
	{
		pItemDest->m_arSubItems.Add(pItemSrc->m_arSubItems[i]);
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CWzTabCtrl message handlers

BOOL CWzTabCtrl::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::DestroyWindow();
}

BOOL CWzTabCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::PreCreateWindow(cs);
}

void CWzTabCtrl::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CWnd::PreSubclassWindow();
}

BOOL CWzTabCtrl::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::PreTranslateMessage(pMsg);
}

int CWzTabCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	EnableToolTips(TRUE);
	
	return 0;
}

void CWzTabCtrl::OnDestroy() 
{
	CWnd::OnDestroy();
}

BOOL CWzTabCtrl::OnEraseBkgnd(CDC* pDC) 
{
	TRACE(_T("OnEraseBkgnd\r\n"));
	return TRUE;
}

void CWzTabCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int nCount = m_arItems.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		int nHit = m_arItems[i]->HitTest(point,m_bShowCloseBtn);
		if(nHit != HIT_TEST_NONE)
		{
			if(nHit == HIT_TEST_TEXTAREA)
			{
				SendEventNotify(EVENT_ITEM_L_DBCLICK,MAKELONG(point.x,point.y),m_arItems[i]->GetConceivableActiveSubItemId());
			}
			else if(nHit >= HIT_TEST_SUBITEMSTART)
			{
				int nSubIdx = nHit - HIT_TEST_SUBITEMSTART;
				SendEventNotify(EVENT_ITEM_L_DBCLICK,MAKELONG(point.x,point.y),GetSubItemId(i,nSubIdx));
			}

			break;
		}
	}

	CWnd::OnLButtonDblClk(nFlags, point);
}

void CWzTabCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	BOOL bHited = FALSE;
	int nCount = m_arItems.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		int nHit = m_arItems[i]->HitTest(point,m_bShowCloseBtn);
		if(nHit != HIT_TEST_NONE)
		{
			bHited = TRUE;
			m_arItems[i]->m_nLastState |= STATE_MOUSE_L_DOWN;
			CClientDC dc(this);
			
			DrawTabItem(&dc, m_arItems[i], i);
		}
	}	

	CWnd::OnLButtonDown(nFlags, point);
}

void CWzTabCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	BOOL bHited = FALSE;
	CClientDC dc(this);
	
	int nCount = m_arItems.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		int nHit = m_arItems[i]->HitTest(point,m_bShowCloseBtn);
		if(nHit != HIT_TEST_NONE)
		{
			bHited = TRUE;
			m_nHover = i;
			
			if(m_nLastHover != m_nHover)
			{
				int nLastHover = m_nLastHover;
				m_nLastHover = m_nHover;
				
				if(nLastHover != -1)
					m_arItems[nLastHover]->m_nHoverSubTab = -1;
				if(nHit == HIT_TEST_CLOSEBTN)
				{
				}
				else if(nHit == HIT_TEST_TEXTAREA)
				{
				}
				else
				{
					int nSubCount = m_arItems[i]->GetSubItemCount();
					int nHoverSub = nHit - HIT_TEST_SUBITEMSTART;
					if(nHoverSub >= nSubCount)
						m_arItems[i]->m_nHoverSubTab = -1;
					else
						m_arItems[i]->m_nHoverSubTab = nHoverSub;
				}

				if(m_arItems[i]->m_nLastHit != nHit)
				{
					m_arItems[i]->m_nLastHit = nHit;
				}
				//上一个hover 是否有效
				if((nLastHover >= 0) && (nLastHover < m_arItems.GetSize()))
				{
					DrawTabItem(&dc, m_arItems[nLastHover], nLastHover);
				}
				//重画当前hover
				DrawTabItem(&dc, m_arItems[i], i);
			}
			else//是在原来的tab 上，现在确定具体的位置
			{
				if(nHit >= HIT_TEST_SUBITEMSTART)
				{
					if(m_arItems[i]->m_nHoverSubTab != (nHit - HIT_TEST_SUBITEMSTART))
					{
						m_arItems[i]->m_nHoverSubTab = nHit - HIT_TEST_SUBITEMSTART;
						DrawTabItem(&dc, m_arItems[i], i);
					}
				}
				else
				{
					m_arItems[i]->m_nHoverSubTab = -1;
					DrawTabItem(&dc, m_arItems[i], i);
				}

				if(m_arItems[i]->m_nLastHit != nHit)
				{
					m_arItems[i]->m_nLastHit = nHit;
				}
			}
		}

		m_arItems[i]->m_nLastHit = nHit;
	}

	if(bHited)
	{
		TRACKMOUSEEVENT tme = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd};	
		_TrackMouseEvent(&tme);
	}
	else
	{
		m_nHover = -1;
	}

	CWnd::OnMouseMove(nFlags, point);
}

LRESULT CWzTabCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
    if(m_nLastHover != -1)
    {
	    m_arItems[m_nLastHover]->m_nHoverSubTab = -1;
	    m_arItems[m_nLastHover]->m_nLastHit = HIT_TEST_NONE;
    }
	m_nHover = -1;
	m_nLastHover = -1;
	Invalidate();
	
	return 0;
}


void CWzTabCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CClientDC dc(this);

	int nLastDown = -1;
	int nCount = m_arItems.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		CTabItem *pItem = m_arItems[i];
		if((pItem->m_nLastState & STATE_MOUSE_L_DOWN) == STATE_MOUSE_L_DOWN)
		{
			nLastDown = i;
			pItem->m_nLastState &= ~STATE_MOUSE_L_DOWN;
			DrawTabItem(&dc, pItem, i);
		}

		int nHit = pItem->HitTest(point,m_bShowCloseBtn);
		if(nHit != HIT_TEST_NONE)
		{
			if(i == nLastDown) /*按下和松开是在同一个TABITEM上*/
			{
				if(nHit == HIT_TEST_CLOSEBTN)
				{
					SendEventNotify(EVENT_ITEM_DELETE,GetSubItemId(i, pItem->GetConceivableActiveSubItemIndex()),0);
					//InternalDeleteItem(i, pItem->GetConceivableActiveSubItemIndex());
					break;
				}
				else if(nHit == HIT_TEST_TEXTAREA)
				{
					SendEventNotify(EVENT_ITEM_L_CLICK,MAKELONG(point.x,point.y),MAKELONG(i, pItem->GetConceivableActiveSubItemId()));

   					if(m_nCurActive != i)
					{
						int subActItemIdx = GetCurrentActiveSubItemIndex(m_nCurActive);
						ASSERT(subActItemIdx >= 0);
						SendEventNotify(EVENT_ITEM_STATE_CHANGE,GetSubItemId(m_nCurActive, subActItemIdx),FALSE);/*去激活这个子项*/
						ResetActiveSubItemIndex(m_nCurActive, subActItemIdx);
						m_nLastActive = m_nCurActive;
						m_nCurActive = i;
						subActItemIdx = pItem->GetConceivableActiveSubItemIndex();
						SetActiveSubItemIndex(i, subActItemIdx);
						SendEventNotify(EVENT_ITEM_STATE_CHANGE,GetSubItemId(i, subActItemIdx),TRUE);/*激活这个子项*/
					}
				}
				else if(nHit >= HIT_TEST_SUBITEMSTART)
				{
					int nSubIdx = nHit - HIT_TEST_SUBITEMSTART;
					SendEventNotify(EVENT_ITEM_L_CLICK,MAKELONG(point.x,point.y),MAKELONG(i,GetSubItemId(i, nSubIdx)));
					if(m_nCurActive != i)
					{
						int subActItemIdx = GetCurrentActiveSubItemIndex(m_nCurActive);
						SendEventNotify(EVENT_ITEM_STATE_CHANGE,GetSubItemId(m_nCurActive, subActItemIdx),FALSE);/*去激活这个子项*/
						ResetActiveSubItemIndex(m_nCurActive, subActItemIdx);
						m_nLastActive = m_nCurActive;
						m_nCurActive = i;
						SetActiveSubItemIndex(i, nSubIdx);
						SendEventNotify(EVENT_ITEM_STATE_CHANGE,GetSubItemId(i,nSubIdx),TRUE);/*激活这个子项*/
					}
					else
					{
						if(nSubIdx != pItem->m_nActiveSubTab)
						{
							SendEventNotify(EVENT_ITEM_STATE_CHANGE,GetSubItemId(m_nCurActive,pItem->m_nActiveSubTab),FALSE);/*去激活这个子项*/
							SetActiveSubItemIndex(m_nCurActive, nSubIdx);
							SendEventNotify(EVENT_ITEM_STATE_CHANGE,GetSubItemId(m_nCurActive,nSubIdx),TRUE);/*激活这个子项*/
						}
					}
				}
				else
					ASSERT(FALSE);
			}
			DrawTabItem(&dc, pItem, i);
		}
	}	

	Invalidate();

	CWnd::OnLButtonUp(nFlags, point);
}

void CWzTabCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

    ASSERT(m_pTheme != NULL);

	CRect rcClient;
	GetClientRect(rcClient);

	CMemDC pDC(&dc, &rcClient);

	// TODO: Add your message handler code here
    m_pTheme->DrawBkgnd(pDC, rcClient);

	int nCount = m_arItems.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		DrawTabItem(pDC,m_arItems[i],i);
	}

	// Do not call CWnd::OnPaint() for painting messages
}

TAB_ITEM_STATE CWzTabCtrl::GetTabItemState(int nItem)
{
	if(nItem == m_nCurActive)
	    return ACTIVE_STATE;
	else if(nItem == m_nLastActive)
	    return LAST_ACTIVE_STATE;
	else if(nItem == m_nHover)
		return HOVER_STATE;
	else
        return NORMAL_STATE;
}

void CWzTabCtrl::CheckRowCountChange()
{
	CRect rcClient;
	GetClientRect(rcClient);
    if(rcClient.Width() == 0)
        return; 

	int oldRowCount = m_nRowCount;
	CSize size = InternalReCalcAllTabsPos();
	ULONG sf = MAKELONG(size.cx,size.cy);
	if(oldRowCount != m_nRowCount)
	{
		SendEventNotify(EVENT_ROW_COUNT_CHANGE, sf, m_nRowCount);
	}
}

void CWzTabCtrl::DrawTabItem(CDC *pDC, CTabItem *pItem, int nItem)
{
    ASSERT(m_pTheme != NULL);
	if(pItem == NULL)
		return ;

    TAB_ITEM_STATE itemState = GetTabItemState(nItem);
	CRect rcItem = pItem->m_rcItem;
	rcItem.OffsetRect(pItem->m_ptTopLeft);
	
	CRect rcText = rcItem;
    m_pTheme->DrawTabBorder(pDC, rcText, itemState);
    
	rcText.right -= (pItem->m_rcItem.Width() - pItem->m_rcBaseArea.Width());//右边空出subitem的空间
	m_pTheme->DrawTabBkgnd(pDC, rcText, itemState);
	if(m_bShowCloseBtn)
	{
		CRect rcIcon = rcText;
		rcIcon.right = rcIcon.left + ICON_AREA_SIZE;
        m_pTheme->DrawItemCloseBtn(pDC, rcIcon, pItem->m_nLastHit, pItem->m_nLastState);
		rcText.left += (ICON_AREA_SIZE + 1);
	}
	rcText.left += (TEXT_AREA_PAD - 2) / 2;
	m_pTheme->DrawTabText(pDC, rcText, pItem->m_strItemText, itemState);

	CRect rcSubItem = rcText;
	rcSubItem.left = rcText.right;
	for(int i = 0; i < pItem->GetSubItemCount(); i++)
	{
		CTabSubItem& subItem = pItem->m_arSubItems[i];
		rcSubItem.right = rcSubItem.left + subItem.m_nWidth;
		TAB_SUB_ITEM_STATE subState = pItem->GetSubItemStateByIndex(i);
		if(m_bShowItemIcon)
			m_pTheme->DrawSubItem(pDC, subItem.m_nIconIdx, subItem.m_strSubItemName, rcSubItem, itemState, subState);
		else
			m_pTheme->DrawSubItem(pDC, -1, subItem.m_strSubItemName, rcSubItem, itemState, subState);
			
		rcSubItem.left = rcSubItem.left + subItem.m_nWidth;
	}
}

void CWzTabCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CWnd::OnRButtonDown(nFlags, point);
}

BOOL CWzTabCtrl::SendEventNotify(UINT EventCode, ULONG Param1, ULONG Param2)
{
	WZTAB_EVENT_PARA para;
	if(m_pNotifyWnd && m_pNotifyWnd->GetSafeHwnd())
	{
		para.para1 = Param1;
		para.para2 = Param2;
		m_pNotifyWnd->SendMessage(WZTAB_EVENT_NOTIFY,EventCode,(LPARAM)&para);
		return TRUE;
	}

	return FALSE;
}

void CWzTabCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
    BOOL bSendNotify = FALSE;
	int nCount = m_arItems.GetSize();
	for(int i = 0; i < nCount; i++)
	{
		int nHit = m_arItems[i]->HitTest(point,m_bShowCloseBtn);
		if(nHit != HIT_TEST_NONE)
		{
			if(nHit == HIT_TEST_TEXTAREA)
			{
                bSendNotify = TRUE;
				SendEventNotify(EVENT_ITEM_R_CLICK,MAKELONG(point.x,point.y),m_arItems[i]->GetConceivableActiveSubItemId());
			}
			else if(nHit >= HIT_TEST_SUBITEMSTART)
			{
				int nSubIdx = nHit - HIT_TEST_SUBITEMSTART;
                bSendNotify = TRUE;
				SendEventNotify(EVENT_ITEM_R_CLICK,MAKELONG(point.x,point.y),GetSubItemId(i,nSubIdx));
			}

			break;
		}
	}

    if(!bSendNotify)
    {
        CWnd::OnRButtonUp(nFlags, point);
    }
}

INT_PTR CWzTabCtrl::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	TRACE(_T("OnToolHitTest(point=(%d,%d))\n"), point.x, point.y);

    int tabIdx, subIdx;
    if(!InternalHitTest(point, tabIdx, subIdx))
    {
    	return -1;
    }
	
    //Get the client (area occupied by this control)
    RECT rcClient;
    GetClientRect( &rcClient );

    //Fill in the TOOLINFO structure
    pTI->hwnd = m_hWnd;
    pTI->uId = (UINT)GetSubItemId(tabIdx, subIdx);
    // Send TTN_NEEDTEXT when tooltip should be shown
    pTI->lpszText = LPSTR_TEXTCALLBACK;
    pTI->rect = rcClient;

    return pTI->uId;
    //return CWnd::OnToolHitTest(point, pTI);
}

BOOL CWzTabCtrl::GetTooltipText(LONG uItemId, CString& tooltip)
{
    tooltip.Format(_T("Tooltip for item %d"), uItemId);

	return TRUE;
}

BOOL CWzTabCtrl::OnToolNeedText(UINT id, NMHDR* pNMHDR, LRESULT* pResult)
{
	static CString tooltip;
	
	if(!GetTooltipText(pNMHDR->idFrom, tooltip))
	{
		return FALSE;
	}

	// Non-unicode applications can receive requests for tooltip-text in unicode
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
#ifndef _UNICODE
    static wchar_t sToolTip[MAX_PATH] = { 0 };
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		pTTTA->lpszText = LPTSTR(LPCTSTR(tooltip));
		//lstrcpyn(pTTTA->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTA->szText));
	}
	else
	{
		::MultiByteToWideChar(CP_ACP, 0, tooltip, -1, sToolTip, MAX_PATH) ;
		pTTTW->lpszText = (LPWSTR)sToolTip;    
		//mbstowcs(pTTTW->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTW->szText)/sizeof(WCHAR));
	}
#else
    static char sToolTip[MAX_PATH] = { 0 };
	if (pNMHDR->code == TTN_NEEDTEXTA)
	{
		::WideCharToMultiByte(CP_ACP, 0, tooltip, -1, sToolTip, MAX_PATH) ;
		pTTTW->lpszText = (LPTSTR)sToolTip;    
		//_wcstombsz(pTTTA->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTA->szText));
	}
	else
	{
		pTTTW->lpszText = LPTSTR(LPCTSTR(tooltip));
		//lstrcpyn(pTTTW->szText, static_cast<LPCTSTR>(tooltip), sizeof(pTTTW->szText)/sizeof(WCHAR));
	}
#endif
	// If wanting to display a tooltip which is longer than 80 characters,
	// then one must allocate the needed text-buffer instead of using szText,
	// and point the TOOLTIPTEXT::lpszText to this text-buffer.
	// When doing this, then one is required to release this text-buffer again
	return TRUE;
}
