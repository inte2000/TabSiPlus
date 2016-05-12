// tabtestDlg.h : header file
//

#if !defined(AFX_TABTESTDLG_H__3C31D1D3_1CE7_4C98_A4AD_374BAE53407B__INCLUDED_)
#define AFX_TABTESTDLG_H__3C31D1D3_1CE7_4C98_A4AD_374BAE53407B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "WzTabCtrl.h"
#include "WzTabGradientTheme.h"

#include "afxwin.h"

/////////////////////////////////////////////////////////////////////////////
// CTabtestDlg dialog

class CTabtestDlg : public CDialog
{
// Construction
public:
	CTabtestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CTabtestDlg)
	enum { IDD = IDD_TABTEST_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	CListBox m_lstMessge;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabtestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

	void OnWaTabItemGroupMode(WZTAB_EVENT_PARA *pPara);
	void OnWaTabItemDelete(WZTAB_EVENT_PARA *pPara);
	void OnWaTabItemLClick(WZTAB_EVENT_PARA *pPara);
	void OnWaTabItemLDbClick(WZTAB_EVENT_PARA *pPara);
	void OnWaTabItemRClick(WZTAB_EVENT_PARA *pPara);
	void OnWaTabItemTipTextNeed(WZTAB_EVENT_PARA *pPara);
	void OnWaTabRowCountChange(WZTAB_EVENT_PARA *pPara);
	void OnWaTabItemStateChange(WZTAB_EVENT_PARA *pPara);

// Implementation
protected:
	HICON m_hIcon;
	HIMAGELIST m_TabImages;
	CWzTabCtrl m_TabCtrl;
    CWzTabGraidentTheme m_TabTheme;
	// Generated message map functions
	//{{AFX_MSG(CTabtestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBtnAdditems();
	afx_msg void OnBtnGroup();
	afx_msg void OnBtnShowclose();
	afx_msg void OnBnClickedBtnDeleteall();
	afx_msg void OnBnClickedBtnDeleteitem();
	afx_msg LRESULT OnWzTabEventNotify(WPARAM wParam, LPARAM lParam);
    afx_msg void OnBnClickedBtnTheme();
    afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABTESTDLG_H__3C31D1D3_1CE7_4C98_A4AD_374BAE53407B__INCLUDED_)
