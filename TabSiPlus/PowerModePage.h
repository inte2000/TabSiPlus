#pragma once

#include "ColourPicker.h"
#include "RangeSliderCtrl.h"

// CPowerModePage dialog

class CPowerModePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CPowerModePage)

public:
	CPowerModePage();
	virtual ~CPowerModePage();

    virtual BOOL OnApply();
// Dialog Data
	enum { IDD = IDD_PROPPAGE_POWERMODE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();

    afx_msg void OnBnClickedRadRandomColor();
    afx_msg void OnBnClickedRadStaticColor();
    afx_msg LONG OnColorPickerSelEndOK(UINT lParam, LONG wParam);
    afx_msg LONG OnColorPickerSelEndCancel(UINT lParam, LONG wParam);
    afx_msg LONG OnColorPickerSelChange(UINT lParam, LONG wParam);
    afx_msg LONG OnColorPickerCloseUp(UINT lParam, LONG wParam);
    afx_msg LONG OnColorPickerDropDown(UINT lParam, LONG wParam);
	afx_msg LRESULT OnRangeValueChange(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
    CColourPicker    m_InitialPicker;
    CColourPicker    m_FadeoutPicker;
    COLORREF         m_crInitial;
    COLORREF         m_crFadeout;
    int              m_colorMode;
    int              m_iUsePowerMode;
    CRangeSliderCtrl m_PartCountRange;
    CRangeSliderCtrl m_PartVxRange;
    CRangeSliderCtrl m_PartVyRange;
};
