// PowerModePage.cpp : implementation file
//

#include "stdafx.h"
#include "TabSiPlus.h"
#include "PowerModePage.h"
#include "OptionSheet.h"
#include "WndUIResource.h"
#include "PowerModeConfig.h"


// CPowerModePage dialog

IMPLEMENT_DYNAMIC(CPowerModePage, CPropertyPage)

CPowerModePage::CPowerModePage()
	: CPropertyPage(CPowerModePage::IDD)
{
    CWndUIResource *pUIRes = GlobalGetUIRes();

    CPowerModeConfig *pmCfg = GlobalPowerModeConfig();

    m_colorMode = pmCfg->GetColorMode();
    m_crInitial = pmCfg->GetInitialColor();
    m_crFadeout = pmCfg->GetFadeoutColor();
}

CPowerModePage::~CPowerModePage()
{
}

void CPowerModePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPowerModePage)
    DDX_Control(pDX, IDC_COLORPICKER_INITIAL, m_InitialPicker);
    DDX_Control(pDX, IDC_COLORPICKER_FADEOUT, m_FadeoutPicker);
    //}}AFX_DATA_MAP
    DDX_ColourPicker(pDX, IDC_COLORPICKER_INITIAL, m_crInitial);
    DDX_ColourPicker(pDX, IDC_COLORPICKER_FADEOUT, m_crFadeout);
}


BEGIN_MESSAGE_MAP(CPowerModePage, CPropertyPage)
    ON_BN_CLICKED(IDC_RAD_RANDOM_COLOR, &CPowerModePage::OnBnClickedRadRandomColor)
    ON_BN_CLICKED(IDC_RAD_STATIC_COLOR, &CPowerModePage::OnBnClickedRadStaticColor)
    ON_MESSAGE(CPN_SELENDOK,     OnColorPickerSelEndOK)
    ON_MESSAGE(CPN_SELENDCANCEL, OnColorPickerSelEndCancel)
    ON_MESSAGE(CPN_SELCHANGE,    OnColorPickerSelChange)
    ON_MESSAGE(CPN_CLOSEUP,      OnColorPickerCloseUp)
    ON_MESSAGE(CPN_DROPDOWN,     OnColorPickerDropDown)
END_MESSAGE_MAP()

// CPowerModePage message handlers
BOOL CPowerModePage::OnInitDialog() 
{
    CPropertyPage::OnInitDialog();
    
    UpdateData(FALSE);

    m_InitialPicker.SetSelectionMode(CP_MODE_BK);
    m_InitialPicker.SetTrackSelection(TRUE);

    m_FadeoutPicker.SetSelectionMode(CP_MODE_BK);
    m_FadeoutPicker.SetTrackSelection(TRUE);

    int nIDCheckButton = (m_colorMode == 0) ? IDC_RAD_STATIC_COLOR : IDC_RAD_RANDOM_COLOR;
    CheckRadioButton(IDC_RAD_RANDOM_COLOR, IDC_RAD_STATIC_COLOR, nIDCheckButton);

    GetDlgItem(IDC_COLORPICKER_INITIAL)->EnableWindow(m_colorMode == 0);
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPowerModePage::OnApply() 
{
    UpdateData(TRUE);

    SetRegistryRootKey(HKEY_CURRENT_USER);
    SetIntegerRegPorpValue(lpszKeyRoot, lpszPowerColorMode, m_colorMode);
    SetColorRegPorpValue(lpszKeyRoot,lpszPowerInitialColor,m_crInitial);
    SetColorRegPorpValue(lpszKeyRoot,lpszPowerFadeoutColor,m_crFadeout);

    CWndUIResource *pUIRes = GlobalGetUIRes();

    CPowerModeConfig *pmCfg = GlobalPowerModeConfig();

    pmCfg->SetColorMode(m_colorMode);
    pmCfg->SetInitialColor(m_crInitial);
    pmCfg->SetFadeoutColor(m_crFadeout);

    return CPropertyPage::OnApply();
}

void CPowerModePage::OnBnClickedRadRandomColor()
{
    m_colorMode = 1;
    GetDlgItem(IDC_COLORPICKER_INITIAL)->EnableWindow(FALSE);
    SetModified();
}

void CPowerModePage::OnBnClickedRadStaticColor()
{
    m_colorMode = 0;
    GetDlgItem(IDC_COLORPICKER_INITIAL)->EnableWindow(TRUE);
    SetModified();
}

LONG CPowerModePage::OnColorPickerSelEndOK(UINT lParam, LONG wParam)
{
    if(wParam == IDC_COLORPICKER_INITIAL || wParam == IDC_COLORPICKER_FADEOUT)
    {
        SetModified();
    }
    return TRUE;
}

LONG CPowerModePage::OnColorPickerSelEndCancel(UINT lParam, LONG wParam)
{
    return TRUE;
}

LONG CPowerModePage::OnColorPickerSelChange(UINT lParam, LONG wParam)
{

    return TRUE;
}

LONG CPowerModePage::OnColorPickerCloseUp(UINT lParam, LONG wParam)
{
    return TRUE;
}

LONG CPowerModePage::OnColorPickerDropDown(UINT lParam, LONG wParam)
{
    return TRUE;
}
