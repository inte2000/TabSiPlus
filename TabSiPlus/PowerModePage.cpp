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

    m_iUsePowerMode = cfg_bUsePowerMode ? 1 : 0;
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
    DDX_Check(pDX, IDC_CHK_USE_POWERMODE, m_iUsePowerMode);
    //}}AFX_DATA_MAP
    DDX_ColourPicker(pDX, IDC_COLORPICKER_INITIAL, m_crInitial);
    DDX_ColourPicker(pDX, IDC_COLORPICKER_FADEOUT, m_crFadeout);
    DDX_Control(pDX, IDC_PARTNUM_RANGE_CTRL, m_PartCountRange);
    DDX_Control(pDX, IDC_VX_RANGE_CTRL, m_PartVxRange);
    DDX_Control(pDX, IDC_VY_RANGE_CTRL, m_PartVyRange);
}


BEGIN_MESSAGE_MAP(CPowerModePage, CPropertyPage)
    ON_BN_CLICKED(IDC_RAD_RANDOM_COLOR, &CPowerModePage::OnBnClickedRadRandomColor)
    ON_BN_CLICKED(IDC_RAD_STATIC_COLOR, &CPowerModePage::OnBnClickedRadStaticColor)
    ON_MESSAGE(CPN_SELENDOK,     OnColorPickerSelEndOK)
    ON_MESSAGE(CPN_SELENDCANCEL, OnColorPickerSelEndCancel)
    ON_MESSAGE(CPN_SELCHANGE,    OnColorPickerSelChange)
    ON_MESSAGE(CPN_CLOSEUP,      OnColorPickerCloseUp)
    ON_MESSAGE(CPN_DROPDOWN,     OnColorPickerDropDown)
    ON_REGISTERED_MESSAGE(RANGE_VALUE_CHANGED, OnRangeValueChange)
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

    m_PartCountRange.SetControlRange(5.0, 50.0);
    m_PartCountRange.SetGranularity(1.0);
    m_PartCountRange.SetValueRange(cfg_uPartNumMin, cfg_uPartNumMax);

    SetDlgItemInt(IDC_EDIT_PARTICLE_MIN, (UINT)cfg_uPartNumMin, FALSE);
    SetDlgItemInt(IDC_EDIT_PARTICLE_MAX, (UINT)cfg_uPartNumMax, FALSE);

    m_PartVxRange.SetControlRange(0.0, 4.0); //[-2.0 ,2.0]
    m_PartVxRange.SetGranularity(0.01);
    double vxmin = double(cfg_uPartVxMin) / 1000.0;
    double vxmax = double(cfg_uPartVxMax) / 1000.0;
    m_PartVxRange.SetValueRange(vxmin, vxmax);

    CString strTmp;
    strTmp.Format(_T("%6.2f"), vxmin - 2.0);
    SetDlgItemText(IDC_EDIT_VX_MIN, strTmp);
    strTmp.Format(_T("%6.2f"), vxmax - 2.0);
    SetDlgItemText(IDC_EDIT_VX_MAX, strTmp);

    m_PartVyRange.SetControlRange(0.0, 5.0); //[-5.0, 0.0]
    m_PartVyRange.SetGranularity(0.01);
    double vymin = double(cfg_uPartVyMin) / 1000.0;
    double vymax = double(cfg_uPartVyMax) / 1000.0;
    m_PartVyRange.SetValueRange(vymin, vymax);

    strTmp.Format(_T("%6.2f"), vymin - 5.0);
    SetDlgItemText(IDC_EDIT_VY_MIN, strTmp);
    strTmp.Format(_T("%6.2f"), vymax - 5.0);
    SetDlgItemText(IDC_EDIT_VY_MAX, strTmp);

    SetDlgItemInt(IDC_EDIT_ACCELERATION, (UINT)cfg_uPartGravity, FALSE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CPowerModePage::OnApply() 
{
    UpdateData(TRUE);

    cfg_bUsePowerMode = (m_iUsePowerMode == 1);
    cfg_uPartGravity = GetDlgItemInt(IDC_EDIT_ACCELERATION, NULL, FALSE);

    SetRegistryRootKey(HKEY_CURRENT_USER);
    SetIntegerRegPorpValue(lpszKeyRoot, lpszPowerColorMode, m_colorMode);
    SetColorRegPorpValue(lpszKeyRoot,lpszPowerInitialColor,m_crInitial);
    SetColorRegPorpValue(lpszKeyRoot,lpszPowerFadeoutColor,m_crFadeout);
    SetBoolRegPorpValue(lpszKeyRoot, lpszUsePowerMode, cfg_bUsePowerMode);
    SetIntegerRegPorpValue(lpszKeyRoot, lpszPartNumMin, cfg_uPartNumMin);
    SetIntegerRegPorpValue(lpszKeyRoot, lpszPartNumMax, cfg_uPartNumMax);
    SetIntegerRegPorpValue(lpszKeyRoot, lpszPartVxMin, cfg_uPartVxMin);
    SetIntegerRegPorpValue(lpszKeyRoot, lpszPartVxMax, cfg_uPartVxMax);
    SetIntegerRegPorpValue(lpszKeyRoot, lpszPartVyMin, cfg_uPartVyMin);
    SetIntegerRegPorpValue(lpszKeyRoot, lpszPartVyMax, cfg_uPartVyMax);
    SetIntegerRegPorpValue(lpszKeyRoot, lpszPartGravity, cfg_uPartGravity);

    CWndUIResource *pUIRes = GlobalGetUIRes();

    CPowerModeConfig *pmCfg = GlobalPowerModeConfig();
    pmCfg->SetColorMode(m_colorMode);
    pmCfg->SetInitialColor(m_crInitial);
    pmCfg->SetFadeoutColor(m_crFadeout);
    pmCfg->SetMinParticles(cfg_uPartNumMin);
    pmCfg->SetMaxParticles(cfg_uPartNumMax);
    pmCfg->SetVxMin(double(cfg_uPartVxMin) / 1000.0 - 2.0);
    pmCfg->SetVxMax(double(cfg_uPartVxMax) / 1000.0 - 2.0);
    pmCfg->SetVyMin(double(cfg_uPartVyMin) / 1000.0 - 5.0);
    pmCfg->SetVyMax(double(cfg_uPartVyMax) / 1000.0 - 5.0);
    pmCfg->SetGravity(double(cfg_uPartGravity) / 1000.0);

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

LRESULT CPowerModePage::OnRangeValueChange(WPARAM wParam, LPARAM lParam)
{
    UINT ctrlID = static_cast<UINT>(wParam);
    
    double valueMin, valueMax;
    if(ctrlID == IDC_PARTNUM_RANGE_CTRL)
    {
        m_PartCountRange.GetValueRange(valueMin, valueMax);
        cfg_uPartNumMin = static_cast<int>(valueMin);
        cfg_uPartNumMax = static_cast<int>(valueMax);
        SetDlgItemInt(IDC_EDIT_PARTICLE_MIN, (UINT)cfg_uPartNumMin, FALSE);
        SetDlgItemInt(IDC_EDIT_PARTICLE_MAX, (UINT)cfg_uPartNumMax, FALSE);
    }
    else if(ctrlID == IDC_VX_RANGE_CTRL)
    {
        m_PartVxRange.GetValueRange(valueMin, valueMax);
        cfg_uPartVxMin = static_cast<int>(valueMin * 1000.0);
        cfg_uPartVxMax = static_cast<int>(valueMax * 1000.0);
        
        CString strTmp;
        strTmp.Format(_T("%6.2f"), valueMin - 2.0);
        SetDlgItemText(IDC_EDIT_VX_MIN, strTmp);
        strTmp.Format(_T("%6.2f"), valueMax - 2.0);
        SetDlgItemText(IDC_EDIT_VX_MAX, strTmp);
    }
    else if(ctrlID == IDC_VY_RANGE_CTRL)
    {
        m_PartVyRange.GetValueRange(valueMin, valueMax);
        cfg_uPartVyMin = static_cast<int>(valueMin * 1000.0);
        cfg_uPartVyMax = static_cast<int>(valueMax * 1000.0);

        CString strTmp;
        strTmp.Format(_T("%6.2f"), valueMin - 5.0);
        SetDlgItemText(IDC_EDIT_VY_MIN, strTmp);
        strTmp.Format(_T("%6.2f"), valueMax - 5.0);
        SetDlgItemText(IDC_EDIT_VY_MAX, strTmp);
    }
    else
    {
    }

	return static_cast<LRESULT>(0);
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
