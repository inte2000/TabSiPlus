// GeneralPage.cpp : implementation file
/*********************************************************************
*    Author         : orbit
*    Date           : November 15, 2000
*    Last-modified  : June 4, 2002 
*    Contact us     : inte2000@163.com
**********************************************************************/

#include "stdafx.h"
#include "tabsiplus.h"
#include "GeneralPage.h"
#include "OptionSheet.h"
#include "WndUIResource.h"
//#include "Globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage property page

IMPLEMENT_DYNCREATE(CGeneralPage, CPropertyPage)

CGeneralPage::CGeneralPage() : CPropertyPage(CGeneralPage::IDD)
{
    //{{AFX_DATA_INIT(CGeneralPage)
    //}}AFX_DATA_INIT
    m_bSmartFlip = cfg_bSmartFlip;

    SetRegistryRootKey(HKEY_CURRENT_USER);
    GetBoolRegPorpValue(lpszKeyRoot,lpszAutoRunPropName,m_bAutoRun,TRUE);
    GetBoolRegPorpValue(lpszKeyRoot,lpszLoadOncePropName,m_bLoadOnceOnly,FALSE);
    GetBoolRegPorpValue(lpszKeyRoot,lpszShowSplashPropName,m_bShowSplash,TRUE);
    GetStringRegPorpValue(lpszKeyRoot,lpszSiLocation,m_strSiLocation.GetBuffer(MAX_PATH),MAX_PATH);
    m_strSiLocation.ReleaseBuffer();

    if(m_strSiLocation.IsEmpty())
    {
        GetSiProgramLocation(m_strSiLocation.GetBuffer(MAX_PATH),MAX_PATH);
        m_strSiLocation.ReleaseBuffer();
    }

    CWndUIResource *pUIRes = GlobalGetUIRes();
    WZ_ASSERT(pUIRes);

    m_crMenuTextHi = pUIRes->GetColor(pResMenuTextHilight);
    m_crMenuText = pUIRes->GetColor(pResMenuTextNormal);
    m_crMenuLeftbar = pUIRes->GetColor(pResMebuLeftbar);
    m_crMenuFrame = pUIRes->GetColor(pResCtrlRectFrameHilight);
    m_crMenuBkgndHi = pUIRes->GetColor(pResMenuBkgndHilight);
    m_crMenuBkgnd = pUIRes->GetColor(pResMenuBkgndNormal);
}

CGeneralPage::~CGeneralPage()
{
}

void CGeneralPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGeneralPage)
    DDX_Control(pDX, IDC_COLORPICKER_MENUTEXTHI, m_MenuTextHi);
    DDX_Control(pDX, IDC_COLORPICKER_MENUTEXT, m_MenuText);
    DDX_Control(pDX, IDC_COLORPICKER_MENULEFT, m_MenuLeftbar);
    DDX_Control(pDX, IDC_COLORPICKER_MENUFRAME, m_MenuFrame);
    DDX_Control(pDX, IDC_COLORPICKER_MENUBKGNDHI, m_MenuBkgndHi);
    DDX_Control(pDX, IDC_COLORPICKER_MENUBKGND, m_MenuBkGnd);
    DDX_Check(pDX, IDC_CHK_AUTORUN, m_bAutoRun);
    DDX_Check(pDX, IDC_CHK_ONLYONCE, m_bLoadOnceOnly);
    DDX_Check(pDX, IDC_CHK_SHOESPLASH, m_bShowSplash);
    DDX_Check(pDX, IDC_CHK_SMARTFLIP, m_bSmartFlip);
    DDX_Text(pDX, IDC_EDIT_SILOCATION, m_strSiLocation);
    DDV_MaxChars(pDX, m_strSiLocation, 256);
    //}}AFX_DATA_MAP
    DDX_ColourPicker(pDX, IDC_COLORPICKER_MENUTEXTHI, m_crMenuTextHi);
    DDX_ColourPicker(pDX, IDC_COLORPICKER_MENUTEXT, m_crMenuText);
    DDX_ColourPicker(pDX, IDC_COLORPICKER_MENULEFT, m_crMenuLeftbar);
    DDX_ColourPicker(pDX, IDC_COLORPICKER_MENUFRAME, m_crMenuFrame);
    DDX_ColourPicker(pDX, IDC_COLORPICKER_MENUBKGNDHI, m_crMenuBkgndHi);
    DDX_ColourPicker(pDX, IDC_COLORPICKER_MENUBKGND, m_crMenuBkgnd);
}

BEGIN_MESSAGE_MAP(CGeneralPage, CPropertyPage)
    //{{AFX_MSG_MAP(CGeneralPage)
    ON_BN_CLICKED(IDC_CHK_AUTORUN, OnChkAutorun)
    ON_BN_CLICKED(IDC_CHK_SHOESPLASH, OnChkShoesplash)
    ON_BN_CLICKED(IDC_CHK_SMARTFLIP, OnChkSmartflip)
    ON_BN_CLICKED(IDC_CHK_ONLYONCE, OnChkOnlyonce)
    //}}AFX_MSG_MAP
    ON_MESSAGE(CPN_SELENDOK,     OnColorPickerSelEndOK)
    ON_MESSAGE(CPN_SELENDCANCEL, OnColorPickerSelEndCancel)
    ON_MESSAGE(CPN_SELCHANGE,    OnColorPickerSelChange)
    ON_MESSAGE(CPN_CLOSEUP,      OnColorPickerCloseUp)
    ON_MESSAGE(CPN_DROPDOWN,     OnColorPickerDropDown)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralPage message handlers

BOOL CGeneralPage::OnInitDialog() 
{
    CPropertyPage::OnInitDialog();
    
    UpdateData(FALSE);
    
    CString strDefaultText,strCustomText;
    strDefaultText.LoadString(IDS_COLORPICKER_DEFAULT_TEXT);
    strCustomText.LoadString(IDS_COLORPICKER_CUSTOM_TEXT);

    m_MenuTextHi.SetDefaultText(strDefaultText);
    m_MenuTextHi.SetCustomText(strCustomText);
    m_MenuText.SetDefaultText(strDefaultText);
    m_MenuText.SetCustomText(strCustomText);
    m_MenuLeftbar.SetDefaultText(strDefaultText);
    m_MenuLeftbar.SetCustomText(strCustomText);
    m_MenuFrame.SetDefaultText(strDefaultText);
    m_MenuFrame.SetCustomText(strCustomText);
    m_MenuBkgndHi.SetDefaultText(strDefaultText);
    m_MenuBkgndHi.SetCustomText(strCustomText);
    m_MenuBkGnd.SetDefaultText(strDefaultText);
    m_MenuBkGnd.SetCustomText(strCustomText);

    m_MenuTextHi.SetSelectionMode(CP_MODE_BK);
    m_MenuText.SetSelectionMode(CP_MODE_BK);
    m_MenuLeftbar.SetSelectionMode(CP_MODE_BK);
    m_MenuFrame.SetSelectionMode(CP_MODE_BK);
    m_MenuBkgndHi.SetSelectionMode(CP_MODE_BK);
    m_MenuBkGnd.SetSelectionMode(CP_MODE_BK);

    m_MenuTextHi.SetTrackSelection(TRUE);
    m_MenuText.SetTrackSelection(TRUE);
    m_MenuLeftbar.SetTrackSelection(TRUE);
    m_MenuFrame.SetTrackSelection(TRUE);
    m_MenuBkgndHi.SetTrackSelection(TRUE);
    m_MenuBkGnd.SetTrackSelection(TRUE);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CGeneralPage::OnApply() 
{
    UpdateData(TRUE);

    CWndUIResource *pUIRes = GlobalGetUIRes();
    WZ_ASSERT(pUIRes);

    pUIRes->SetColor(pResMenuTextHilight,m_crMenuTextHi);
    pUIRes->SetColor(pResMenuTextNormal,m_crMenuText);
    pUIRes->SetColor(pResMebuLeftbar,m_crMenuLeftbar);
    pUIRes->SetColor(pResCtrlRectFrameHilight,m_crMenuFrame);
    pUIRes->SetColor(pResMenuBkgndHilight,m_crMenuBkgndHi);
    pUIRes->SetColor(pResMenuBkgndNormal,m_crMenuBkgnd);

    cfg_bSmartFlip = m_bSmartFlip;

    SetRegistryRootKey(HKEY_CURRENT_USER);
    SetBoolRegPorpValue(lpszKeyRoot,lpszAutoRunPropName,m_bAutoRun);
    SetBoolRegPorpValue(lpszKeyRoot,lpszLoadOncePropName,m_bLoadOnceOnly);
    SetBoolRegPorpValue(lpszKeyRoot,lpszShowSplashPropName,m_bShowSplash);
    SetBoolRegPorpValue(lpszKeyRoot,lpszSmartFlip,m_bSmartFlip);
    SetStringRegPorpValue(lpszKeyRoot,lpszSiLocation,m_strSiLocation);

    SetColorRegPorpValue(lpszKeyRoot,pResMenuTextHilight,m_crMenuTextHi);
    SetColorRegPorpValue(lpszKeyRoot,pResMenuTextNormal,m_crMenuText);
    SetColorRegPorpValue(lpszKeyRoot,pResMebuLeftbar,m_crMenuLeftbar);
    SetColorRegPorpValue(lpszKeyRoot,pResCtrlRectFrameHilight,m_crMenuFrame);
    SetColorRegPorpValue(lpszKeyRoot,pResMenuBkgndHilight,m_crMenuBkgndHi);
    SetColorRegPorpValue(lpszKeyRoot,pResMenuBkgndNormal,m_crMenuBkgnd);

    SetWindowsAutoRunKey(m_bAutoRun);

    return CPropertyPage::OnApply();
}

LONG CGeneralPage::OnColorPickerSelEndOK(UINT lParam, LONG wParam)
{
    if(wParam == IDC_COLORPICKER_MENUTEXTHI || wParam == IDC_COLORPICKER_MENUTEXT 
        || wParam == IDC_COLORPICKER_MENULEFT || wParam == IDC_COLORPICKER_MENUFRAME
            || wParam == IDC_COLORPICKER_MENUBKGNDHI || wParam == IDC_COLORPICKER_MENUBKGND )
        SetModified();
    return TRUE;
}

LONG CGeneralPage::OnColorPickerSelEndCancel(UINT lParam, LONG wParam)
{
    return TRUE;
}

LONG CGeneralPage::OnColorPickerSelChange(UINT lParam, LONG wParam)
{

    return TRUE;
}

LONG CGeneralPage::OnColorPickerCloseUp(UINT lParam, LONG wParam)
{
    return TRUE;
}

LONG CGeneralPage::OnColorPickerDropDown(UINT lParam, LONG wParam)
{
    return TRUE;
}

void CGeneralPage::OnChkAutorun() 
{
    SetModified();
}

void CGeneralPage::OnChkShoesplash() 
{
    SetModified();
}

void CGeneralPage::OnChkSmartflip() 
{
    SetModified();
}

void CGeneralPage::OnChkOnlyonce() 
{
    SetModified();
}
