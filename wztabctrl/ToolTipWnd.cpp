// ToolTipWnd.cpp : implementation file
//

#include "stdafx.h"
#include "ToolTipWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

TCHAR CToolTipWnd::m_szClassName[16] = _T("CTooltipWnd");
const UINT nTimerID = 0x001;
/////////////////////////////////////////////////////////////////////////////
// CToolTipWnd

CToolTipWnd::CToolTipWnd()
{
	m_pParentWnd = NULL;
	m_uTimerID = 0;
	m_crBackGnd = ::GetSysColor(COLOR_INFOBK);
	m_crText = ::GetSysColor(COLOR_INFOTEXT);
}

CToolTipWnd::CToolTipWnd(CWnd *pParentWnd)
{
	m_pParentWnd = pParentWnd;
	m_uTimerID = 0;
	m_crBackGnd = ::GetSysColor(COLOR_INFOBK);
	m_crText = ::GetSysColor(COLOR_INFOTEXT);
}

CToolTipWnd::~CToolTipWnd()
{
}


BEGIN_MESSAGE_MAP(CToolTipWnd, CWnd)
	//{{AFX_MSG_MAP(CToolTipWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



BOOL CToolTipWnd::PopupTipWnd(LPPOINT ptCur, LPCTSTR lpszText)
{
	if(!IsWindow(GetSafeHwnd()))
	{
		if(!Create(m_pParentWnd))
			return FALSE;
	}

	if(m_uTimerID	!= 0)
	{
		KillTimer(m_uTimerID);
		m_uTimerID = 0;
	}
	
	m_strCurTipText = lpszText;
	m_uTimerID = SetTimer(nTimerID,1000,NULL);
	if(m_uTimerID != 0)
	{
		ASSERT_VALID(m_pParentWnd);
		CPoint ptPos = *ptCur;
		m_pParentWnd->ClientToScreen(&ptPos);

		CDC *pDC = GetDC();
		ASSERT(pDC != NULL);
		CFont *pOldFont = (CFont *)pDC->SelectStockObject(DEFAULT_GUI_FONT);
		CRect rcTemp(0,0,0,0);
		int nHeight = pDC->DrawText(m_strCurTipText,&rcTemp,DT_LEFT|DT_SINGLELINE|DT_CALCRECT);
		pDC->SelectObject(pOldFont);
		ReleaseDC(pDC);

		int nWidth = rcTemp.Width() + 8;
		nHeight += 8;

		SetWindowPos(&wndTop,ptPos.x,ptPos.y,nWidth,nHeight,SWP_SHOWWINDOW|SWP_NOCOPYBITS|SWP_NOACTIVATE|SWP_NOZORDER);

		return TRUE;
	}
	else
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}
}

BOOL CToolTipWnd::HideTipWnd()
{
	if(IsWindow(GetSafeHwnd()))
	{
		if(m_uTimerID	!= 0)
		{
			KillTimer(m_uTimerID);
			m_uTimerID = 0;
		}

		ShowWindow(SW_HIDE);
	
		return TRUE;
	}
	
	return FALSE;
}

BOOL CToolTipWnd::Create(CWnd *pParentWnd)
{
	ASSERT_VALID(pParentWnd);

	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, m_szClassName, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style			= CS_SAVEBITS;
		wndcls.lpfnWndProc		= ::DefWindowProc;
		wndcls.cbClsExtra		= wndcls.cbWndExtra = 0;
		wndcls.hInstance		= hInst;
		wndcls.hIcon			= NULL;
		wndcls.hCursor			= ::LoadCursor(NULL, IDC_ARROW);
		wndcls.hbrBackground	= NULL;
		wndcls.lpszMenuName		= NULL;
		wndcls.lpszClassName	= m_szClassName;

		if(!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
		}
	}

	DWORD dwStyle = WS_POPUP; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW;// | WS_EX_TOPMOST;

	if (!CreateEx(dwExStyle, m_szClassName, NULL, dwStyle, 0, 0, 0, 0, pParentWnd->GetSafeHwnd(), NULL, NULL))
	{
		return FALSE;
	}

	return TRUE;
}

void CToolTipWnd::DrawTipWnd(CDC *pDC,const RECT& rcClient)
{
	CRect rcWnd = rcClient;
	pDC->FillSolidRect(&rcClient,m_crBackGnd);
	CPen BorderPen(PS_SOLID,1,RGB(0,0,255)); 
	CPen *pOldPen = (CPen *)pDC->SelectObject(&BorderPen);
	pDC->SelectStockObject(NULL_BRUSH);
	pDC->Rectangle(rcWnd);
	BorderPen.DeleteObject();

	rcWnd.DeflateRect(1,1,1,1);

	CFont *pOldFont = (CFont *)pDC->SelectStockObject(DEFAULT_GUI_FONT);
	COLORREF crOTxt = pDC->SetTextColor(m_crText);
	int mode = pDC->SetBkMode(TRANSPARENT);
	pDC->DrawText(m_strCurTipText,&rcWnd,DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	pDC->SetBkMode(mode);
	pDC->SetTextColor(crOTxt);
	pDC->SelectObject(pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// CToolTipWnd message handlers

BOOL CToolTipWnd::DestroyWindow() 
{
	if(m_uTimerID	!= 0)
	{
		KillTimer(m_uTimerID);
		m_uTimerID = 0;
	}
	return CWnd::DestroyWindow();
}

int CToolTipWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	return 0;
}

void CToolTipWnd::OnTimer(UINT nIDEvent) 
{
	TRACE(_T("OnTimer was called\r\n"));

	if(m_uTimerID	== nIDEvent)
	{
		KillTimer(m_uTimerID);
		m_uTimerID = 0;
	}

	ShowWindow(SW_HIDE);
	
	CWnd::OnTimer(nIDEvent);
}

void CToolTipWnd::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	CBitmap bitmap;
	memDC.CreateCompatibleDC(&dc);
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap); 
	
	memDC.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &dc, 0,0, SRCCOPY);
	
	DrawTipWnd(&memDC,rect);

	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0,0, SRCCOPY);
	
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject(); 
}

BOOL CToolTipWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
