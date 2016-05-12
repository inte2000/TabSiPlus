#pragma once

#include "WzTabCtrl.h"
#include "WzTabTestTheme.h"

// CThemeMakerDlg dialog

class CThemeMakerDlg : public CDialog
{
	DECLARE_DYNAMIC(CThemeMakerDlg)

public:
	CThemeMakerDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CThemeMakerDlg();

// Dialog Data
	enum { IDD = IDD_DLG_THEME_MAKER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedOk();
    afx_msg void OnDestroy();
    afx_msg void OnBnClickedBtnBormalT();
    afx_msg void OnBnClickedBtnBormalC();
    afx_msg void OnBnClickedBtnBormalB();
    afx_msg void OnBnClickedBtnHoverT();
    afx_msg void OnBnClickedBtnHoverC();
    afx_msg void OnBnClickedBtnHoverB();
    afx_msg void OnBnClickedBtnActiveT();
    afx_msg void OnBnClickedBtnActiveC();
    afx_msg void OnBnClickedBtnActiveB();
    afx_msg void OnBnClickedBtnLastActiveT();
    afx_msg void OnBnClickedBtnLastActiveC();
    afx_msg void OnBnClickedBtnLastActiveB();
    afx_msg void OnBnClickedBtnTextNormal();
    afx_msg void OnBnClickedBtnTextHover();
    afx_msg void OnBnClickedBtnTextActive();
    afx_msg void OnBnClickedBtnTextLastActive();
    afx_msg void OnBnClickedBtnOpen();
    afx_msg void OnBnClickedBtnFont();
    afx_msg void OnBnClickedBtnOption();
    afx_msg void OnBnClickedCancel();
    afx_msg void OnBnClickedBtnBorderNormal();
    afx_msg void OnBnClickedBtnBorderHover();
    afx_msg void OnBnClickedBtnBorderActive();
    afx_msg void OnBnClickedBtnBorderLastActive();
	afx_msg LRESULT OnWzTabEventNotify(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	void OnWaTabRowCountChange(WZTAB_EVENT_PARA *pPara);

    void InitWzTabCtrl();
    void SetButtonText();
    CString StringFromColor(COLORREF color);

protected:
	HICON m_hIcon;
	CWzTabCtrl m_TabCtrl;
	HIMAGELIST m_TabImages;
    CWzTabTestTheme *m_pTabTheme;
};
