
#include "stdafx.h"
#include "RangeSliderCtrl.h"
#include "MemDC.h"

#include <math.h> // for fabs

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define RANGESLIDER_CLASSNAME "MFCRangeSliderCtrl"

const UINT RANGE_VALUE_CHANGED = 
   ::RegisterWindowMessage(_T("RANGE_VALUE_CHANGED{2E7439A4-12DB-42a2-BB95-3462B3EC39CE}CRangeSliderCtrl"));


// Register the window class, so you can use this class as a custom control.
BOOL CRangeSliderCtrl::RegisterControlClass(void)
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, RANGESLIDER_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = NULL; // No Background brush (gives flicker)
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = RANGESLIDER_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

void AFXAPI DDX_RangeSliderCtrl(CDataExchange *pDX, int nIDC, double& valMin, double& valMax)
{
    HWND hWndCtrl = pDX->PrepareCtrl(nIDC);
    ASSERT (hWndCtrl != NULL);                
    
    CRangeSliderCtrl *pRangeSlider = (CRangeSliderCtrl *)CWnd::FromHandle(hWndCtrl);
    if (pDX->m_bSaveAndValidate)
    {
        pRangeSlider->GetValueRange(valMin, valMax);
    }
    else // initializing
    {
        pRangeSlider->SetValueRange(valMin, valMax);
    }
}


static void ExchangeDoubleValue(double& v1, double& v2)
{
    double tmp = v1;
    v1 = v2;
    v2 = tmp;
}

/////////////////////////////////////////////////////////////////////////////
// CRangeSliderCtrl

CRangeSliderCtrl::CRangeSliderCtrl()
{
	CRangeSliderCtrl::RegisterControlClass();
	m_Min = 0.0;
	m_Max = 1.0;
	m_Left = m_Min;
	m_Right = m_Max;
    m_grainy = 0.1;
	m_bHorizontal = TRUE;
	m_handleThick = 6;
	m_bTracking = FALSE;
    m_hHorSizer = ::LoadCursor(NULL, IDC_SIZEWE);
    m_hVerSizer = ::LoadCursor(NULL, IDC_SIZENS);
    m_hAllSizer = ::LoadCursor(NULL, IDC_SIZEALL);
    m_TrackMode = TRACK_NONE;
    m_mouseHit = TRACK_NONE;
}

CRangeSliderCtrl::~CRangeSliderCtrl()
{
}

// Relay call to create
void CRangeSliderCtrl::Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext)
{
	CWnd::Create(NULL, "", dwStyle, rect, pParentWnd, nID, pContext);
}

void CRangeSliderCtrl::SetVerticalMode(BOOL bVerticalMode) 
{
	if (m_bHorizontal != bVerticalMode) 
		return; 

	m_bHorizontal = !bVerticalMode;
	Invalidate();
}

void CRangeSliderCtrl::SetControlRange(double min, double max) 
{
	if (min > max) 
    {
        ExchangeDoubleValue(min, max);
	}

	m_Min = min;
	m_Max = max;
    RangeNormalize();

	Invalidate();
}

void CRangeSliderCtrl::SetInternalRange(double left, double right) 
{
	m_Left = left;
	m_Right = right;
	Invalidate();
}

void CRangeSliderCtrl::SetValueRange(double valueMin, double valueMax)
{
    if(valueMin > valueMax)
    {
        ExchangeDoubleValue(valueMin, valueMax);
    }
    m_Left = valueMin;
    m_Right = valueMax;
    RangeNormalize();

    Invalidate();
}

void CRangeSliderCtrl::GetValueRange(double &valueMin, double &valueMax)
{
    valueMin = GetValueMin();
    valueMax = GetValueMax();
}

double CRangeSliderCtrl::GetValueMin()
{
    //double vleft = (m_Left + 0.00000001) / m_grainy;
    double vleft = (m_Left + m_grainy * 0.5000000001) / m_grainy;
    int tmp = static_cast<int>(vleft);
    
    return (tmp * m_grainy);
}

double CRangeSliderCtrl::GetValueMax()
{
    //double vright = (m_Right + 0.00000001) / m_grainy;
    double vright = (m_Right + m_grainy * 0.5000000001) / m_grainy;
    int tmp = static_cast<int>(vright);
    
    return (tmp * m_grainy);
}

BEGIN_MESSAGE_MAP(CRangeSliderCtrl, CWnd)
	//{{AFX_MSG_MAP(CRangeSliderCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	ON_WM_SETFOCUS()
	ON_WM_GETDLGCODE()
    ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRangeSliderCtrl message handlers
void CRangeSliderCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CMemDC memDC(&dc);
	memDC.SetMapMode(MM_TEXT);

	CRect ClipBox;
	memDC.GetClipBox(&ClipBox);

    PaintRangeSliderCtrl(memDC, ClipBox);
}

int CRangeSliderCtrl::GetScrollClient()
{
	CRect rcClient;
	GetClientRect(&rcClient);
    
    if(m_bHorizontal)
    {
        return (rcClient.Width() - 2 * m_handleThick);
    }
    else
    {
    	return (rcClient.Height() - 2 * m_handleThick);
    }
}

int CRangeSliderCtrl::GetRangeRect(CRect& rcLeft, CRect& rcRight, CRect& rcMid)
{
	CRect rcClient;
	GetClientRect(&rcClient);

    int dx;
    if(m_bHorizontal)
    {
        dx = rcClient.Width() - 2 * m_handleThick;
	    int x1 = static_cast<int>((m_Left - m_Min)/(m_Max - m_Min) * dx + 0.5);
	    int x2 = static_cast<int>((m_Right - m_Min)/(m_Max - m_Min) * dx + 0.5);
        rcLeft = CRect(CPoint(x1, 0), CSize(m_handleThick, rcClient.Height()));
	    rcRight = CRect(CPoint(m_handleThick + x2, 0), CSize(m_handleThick, rcClient.Height()));
        rcMid = CRect(rcLeft.right, 0, rcRight.left, rcClient.Height());
    }
    else
    {
    	dx = rcClient.Height() - 2 * m_handleThick;
	    int x1 = static_cast<int>((m_Left - m_Min)/(m_Max - m_Min) * dx + 0.5);
	    int x2 = static_cast<int>((m_Right - m_Min)/(m_Max - m_Min) * dx + 0.5);
        rcLeft = CRect (CPoint(0, x1), CSize(rcClient.Width(), m_handleThick));
        rcRight = CRect(CPoint(0, m_handleThick + x2), CSize(rcClient.Width(), m_handleThick));
        rcMid = CRect(0, rcLeft.bottom, rcLeft.right, rcRight.top);
    }

    return dx;
}

void CRangeSliderCtrl::RangeNormalize()
{
    if(m_Left > m_Max)
    {
	    m_Left = m_Max;
	    m_Right = m_Max;
    }
    else if(m_Right < m_Min)
    {
	    m_Left = m_Min;
	    m_Right = m_Min;
    }
    else if(m_Right > m_Max)
    {
        m_Right = m_Max;
    }
    else if(m_Left < m_Min)
    {
        m_Left = m_Min;
    }
    else
    {
        //normal status
    }
}

void CRangeSliderCtrl::PaintBackground(CDC& dc, const CRect& rc)
{
	CGdiObject *pOldBr = dc.SelectStockObject(GRAY_BRUSH);
	CGdiObject *pOldPen = dc.SelectStockObject(BLACK_PEN);

	dc.Rectangle(&rc);

	dc.SelectObject(pOldBr);
	dc.SelectObject(pOldPen);
}

void CRangeSliderCtrl::PaintHandle(CDC& dc, CRect& rcHandle, MOUSE_HIT mode, CBrush *pBkGnd, CPen *pLightPen, CPen *pDarkPen)
{
    CPen *pOldPen = (CPen *)dc.SelectStockObject(BLACK_PEN);
	CBrush *pOldBr = (CBrush *)dc.SelectObject(pBkGnd);

    dc.Rectangle(&rcHandle);
	if (!m_bTracking || (m_TrackMode != mode)) 
    {
	    rcHandle.DeflateRect(1,1,2,2);
		dc.SelectObject(pLightPen);
		dc.MoveTo(rcHandle.left, rcHandle.bottom);
		dc.LineTo(rcHandle.left, rcHandle.top);
		dc.LineTo(rcHandle.right, rcHandle.top);
		
		dc.SelectObject(pDarkPen);
		dc.LineTo(rcHandle.right, rcHandle.bottom);
		dc.LineTo(rcHandle.left, rcHandle.bottom);
	}
	dc.SelectObject(pOldBr);
	dc.SelectObject(pOldPen);
}

void CRangeSliderCtrl::PaintMiddleBar(CDC& dc, CRect& rcMid)
{
	CPen *pOldPen = (CPen *)dc.SelectStockObject(NULL_PEN);
	CBrush br;
	br.CreateSolidBrush(RGB(255,255,150)); // light yellow;
	CBrush *pOldBr = (CBrush *)dc.SelectObject(&br);
	
    dc.Rectangle(&rcMid);

    if (GetFocus() == this)
    {
		rcMid.DeflateRect(3,3);
		dc.DrawFocusRect(&rcMid);
	}
	dc.SelectObject(pOldBr);
	dc.SelectObject(pOldPen);
}

void CRangeSliderCtrl::PaintRangeSliderCtrl(CDC& dc, const CRect& rcClipbox)
{
    CRect rcLeft, rcRight, rcMid;
    GetRangeRect(rcLeft, rcRight, rcMid);

    PaintBackground(dc, rcClipbox);

	CBrush ButtonFace;
	ButtonFace.CreateSolidBrush(::GetSysColor(COLOR_BTNFACE));
	CPen pen1, pen2;
	pen1.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNHIGHLIGHT));
    pen2.CreatePen(PS_SOLID, 1, ::GetSysColor(COLOR_BTNSHADOW));

    PaintHandle(dc, rcLeft, TRACK_LEFT, &ButtonFace, &pen1, &pen2);
    PaintHandle(dc, rcRight, TRACK_RIGHT, &ButtonFace, &pen1, &pen2);

    if(m_bHorizontal)
        rcMid.DeflateRect(0, 1, 0, 0);
    else
        rcMid.DeflateRect(1, 0, 0, 0);

    PaintMiddleBar(dc, rcMid);
}

void CRangeSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	TRACE("Down Point %d, %d\n", point.x, point.y);
	SetFocus();
	Invalidate();

	if (!m_bTracking) 
    {
        CRect rcLeft, rcRight, rcMid;
        GetRangeRect(rcLeft, rcRight, rcMid);
		if (rcLeft.PtInRect(point)) 
        {
			m_bTracking = TRUE;
			m_ClickOffset = point - rcLeft.CenterPoint();
		} 
        else if (rcRight.PtInRect(point)) 
        {
			m_bTracking = TRUE;
			m_ClickOffset = point - rcRight.CenterPoint();
		} 
        else if (rcMid.PtInRect(point)) 
        {
			m_bTracking = TRUE;
			m_ClickOffset = point - rcMid.CenterPoint();
        }
        else
        {
			m_bTracking = FALSE;
        }
        m_TrackMode = m_mouseHit;
            
        if (m_bTracking)
			SetCapture();
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CRangeSliderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
    CRect rcLeft, rcRight, rcMid;
    int scroll = GetRangeRect(rcLeft, rcRight, rcMid);
    if(rcLeft.PtInRect(point))
        m_mouseHit = TRACK_LEFT;
    else if(rcRight.PtInRect(point))
        m_mouseHit = TRACK_RIGHT;
    else if(rcMid.PtInRect(point))
        m_mouseHit = TRACK_MIDDLE;
    else
        m_mouseHit = TRACK_NONE;

    if (m_bTracking) 
    {
		int x = 0;
		if (m_bHorizontal)
			x = point.x - m_ClickOffset.x;
		else
			x = point.y - m_ClickOffset.y;

		WPARAM changed = 0;
		switch (m_TrackMode) 
        {
		    case TRACK_LEFT: 
            {
			    double oldLeft = m_Left;
			    m_Left = static_cast<double>(x - m_handleThick / 2) / scroll * (m_Max - m_Min) + m_Min; 
			    if (m_Left >= m_Right)
				    m_Left = m_Right;
			    if (m_Left <= m_Min)
				    m_Left = m_Min;

			    if (oldLeft != m_Left)
				    changed = RS_LEFTCHANGED;
		    } 
            break;
		    case TRACK_RIGHT: 
            {
			    double oldRight = m_Right;
			    m_Right = static_cast<double>(x - m_handleThick * 3 / 2) / scroll * (m_Max - m_Min) + m_Min;
			    if (m_Right <= m_Left)
				    m_Right = m_Left;
			    if (m_Right >= m_Max)
				    m_Right = m_Max;
			    if (oldRight != m_Right)
				    changed = RS_RIGHTCHANGED;
		    } 
            break;
		    case TRACK_MIDDLE:
		    {
			    double delta = m_Right - m_Left;
			    ASSERT(delta >= 0.0);
			    m_Left  = static_cast<double>(x - m_handleThick) / scroll * (m_Max - m_Min) + m_Min - delta/2.0;
			    m_Right = static_cast<double>(x - m_handleThick) / scroll * (m_Max - m_Min) + m_Min + delta/2.0; 
			    if (m_Left <= m_Min) {
				    m_Left = m_Min;
				    m_Right = m_Left + delta;
			    }
			    if (m_Right >= m_Max) {
				    m_Right = m_Max;
				    m_Left = m_Right - delta;
			    }
			    changed = RS_BOTHCHANGED;
		    }
		    break;
		    default:
			    TRACE("Unknown Track Mode\n");
			    ASSERT(FALSE);
			    break;
		}
		::SendMessage(GetParent()->GetSafeHwnd(), RANGE_VALUE_CHANGED, (WPARAM)GetDlgCtrlID(), (LPARAM)changed);
		Invalidate();
	}
	
	CWnd::OnMouseMove(nFlags, point);
}

void CRangeSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bTracking) 
    {
		m_bTracking = FALSE;
		::ReleaseCapture();
		Invalidate();
	}
	
	CWnd::OnLButtonUp(nFlags, point);
}

UINT CRangeSliderCtrl::OnGetDlgCode() 
{
	return DLGC_WANTARROWS;
}

void CRangeSliderCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	UINT Key1, Key2;
	if (m_bHorizontal) 
    {
		Key1 = VK_RIGHT; Key2 = VK_LEFT;
	} 
    else 
    {
		Key1 = VK_DOWN; Key2 = VK_UP;
	}
	
	if ((nChar == Key1 || nChar == Key2))
    {
        int scroll = GetScrollClient();
		double dx = (m_Max - m_Min) / scroll;
		if (dx != 0.0 ) 
        {
			int left = static_cast<int>((m_Left - m_Min) / dx + 0.5);
			int right = static_cast<int>((m_Right - m_Min) / dx + 0.5);
			BOOL bShift = ::GetKeyState(VK_SHIFT) < 0; // T if Shift is pressed.
			if (nChar == Key2) 
            {
				if (!bShift) // Shift not pressed => move intervall
					left--;
				right--; 
			} 
            else 
            {
				if (!bShift) // Shift not pressed => move intervall
					left++;
				right++;
			}
			if (left >= 0 && right <= scroll && left <= right) 
            {
				m_Left = m_Min + left * dx;
				m_Right = m_Min + right * dx;
			}
			if (!bShift)
				::SendMessage(GetParent()->GetSafeHwnd(), RANGE_VALUE_CHANGED, (WPARAM)GetDlgCtrlID(), (LPARAM)RS_BOTHCHANGED);
			else 
				::SendMessage(GetParent()->GetSafeHwnd(), RANGE_VALUE_CHANGED, (WPARAM)GetDlgCtrlID(), (LPARAM)RS_RIGHTCHANGED);
			Invalidate();
		}
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CRangeSliderCtrl::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd::OnKillFocus(pNewWnd);

    if (m_bTracking) 
    {
		m_bTracking = FALSE;
		::ReleaseCapture();
	}

    Invalidate();
}

void CRangeSliderCtrl::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);
	
	Invalidate();	
}

BOOL CRangeSliderCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
    if((m_mouseHit == TRACK_LEFT) || (m_mouseHit == TRACK_RIGHT))
    {
        SetCursor(m_bHorizontal ? m_hHorSizer : m_hVerSizer);
        return TRUE;
    }
    else if(m_mouseHit == TRACK_MIDDLE)
    {
        SetCursor(m_hAllSizer);
        return TRUE;
    }
    else
    {
        return CWnd::OnSetCursor(pWnd, nHitTest, message);
    }
}
