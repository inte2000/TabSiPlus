// TabCheckListBox.cpp : implementation file
// 5/22/98 Modified
// by Qing Zhang (qzhang7@tfn.net)
// If you like this code, just don't remove my name from the source.
/*********************************
*    修改了DrawItem的部分代码，使文字在不同状态下显示不同颜色
*    添加了属性设置函数
*    modify by orbit (inte2000@163.com)
*********************************/

#include "stdafx.h"
#include "TabCheckListBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabCheckListBox

CTabCheckListBox::CTabCheckListBox()
{
    m_lpnEachStop = NULL;
    m_clrChkText = RGB(0,0,255);
    m_clrText = RGB(136,0,136);
    m_clrDisableText = RGB(0x80, 0x80, 0x80);
}

CTabCheckListBox::~CTabCheckListBox()
{
    delete []m_lpnEachStop;
}


BEGIN_MESSAGE_MAP(CTabCheckListBox, CCheckListBox)
    //{{AFX_MSG_MAP(CTabCheckListBox)
        // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabCheckListBox message handlers

void CTabCheckListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    // TODO: Add your code to draw the specified item
    ASSERT((GetStyle() & (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS)) ==
        (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS));

    CDC* pDC = CDC::FromHandle(lpDrawItemStruct->hDC);

    if (((LONG)(lpDrawItemStruct->itemID) >= 0) &&
        (lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
    {
        int cyItem = GetItemHeight(lpDrawItemStruct->itemID);
        BOOL fDisabled = !IsWindowEnabled() || !IsEnabled(lpDrawItemStruct->itemID);
        BOOL bCheck = GetCheck(lpDrawItemStruct->itemID);
                
        COLORREF newTextColor = fDisabled ?    m_clrDisableText : (bCheck ? m_clrChkText : m_clrText);//GetSysColor(COLOR_WINDOWTEXT);  // light gray
        COLORREF oldTextColor = pDC->SetTextColor(newTextColor);

        COLORREF newBkColor = GetSysColor(COLOR_WINDOW);
        COLORREF oldBkColor = pDC->SetBkColor(newBkColor);

        if (newTextColor == newBkColor)
            newTextColor = RGB(0xC0, 0xC0, 0xC0);   // dark gray

        if (!fDisabled && ((lpDrawItemStruct->itemState & ODS_SELECTED) != 0))
        {
            pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHTTEXT));
            pDC->SetBkColor(GetSysColor(COLOR_HIGHLIGHT));
        }

        if (m_cyText == 0)
            VERIFY(cyItem >= CalcMinimumItemHeight());

        CString strText;
        GetText(lpDrawItemStruct->itemID, strText);

        pDC->ExtTextOut(lpDrawItemStruct->rcItem.left,
            lpDrawItemStruct->rcItem.top + max(0, (cyItem - m_cyText) / 2),
            ETO_OPAQUE, &(lpDrawItemStruct->rcItem), "", 0, NULL);
        pDC->TabbedTextOut(lpDrawItemStruct->rcItem.left,
            lpDrawItemStruct->rcItem.top + max(0, (cyItem - m_cyText) / 2),
            strText, strText.GetLength(), m_nTabStops, m_lpnEachStop, lpDrawItemStruct->rcItem.left);

        pDC->SetTextColor(oldTextColor);
        pDC->SetBkColor(oldBkColor);
    }

    if ((lpDrawItemStruct->itemAction & ODA_FOCUS) != 0)
        pDC->DrawFocusRect(&(lpDrawItemStruct->rcItem));
}

void CTabCheckListBox::SetTabStops()
{
    delete []m_lpnEachStop;
    m_lpnEachStop = new int[1];
    int nBaseUnit = GetAverageCharWidths();
    m_lpnEachStop[0] = nBaseUnit / 2;
    m_nTabStops = 1;
}


BOOL CTabCheckListBox::SetTabStops( const int& cxEachStop )
{
    if(cxEachStop < 0)
        return FALSE;
    delete []m_lpnEachStop;
    m_lpnEachStop = new int[1];
    int nBaseUnit = GetAverageCharWidths();
    m_lpnEachStop[0] = (cxEachStop * nBaseUnit) / 4;
    m_nTabStops = 1;
    return TRUE;
}

BOOL CTabCheckListBox::SetTabStops( int nTabStops, LPINT rgTabStops )
{
    delete []m_lpnEachStop;
    int nStop = 0;
    m_nTabStops = nTabStops;
    m_lpnEachStop = new int[nTabStops];
//    int nBaseUnit = GetAverageCharWidths();
    int nPrevStop = 0;
    for(int i = 0; i < nTabStops; i++)
    {
        nStop = rgTabStops[i];//(rgTabStops[i] * nBaseUnit) / 4;
        if(nStop > nPrevStop)
        {
            m_lpnEachStop[i] = nStop;
            nPrevStop = nStop;
        }
        else
            return FALSE;
    }
    return TRUE;
}

int CTabCheckListBox::GetAverageCharWidths()
{
/*
    CFont* pFont = GetFont();
    LOGFONT lf;
    pFont->GetLogFont(&lf);
    int nBaseUnit = lf.lfWidth;
    if(nBaseUnit == 0)
*/
    int nBaseUnit = LOWORD(GetDialogBaseUnits());    
    return nBaseUnit;
}