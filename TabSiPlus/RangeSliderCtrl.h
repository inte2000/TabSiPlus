#if !defined(AFX_RANGESLIDERCTRL_H__CEC8D48F_0FDC_415C_A751_3AF12AF8B736__INCLUDED_)
#define AFX_RANGESLIDERCTRL_H__CEC8D48F_0FDC_415C_A751_3AF12AF8B736__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RangeSliderCtrl.h : header file
//

extern const UINT RANGE_VALUE_CHANGED;


/////////////////////////////////////////////////////////////////////////////
// CRangeSliderCtrl window

enum _RangeMessages 
{
	RS_LEFTCHANGED,
	RS_RIGHTCHANGED,
	RS_BOTHCHANGED,
};

typedef enum 
{
	TRACK_LEFT,
	TRACK_RIGHT,
	TRACK_MIDDLE,
    TRACK_NONE
}MOUSE_HIT; 

void AFXAPI DDX_RangeSliderCtrl(CDataExchange *pDX, int nIDC, double& valMin, double& valMax);


class CRangeSliderCtrl : public CWnd
{
// Construction
public:
	CRangeSliderCtrl();
	virtual ~CRangeSliderCtrl();

	static BOOL RegisterControlClass(void); // Make it work as custom control

	void Create(DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID, CCreateContext *pContext = NULL);

    void SetGranularity(double grainy) { m_grainy = grainy;} //…Ë÷√ø≈¡£∂»
    void SetHandleSize(int thick) { m_handleThick = thick; }

    void SetControlRange(double min, double max); 
	void GetControlRange(double &min, double &max) 
    { 
		min = m_Min; 
		max = m_Max; 
	}

	void SetValueRange(double valueMin, double valueMax);
	void GetValueRange(double &valueMin, double &valueMax);
    double GetValueMin();
    double GetValueMax();

	void SetVerticalMode(BOOL bVerticalMode = TRUE);
	BOOL GetVerticalMode(void) { return !m_bHorizontal; }

    //for debug use only
	void SetInternalRange(double left, double right); 
	void GetInternalRange(double &left, double &right)
    { 
		left = m_Left; 
		right = m_Right; 
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRangeSliderCtrl)
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:
	//{{AFX_MSG(CRangeSliderCtrl)
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg UINT OnGetDlgCode();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	// Data
	double m_Min, m_Max;           // Outer Edges of the Control
	double m_Left, m_Right;        // Position of Arrows
    double m_grainy;

	int m_handleThick; 

	BOOL m_bHorizontal;         // Horinzontal or Vertical Display of the range.
	void PaintBackground(CDC& dc, const CRect& rc);
	void PaintHandle(CDC& dc, CRect& rcHandle, MOUSE_HIT mode, CBrush *pBkGnd, CPen *pLightPen, CPen *pDarkPen);
	void PaintMiddleBar(CDC& dc, CRect& rcMid);
	void PaintRangeSliderCtrl(CDC& dc, const CRect& rcClipbox);
    int GetScrollClient();
    int GetRangeRect(CRect& rcLeft, CRect& rcRight, CRect& rcMid);
	void RangeNormalize();
	// Tracking the Mouse
	BOOL m_bTracking;
    MOUSE_HIT m_mouseHit;
    MOUSE_HIT m_TrackMode;
	CPoint m_ClickOffset;          // Drag mouse at point of first click.

    HCURSOR m_hHorSizer;
    HCURSOR m_hVerSizer;
    HCURSOR m_hAllSizer;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RANGESLIDER_H__CEC8D48F_0FDC_415C_A751_3AF12AF8B736__INCLUDED_)
