#include "stdafx.h"
#include "WzTabGlobal.h"
#include "WzTabSubItem.h"

CTabSubItem::CTabSubItem()
{
    m_nIconIdx = -1;
    m_nWidth = 0;
    m_nHeight = 0;
    lpData = NULL;
}

CSize CTabSubItem::CalcSubItemSize(CDC *pDC, BOOL bShowIcon)
{
    m_nWidth = 0;
    m_nHeight = DEFAULT_ITEM_HEIGHT;
    
    if(!m_strSubItemName.IsEmpty())
    {
        CRect rcTextBox = CRect(0,0,0,0);
        int nTextHeight = pDC->DrawText(m_strSubItemName,&rcTextBox,DT_CALCRECT|DT_LEFT|DT_SINGLELINE|DT_VCENTER);
        nTextHeight *= 2;
        if((nTextHeight + 2 * BORDER_MARGIN) < DEFAULT_ITEM_HEIGHT)
            m_nHeight = DEFAULT_ITEM_HEIGHT;
        else 
            m_nHeight = nTextHeight + 2 * BORDER_MARGIN;
        
        m_nWidth += rcTextBox.Width() + SUB_TEXT_AREA_PAD;
    }

    if((m_nIconIdx != -1) && bShowIcon)
    {
        m_nWidth += (ICON_AREA_SIZE + SUB_ICON_AREA_PAD);
    }
    
    return CSize(m_nWidth,m_nHeight);
}


