#include "stdafx.h"
#include "WzTabTestTheme.h"
#include "Gradient.h"
#include "WzIniFile.h"


CWzTabTestTheme::CWzTabTestTheme()
{
    m_pImgList = NULL;
}

CWzTabTestTheme::~CWzTabTestTheme()
{
	ReleaseConfig();
}

CImageList *CWzTabTestTheme::SetImageList(CImageList *pImgList) 
{ 
    CImageList *pold = m_pImgList;
    m_pImgList = pImgList; 

    return pold;
}

void CWzTabTestTheme::InitDefaultConfig()
{
    m_crTabTextNormal = RGB(132,210,45);
    m_crTabTextHover = RGB(1,185,254);
    m_crTabTextSelected = RGB(148,60,196);
    m_crTabTextLastSelected = RGB(100,30,128);
    m_crTabBkNormalT=RGB(30,249,123);
    m_crTabBkNormalC=RGB(17,154,255);
    m_crTabBkNormalB=RGB(189,78,194);
    m_crTabBkHoverT=RGB(108,251,168);
    m_crTabBkHoverC=RGB(89,189,242);
    m_crTabBkHoverB=RGB(213,141,216);
    m_crTabBkSelectedT=RGB(184,181,244);
    m_crTabBkSelectedC=RGB(150,248,141);
    m_crTabBkSelectedB=RGB(252,154,184);
    m_crTabBkLastSelectedT=RGB(128,128,255);
    m_crTabBkLastSelectedC=RGB(240,147,172);
    m_crTabBkLastSelectedB=RGB(80,160,150);
    m_crWndBkGnd = RGB(108,175,183);

    m_crBorderNormal = RGB(0,0,255);
	m_crBorderHover = RGB(255,128,64);
	m_crBorderSelected = RGB(0,128,255);
	m_crBorderLastSelected = RGB(0,128,64);

    m_fontName = "Tahoma";
    m_fontSize = 120;
    m_fontBold = 500;

    InitGdiObject();
}

BOOL CWzTabTestTheme::InitGdiObject()
{
	ASSERT(m_BoderPenNormal.GetSafeHandle() == NULL);
	m_BoderPenNormal.CreatePen(PS_SOLID,1,m_crBorderNormal);
	if(m_BoderPenNormal.GetSafeHandle() == NULL)
		return FALSE;

	ASSERT(m_BoderPenHover.GetSafeHandle() == NULL);
	m_BoderPenHover.CreatePen(PS_SOLID,1,m_crBorderHover);
	if(m_BoderPenHover.GetSafeHandle() == NULL)
		return FALSE;

	ASSERT(m_BoderPenSelected.GetSafeHandle() == NULL);
	m_BoderPenSelected.CreatePen(PS_SOLID,1,m_crBorderSelected);
	if(m_BoderPenSelected.GetSafeHandle() == NULL)
		return FALSE;

	ASSERT(m_BoderPenLastSelected.GetSafeHandle() == NULL);
	m_BoderPenLastSelected.CreatePen(PS_SOLID,1,m_crBorderLastSelected);
	if(m_BoderPenLastSelected.GetSafeHandle() == NULL)
		return FALSE;

	ASSERT(m_TextFont.GetSafeHandle() == NULL);
	//m_TextFont.CreateStockObject(SYSTEM_FONT);
    //m_TextFont.CreatePointFont(m_fontSize, m_fontName);

    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));

    lf.lfHeight = m_fontSize;
    lf.lfWeight = m_fontBold;
    _tcscpy_s(lf.lfFaceName, LF_FACESIZE, m_fontName);
    //m_TextFont.CreateFontIndirect(&lf);
    m_TextFont.CreatePointFontIndirect(&lf);

	if(m_TextFont.GetSafeHandle() == NULL)
		return FALSE;

    return TRUE;
}

void CWzTabTestTheme::ReleaseGdiObject()
{
	if(m_BoderPenNormal.GetSafeHandle() != NULL)
	{
		m_BoderPenNormal.DeleteObject();
	}
	if(m_BoderPenHover.GetSafeHandle() != NULL)
	{
		m_BoderPenHover.DeleteObject();
	}
	if(m_BoderPenSelected.GetSafeHandle() != NULL)
	{
		m_BoderPenSelected.DeleteObject();
	}
	if(m_BoderPenLastSelected.GetSafeHandle() != NULL)
	{
		m_BoderPenLastSelected.DeleteObject();
	}

	if(m_TextFont.GetSafeHandle() != NULL)
	{
		m_TextFont.DeleteObject();
	}
}

void CWzTabTestTheme::ResetNormalBorderPen()
{
	if(m_BoderPenNormal.GetSafeHandle() != NULL)
	{
		m_BoderPenNormal.DeleteObject();
	}
	m_BoderPenNormal.CreatePen(PS_SOLID,1,m_crBorderNormal);
}

void CWzTabTestTheme::ResetHoverBorderPen()
{
	if(m_BoderPenHover.GetSafeHandle() != NULL)
	{
		m_BoderPenHover.DeleteObject();
	}
	m_BoderPenHover.CreatePen(PS_SOLID,1,m_crBorderHover);
}

void CWzTabTestTheme::ResetSelectedBorderPen()
{
	if(m_BoderPenSelected.GetSafeHandle() != NULL)
	{
		m_BoderPenSelected.DeleteObject();
	}
	m_BoderPenSelected.CreatePen(PS_SOLID,1,m_crBorderSelected);
}

void CWzTabTestTheme::ResetLastSelectedBorderPen()
{
	if(m_BoderPenLastSelected.GetSafeHandle() != NULL)
	{
		m_BoderPenLastSelected.DeleteObject();
	}
	m_BoderPenLastSelected.CreatePen(PS_SOLID,1,m_crBorderLastSelected);
}

void CWzTabTestTheme::ResetTextfont()
{
	if(m_TextFont.GetSafeHandle() != NULL)
	{
		m_TextFont.DeleteObject();
	}
    LOGFONT lf;
    memset(&lf, 0, sizeof(LOGFONT));

    lf.lfHeight = m_fontSize;
    lf.lfWeight = m_fontBold;
    _tcscpy_s(lf.lfFaceName, LF_FACESIZE, m_fontName);
    //m_TextFont.CreateFontIndirect(&lf);
    m_TextFont.CreatePointFontIndirect(&lf);

    //m_TextFont.CreatePointFont(m_fontSize, m_fontName);
}


BOOL CWzTabTestTheme::InitConfig(LPCTSTR themeFile)
{
    CWzIniFile ini(themeFile);
    ini.Open();

    int sign = 0;
    if(ini.GetValue(_T("INFO"), _T("sign"), sign, 0) && (sign == themeFilesign))
    {
	    ini.GetValue(_T("TAB_COLOR"), _T("TextNormal"), m_crTabTextNormal, RGB(132,210,45));
	    ini.GetValue(_T("TAB_COLOR"), _T("TextHover"), m_crTabTextHover, RGB(1,185,254));
	    ini.GetValue(_T("TAB_COLOR"), _T("TextSelected"), m_crTabTextSelected, RGB(148,60,196));
	    ini.GetValue(_T("TAB_COLOR"), _T("TextLastSelected"), m_crTabTextLastSelected, RGB(100,30,128));
	    ini.GetValue(_T("TAB_COLOR"), _T("BkGndNormalT"), m_crTabBkNormalT, RGB(128,128,0));
	    ini.GetValue(_T("TAB_COLOR"), _T("BkGndNormalC"), m_crTabBkNormalC, RGB(0,128,220));
	    ini.GetValue(_T("TAB_COLOR"), _T("BkGndNormalB"), m_crTabBkNormalB, RGB(128,60,128));
	    ini.GetValue(_T("TAB_COLOR"), _T("BkGndHoverT"), m_crTabBkHoverT, RGB(251,134,4));
	    ini.GetValue(_T("TAB_COLOR"), _T("BkGndHoverC"), m_crTabBkHoverC, RGB(10,210,245));
	    ini.GetValue(_T("TAB_COLOR"), _T("BkGndHoverB"), m_crTabBkHoverB, RGB(128,200,80));
	    ini.GetValue(_T("TAB_COLOR"), _T("BkGndSelectedT"), m_crTabBkSelectedT, RGB(17,238,227));
	    ini.GetValue(_T("TAB_COLOR"), _T("BkGndSelectedC"), m_crTabBkSelectedC, RGB(167,88,146));
	    ini.GetValue(_T("TAB_COLOR"), _T("BkGndSelectedB"), m_crTabBkSelectedB, RGB(80,120,100));
	    ini.GetValue(_T("TAB_COLOR"), _T("BkGndLastSelectedT"), m_crTabBkLastSelectedT, RGB(128,128,255));
	    ini.GetValue(_T("TAB_COLOR"), _T("BkGndLastSelectedC"), m_crTabBkLastSelectedC, RGB(0,255,64));
	    ini.GetValue(_T("TAB_COLOR"), _T("BkGndLastSelectedB"), m_crTabBkLastSelectedB, RGB(80,160,150));
	    ini.GetValue(_T("GLOBAL"), _T("WndBkGnd"), m_crWndBkGnd, RGB(108,175,183));

        ini.GetValue(_T("GLOBAL"), _T("BorderColorNormal"), m_crBorderNormal, RGB(0,0,255));
        ini.GetValue(_T("GLOBAL"), _T("BorderColorHover"), m_crBorderHover, RGB(255,128,64));
        ini.GetValue(_T("GLOBAL"), _T("BorderColorSelected"), m_crBorderSelected, RGB(0,128,255));
        ini.GetValue(_T("GLOBAL"), _T("BorderColorLastSelected"), m_crBorderLastSelected, RGB(0,128,64));
        
        ini.GetValue(_T("GLOBAL"), _T("FontName"), m_fontName, _T("System"));
        ini.GetValue(_T("GLOBAL"), _T("FontSize"), m_fontSize, 120);
        ini.GetValue(_T("GLOBAL"), _T("FontBold"), m_fontBold, 500);

        ReleaseGdiObject();
        InitGdiObject();
        return TRUE;
    }

	return FALSE;
}

BOOL CWzTabTestTheme::SaveConfig(LPCTSTR themeFile)
{
    CWzIniFile ini(themeFile);

    ini.SetValue(_T("INFO"), _T("sign"), themeFilesign);

	ini.SetValue(_T("TAB_COLOR"), _T("TextNormal"), m_crTabTextNormal);
	ini.SetValue(_T("TAB_COLOR"), _T("TextHover"), m_crTabTextHover);
	ini.SetValue(_T("TAB_COLOR"), _T("TextSelected"), m_crTabTextSelected);
	ini.SetValue(_T("TAB_COLOR"), _T("TextLastSelected"), m_crTabTextLastSelected);
	ini.SetValue(_T("TAB_COLOR"), _T("BkGndNormalT"), m_crTabBkNormalT);
	ini.SetValue(_T("TAB_COLOR"), _T("BkGndNormalC"), m_crTabBkNormalC);
	ini.SetValue(_T("TAB_COLOR"), _T("BkGndNormalB"), m_crTabBkNormalB);
	ini.SetValue(_T("TAB_COLOR"), _T("BkGndHoverT"), m_crTabBkHoverT);
	ini.SetValue(_T("TAB_COLOR"), _T("BkGndHoverC"), m_crTabBkHoverC);
	ini.SetValue(_T("TAB_COLOR"), _T("BkGndHoverB"), m_crTabBkHoverB);
	ini.SetValue(_T("TAB_COLOR"), _T("BkGndSelectedT"), m_crTabBkSelectedT);
	ini.SetValue(_T("TAB_COLOR"), _T("BkGndSelectedC"), m_crTabBkSelectedC);
	ini.SetValue(_T("TAB_COLOR"), _T("BkGndSelectedB"), m_crTabBkSelectedB);
	ini.SetValue(_T("TAB_COLOR"), _T("BkGndLastSelectedT"), m_crTabBkLastSelectedT);
	ini.SetValue(_T("TAB_COLOR"), _T("BkGndLastSelectedC"), m_crTabBkLastSelectedC);
	ini.SetValue(_T("TAB_COLOR"), _T("BkGndLastSelectedB"), m_crTabBkLastSelectedB);
	ini.SetValue(_T("GLOBAL"), _T("WndBkGnd"), m_crWndBkGnd);

    ini.SetValue(_T("GLOBAL"), _T("BorderColorNormal"), m_crBorderNormal);
    ini.SetValue(_T("GLOBAL"), _T("BorderColorHover"), m_crBorderHover);
    ini.SetValue(_T("GLOBAL"), _T("BorderColorSelected"), m_crBorderSelected);
    ini.SetValue(_T("GLOBAL"), _T("BorderColorLastSelected"), m_crBorderLastSelected);

    ini.SetValue(_T("GLOBAL"), _T("FontName"), m_fontName);
    ini.SetValue(_T("GLOBAL"), _T("FontSize"), m_fontSize);
    ini.SetValue(_T("GLOBAL"), _T("FontBold"), m_fontBold);

    return ini.Save();
}

void CWzTabTestTheme::ReleaseConfig()
{
    ReleaseGdiObject();
}

COLORREF CWzTabTestTheme::GetTabWndBkgndColor()
{
    return m_crWndBkGnd;
}


CFont *CWzTabTestTheme::GetTextFont() 
{ 
	ASSERT(m_TextFont.GetSafeHandle() != NULL); 
	
	return &m_TextFont; 
};

void CWzTabTestTheme::DrawSubItem(CDC *pDC, int iconIdx, const CString& text, const CRect& rcSubItem, 
                                      TAB_ITEM_STATE baseState, TAB_SUB_ITEM_STATE subState)
{
	ASSERT(m_pImgList != NULL);
	ASSERT(m_TextFont.GetSafeHandle() != NULL);
	ASSERT(m_BoderPenNormal.GetSafeHandle() != NULL);
	ASSERT(m_BoderPenHover.GetSafeHandle() != NULL);
	ASSERT(m_BoderPenSelected.GetSafeHandle() != NULL);
	ASSERT(m_BoderPenLastSelected.GetSafeHandle() != NULL);

    switch(subState)
	{
	case SUB_HOVER_STATE:
		pDC->SelectObject(&m_BoderPenHover);
		break;
	case SUB_ACTIVE_STATE:
		pDC->SelectObject(&m_BoderPenSelected);
		break;
	case SUB_LAST_ACTIVE_STATE:
		pDC->SelectObject(&m_BoderPenLastSelected);
		break;
	case SUB_NORMAL_STATE:
		pDC->SelectObject(&m_BoderPenNormal);
		break;
	default:
		pDC->SelectObject(&m_BoderPenNormal);
		break;
	}

	pDC->MoveTo(rcSubItem.left,rcSubItem.top);
	pDC->LineTo(rcSubItem.left,rcSubItem.bottom);

	CRect rcText = rcSubItem;
	rcText.left++;

    CGradient gradient;
    if((subState == SUB_HOVER_STATE) || (baseState == HOVER_STATE))
    {
        COLORREF colors[] = {m_crTabBkHoverT, m_crTabBkHoverC, m_crTabBkHoverB};
        gradient.MulticolorVerticalGradient(pDC->m_hDC, (LPRECT)&rcText, colors, 3, FALSE, 0.4);
    }
    else if((subState == SUB_ACTIVE_STATE) && (baseState == ACTIVE_STATE))
    {
        COLORREF colors[] = {m_crTabBkSelectedT, m_crTabBkSelectedC, m_crTabBkSelectedB};
        gradient.MulticolorVerticalGradient(pDC->m_hDC, (LPRECT)&rcText, colors, 3, FALSE, 0.4);
    }
    else if((subState == SUB_LAST_ACTIVE_STATE) && (baseState == LAST_ACTIVE_STATE))
    {
        COLORREF colors[] = {m_crTabBkLastSelectedT, m_crTabBkLastSelectedC, m_crTabBkLastSelectedB};
        gradient.MulticolorVerticalGradient(pDC->m_hDC, (LPRECT)&rcText, colors, 3, FALSE, 0.4);
    }
    else
    {
        COLORREF colors[] = {m_crTabBkNormalT, m_crTabBkNormalC, m_crTabBkNormalB};
        gradient.MulticolorVerticalGradient(pDC->m_hDC, (LPRECT)&rcText, colors, 3, FALSE, 0.4);
    }

	CRect rcIcon = rcText;
	if(iconIdx != -1)
	{
		rcText.right -= ICON_AREA_SIZE;
		rcIcon.left = rcText.right + 1;

		int nTopSlip = (rcIcon.Height() - ICON_SIZE) / 2;
		int nLeftSlip = (ICON_AREA_SIZE - ICON_SIZE) / 2;
		POINT pt = rcIcon.TopLeft();
		pt.y += nTopSlip;
		pt.x += nLeftSlip;
		m_pImgList->Draw(pDC, iconIdx, pt, ILD_TRANSPARENT);
	}
	if(!text.IsEmpty())
	{
		rcText.left += (SUB_TEXT_AREA_PAD - 2) / 2;
        if((subState == SUB_HOVER_STATE) || (baseState == HOVER_STATE))
        {
			pDC->SetTextColor(m_crTabTextHover);
        }
        else if((subState == SUB_ACTIVE_STATE) && (baseState == ACTIVE_STATE))
        {
            pDC->SetTextColor(m_crTabTextSelected);
        }
    	else if((subState == SUB_LAST_ACTIVE_STATE) && (baseState == LAST_ACTIVE_STATE))
    	{
            pDC->SetTextColor(m_crTabTextLastSelected);
    	}
        else
        {
            pDC->SetTextColor(m_crTabTextNormal);
        }

		CFont *pOf = pDC->SelectObject(&m_TextFont);
		pDC->SetBkMode(TRANSPARENT);
		pDC->DrawText(text, -1, (RECT *)&rcText, DT_SINGLELINE|DT_VCENTER);
		pDC->SelectObject(pOf);
	}
}

void CWzTabTestTheme::DrawBkgnd(CDC *pDC, const CRect& rcClient)
{
    pDC->FillSolidRect(rcClient,m_crWndBkGnd);
}

void CWzTabTestTheme::DrawItemCloseBtn(CDC *pDC, const CRect& rcIcon, int lastHit, int lastState)
{
	ASSERT(m_pImgList != NULL);

	int nTopSlip = (rcIcon.Height() - ICON_SIZE) / 2;
	int nLeftSlip = (ICON_AREA_SIZE - ICON_SIZE) / 2;
	CPoint pt = rcIcon.TopLeft();
	pt.y += nTopSlip;
	pt.x += nLeftSlip;
	m_pImgList->Draw(pDC,0,pt,ILD_TRANSPARENT);

	if(lastHit == HIT_TEST_CLOSEBTN)
	{
		if((lastState & STATE_MOUSE_L_DOWN) == STATE_MOUSE_L_DOWN)
		{
			pDC->DrawEdge((LPRECT)&rcIcon,BDR_SUNKENINNER,BF_TOPLEFT);
			pDC->DrawEdge((LPRECT)&rcIcon,BDR_SUNKENOUTER,BF_BOTTOMRIGHT);
		}
		else
		{
			pDC->DrawEdge((LPRECT)&rcIcon,BDR_RAISEDINNER,BF_TOPLEFT);
			pDC->DrawEdge((LPRECT)&rcIcon,BDR_RAISEDOUTER,BF_BOTTOMRIGHT);
		}
	}
}

void CWzTabTestTheme::DrawTabBorder(CDC *pDC, CRect& rcBorder, TAB_ITEM_STATE state)
{
	ASSERT(m_BoderPenNormal.GetSafeHandle() != NULL);
	ASSERT(m_BoderPenHover.GetSafeHandle() != NULL);
	ASSERT(m_BoderPenSelected.GetSafeHandle() != NULL);
	ASSERT(m_BoderPenLastSelected.GetSafeHandle() != NULL);

	pDC->SelectStockObject(NULL_BRUSH);
	switch(state)
	{
	case HOVER_STATE:
		pDC->SelectObject(&m_BoderPenHover);
		break;
	case ACTIVE_STATE:
		pDC->SelectObject(&m_BoderPenSelected);
		break;
	case LAST_ACTIVE_STATE:
		pDC->SelectObject(&m_BoderPenLastSelected);
		break;
	case NORMAL_STATE:
		pDC->SelectObject(&m_BoderPenNormal);
		break;
	default:
		pDC->SelectObject(&m_BoderPenNormal);
		break;
	}

	rcBorder.right++;
	pDC->Rectangle(rcBorder);

	rcBorder.DeflateRect(1, 1, 1, 1);
}

void CWzTabTestTheme::DrawTabBkgnd(CDC *pDC, const CRect& rcBkgnd, TAB_ITEM_STATE state)
{
    CGradient gradient;
    switch(state)
    {
    case ACTIVE_STATE:
        {
        COLORREF colors[] = {m_crTabBkSelectedT, m_crTabBkSelectedC, m_crTabBkSelectedB};
        gradient.MulticolorVerticalGradient(pDC->m_hDC, (LPRECT)&rcBkgnd, colors, 3, FALSE, 0.4);
        break;
        }
    case LAST_ACTIVE_STATE:
        {
        COLORREF colors[] = {m_crTabBkLastSelectedT, m_crTabBkLastSelectedC, m_crTabBkLastSelectedB};
        gradient.MulticolorVerticalGradient(pDC->m_hDC, (LPRECT)&rcBkgnd, colors, 3, FALSE, 0.4);
        break;
        }
    case HOVER_STATE:
        {
        COLORREF colors[] = {m_crTabBkHoverT, m_crTabBkHoverC, m_crTabBkHoverB};
        gradient.MulticolorVerticalGradient(pDC->m_hDC, (LPRECT)&rcBkgnd, colors, 3, FALSE, 0.4);
        break;
        }
    case NORMAL_STATE:
        {
        COLORREF colors[] = {m_crTabBkNormalT, m_crTabBkNormalC, m_crTabBkNormalB};
        gradient.MulticolorVerticalGradient(pDC->m_hDC, (LPRECT)&rcBkgnd, colors, 3, FALSE, 0.4);
        //pDC->FillSolidRect(rcBkgnd, m_crTabBkNormalT);
        break;
        }
    default:
        {
        COLORREF colors[] = {m_crTabBkNormalT, m_crTabBkNormalC, m_crTabBkNormalB};
        gradient.MulticolorVerticalGradient(pDC->m_hDC, (LPRECT)&rcBkgnd, colors, 3, FALSE, 0.4);
        break;
        }
    }
}

void CWzTabTestTheme::DrawTabText(CDC *pDC, const CRect& rcText, LPCTSTR lpszText, TAB_ITEM_STATE state)
{
	ASSERT(m_TextFont.GetSafeHandle() != NULL);
	
    switch(state)
    {
    case ACTIVE_STATE:
        pDC->SetTextColor(m_crTabTextSelected);
        break;
    case LAST_ACTIVE_STATE:
        pDC->SetTextColor(m_crTabTextLastSelected);
        break;
    case HOVER_STATE:
        pDC->SetTextColor(m_crTabTextHover);
        break;
    case NORMAL_STATE:
        pDC->SetTextColor(m_crTabTextNormal);
        break;
    default:
        pDC->SetTextColor(m_crTabTextNormal);
        break;
    }

	CFont *pOf = pDC->SelectObject(&m_TextFont);
	pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(lpszText,-1,(RECT *)&rcText,DT_SINGLELINE|DT_VCENTER);
	pDC->SelectObject(pOf);
}

