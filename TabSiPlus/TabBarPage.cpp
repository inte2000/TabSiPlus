// TabBarPage.cpp : implementation file
/*********************************************************************
*    Author         : orbit
*    Date           : November 15, 2000
*    Last-modified  : June 4, 2002 
*    Contact us     : inte2000@163.com
**********************************************************************/

#include "stdafx.h"
#include "tabsiplus.h"
#include "TabBarPage.h"
#include "OptionSheet.h"
#include "GlobalFileList.h"
#include "Globals.h"
#include "WzIniFile.h"
#include "WzTabTheme.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTabBarPage property page

IMPLEMENT_DYNCREATE(CTabBarPage, CPropertyPage)

//##ModelId=431C34C802EE
CTabBarPage::CTabBarPage() : CPropertyPage(CTabBarPage::IDD)
{
    //{{AFX_DATA_INIT(CTabBarPage)
    m_bSmartLoc = FALSE;
    m_uSrhDeep = 0;
    //}}AFX_DATA_INIT
    SetRegistryRootKey(HKEY_CURRENT_USER);
    m_bSmartLoc = cfg_bSmartLoc;
    m_uSrhDeep  = cfg_uSrhDeep;
    m_bShowIcon = cfg_bShowIcons;
    m_bUseShellMenu = !cfg_bNoShellContext;
    m_nAddMode = (int)cfg_iAddOrder;
    m_bGroupSameName = cfg_bGroupSameTable;
    m_bShowCloseBtn = cfg_bShowCloseBtn;
    m_nDockPos = (int)cfg_TabWndPosition;
}

//##ModelId=431C34C802EF
CTabBarPage::~CTabBarPage()
{
}

//##ModelId=431C34C8034D
void CTabBarPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CTabBarPage)
    DDX_Check(pDX, IDC_CHK_SHELLMENU, m_bUseShellMenu);
    DDX_Check(pDX, IDC_CHK_SHOWICON, m_bShowIcon);
    DDX_Radio(pDX, IDC_RAD_ADDF, m_nAddMode);
    DDX_Radio(pDX, IDC_RAD_TOP, m_nDockPos);
    DDX_Check(pDX, IDC_CHK_SMART_LOC, m_bSmartLoc);
    DDX_Text(pDX, IDC_EDIT_SRH_DEEP, m_uSrhDeep);
    DDV_MinMaxUInt(pDX, m_uSrhDeep, 0, 10);
    DDX_Check(pDX, IDC_CHK_GROUP_SAME, m_bGroupSameName);
    DDX_Control(pDX, IDC_CMB_THEME, m_cmb_Theme);
    DDX_Check(pDX, IDC_CHK_SHOW_CLOSE, m_bShowCloseBtn);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTabBarPage, CPropertyPage)
    //{{AFX_MSG_MAP(CTabBarPage)
    ON_BN_CLICKED(IDC_CHK_SHELLMENU, OnChkShellmenu)
    ON_BN_CLICKED(IDC_CHK_SHOWICON, OnChkShowicon)
    ON_EN_CHANGE(IDC_EDIT_MAXWND, OnChangeEditMaxwnd)
    ON_BN_CLICKED(IDC_RAD_ADDB, OnRadAddb)
    ON_BN_CLICKED(IDC_RAD_ADDF, OnRadAddf)
    ON_BN_CLICKED(IDC_RAD_BOTTOM, OnRadBottom)
    ON_BN_CLICKED(IDC_RAD_TOP, OnRadTop)
    ON_BN_CLICKED(IDC_CHK_SMART_LOC, OnChkSmartLoc)
    ON_BN_CLICKED(IDC_CHK_GROUP_SAME, &CTabBarPage::OnBnClickedChkGroupSame)
    ON_CBN_SELCHANGE(IDC_CMB_THEME, &CTabBarPage::OnCbnSelchangeCmbTheme)
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_CHK_SHOW_CLOSE, &CTabBarPage::OnBnClickedChkShowClose)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabBarPage message handlers

//##ModelId=431C34C8035C
BOOL CTabBarPage::OnInitDialog() 
{
    CPropertyPage::OnInitDialog();

    UpdateData(FALSE);

    m_themeInfo.clear();
    FillThemeCombobox();

    GetDlgItem(IDC_EDIT_SRH_DEEP)->EnableWindow(m_bSmartLoc);

    SetModified();
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

//##ModelId=431C34C8034B
BOOL CTabBarPage::OnApply() 
{
    UpdateData(TRUE);
    
    cfg_bShowIcons = m_bShowIcon;
    cfg_bNoShellContext = !m_bUseShellMenu;
    cfg_bSmartLoc = m_bSmartLoc;
    cfg_uSrhDeep  = m_uSrhDeep;
    cfg_iAddOrder = (AddOrder)m_nAddMode;
    cfg_bGroupSameTable = m_bGroupSameName;
    cfg_TabWndPosition = (SnapOrientations)m_nDockPos;
    cfg_bShowCloseBtn = m_bShowCloseBtn;

    SetRegistryRootKey(HKEY_CURRENT_USER);
    
    SetIntegerRegPorpValue(lpszKeyRoot,lpszInitPos,m_nDockPos);
    SetIntegerRegPorpValue(lpszKeyRoot,lpszOrderBy,(int)cfg_iAddOrder);
    SetBoolRegPorpValue(lpszKeyRoot,lpszShowIcon,cfg_bShowIcons);
    SetBoolRegPorpValue(lpszKeyRoot,lpszNoShellMenu,cfg_bNoShellContext);
    
    SetBoolRegPorpValue(lpszKeyRoot,lpszSmartLoc,cfg_bSmartLoc);
    SetIntegerRegPorpValue(lpszKeyRoot,lpszSrhDeep,cfg_uSrhDeep);
    SetBoolRegPorpValue(lpszKeyRoot,lpszcfGroupSame,cfg_bGroupSameTable);
    SetBoolRegPorpValue(lpszKeyRoot,lpszcfCloseBtn,cfg_bShowCloseBtn);
    SetStringRegPorpValue(lpszKeyRoot,lpszThemeName,g_szThemeName);
    
    g_pOpenFileList->SetSearchPathDeep(cfg_uSrhDeep);
    if(!cfg_bSmartLoc)
        g_pOpenFileList->ClearPathList();

    return CPropertyPage::OnApply();
}

//##ModelId=431C34C80399
void CTabBarPage::OnChkShellmenu() 
{
    SetModified();
}

//##ModelId=431C34C8039B
void CTabBarPage::OnChkShowicon() 
{
    m_bShowIcon = !m_bShowIcon;

    COptionSheet *pParentSheet = (COptionSheet *)GetParent();
    ASSERT(pParentSheet);
    CWnd *pTabbarWnd = pParentSheet->m_pNotifyWnd;
    ASSERT(pTabbarWnd);

    pTabbarWnd->SendMessage(WM_OPSHEET2TABBAR_SHOW_ICON,m_bShowIcon,0);

    cfg_bShowIcons = m_bShowIcon;
    SetBoolRegPorpValue(lpszKeyRoot,lpszShowIcon,cfg_bShowIcons);

    SetModified();
}

//##ModelId=431C34C803B9
void CTabBarPage::OnChangeEditMaxwnd() 
{
    SetModified();
}

//##ModelId=431C34C803BB
void CTabBarPage::OnRadAddb() 
{
    SetModified();
}

//##ModelId=431C34C803C8
void CTabBarPage::OnRadAddf() 
{
    SetModified();
}


//##ModelId=431C34C803D8
void CTabBarPage::OnRadBottom() 
{
    COptionSheet *pParentSheet = (COptionSheet *)GetParent();
    ASSERT(pParentSheet);
    CWnd *pTabbarWnd = pParentSheet->m_pNotifyWnd;
    ASSERT(pTabbarWnd);

    if(m_nDockPos != (int)soBottom)
    {
        int oldpos = m_nDockPos;
        m_nDockPos = (int)soBottom;
        cfg_TabWndPosition = (SnapOrientations)m_nDockPos;
        pTabbarWnd->SendMessage(WM_OPSHEET2TABBAR_POS_CHANGE,oldpos,m_nDockPos);
    }

    SetModified();
}

//##ModelId=431C34C803DA
void CTabBarPage::OnRadTop() 
{
    COptionSheet *pParentSheet = (COptionSheet *)GetParent();
    ASSERT(pParentSheet);
    CWnd *pTabbarWnd = pParentSheet->m_pNotifyWnd;
    ASSERT(pTabbarWnd);

    if(m_nDockPos != (int)soTop)
    {
        int oldpos = m_nDockPos;
        m_nDockPos = (int)soTop;
        cfg_TabWndPosition = (SnapOrientations)m_nDockPos;
        pTabbarWnd->SendMessage(WM_OPSHEET2TABBAR_POS_CHANGE,oldpos,m_nDockPos);
    }
    
    SetModified();
}

void CTabBarPage::OnChkSmartLoc() 
{
    m_bSmartLoc = !m_bSmartLoc;
    GetDlgItem(IDC_EDIT_SRH_DEEP)->EnableWindow(m_bSmartLoc);
}

void CTabBarPage::OnBnClickedChkGroupSame()
{
    m_bGroupSameName = !m_bGroupSameName;

    COptionSheet *pParentSheet = (COptionSheet *)GetParent();
    ASSERT(pParentSheet);
    CWnd *pTabbarWnd = pParentSheet->m_pNotifyWnd;
    ASSERT(pTabbarWnd);

    pTabbarWnd->SendMessage(WM_OPSHEET2TABBAR_GROUP_SAME,m_bGroupSameName,0);

    cfg_bGroupSameTable = m_bGroupSameName;
    SetBoolRegPorpValue(lpszKeyRoot,lpszcfGroupSame,cfg_bGroupSameTable);
    SetModified();
}

void CTabBarPage::OnCbnSelchangeCmbTheme()
{
    int curSel = m_cmb_Theme.GetCurSel(); 
    if((curSel >= 0) && (curSel < (int)m_themeInfo.size()))
    {
        if(m_themeInfo[curSel].fileName.CompareNoCase(g_szThemeName) != 0)
        {
            _tcscpy_s(g_szThemeName, 128, m_themeInfo[curSel].fileName);

            COptionSheet *pParentSheet = (COptionSheet *)GetParent();
            ASSERT(pParentSheet);
            CWnd *pTabbarWnd = pParentSheet->m_pNotifyWnd;
            ASSERT(pTabbarWnd);
            
            pTabbarWnd->SendMessage(WM_OPSHEET2TABBAR_THEME_CHANGE,(WPARAM)(LPCTSTR)g_szThemeName,0);

            SetModified();
        }
    }
}

void CTabBarPage::ProcessThemeFile(LPCTSTR lpFullName, LPCTSTR lpFileName)
{
    CWzIniFile ini(lpFullName);
    ini.Open();

    int sign = 0;
    if(ini.GetValue(_T("INFO"), _T("sign"), sign, 0) && (sign == themeFilesign))
    {
        ThemeInfo info;
        ini.GetValue(_T("INFO"), _T("name"), info.name, _T(""));
        info.fileName = lpFileName;
        info.fullPathName = lpFullName;

        m_themeInfo.push_back(info);
        int item = m_cmb_Theme.AddString(info.name);
        if((item >= 0) && (lstrcmpi(g_szThemeName, lpFileName) == 0))
        {
            m_cmb_Theme.SetCurSel(item);
        }
    }
}

void CTabBarPage::FillThemeCombobox()
{
    TCHAR szThemePath[MAX_PATH] = {0};
    GetAppFilePathName(_T(""), szThemePath, MAX_PATH);
    
    TCHAR szRootPath[_MAX_PATH];
    _tcscpy(szRootPath,szThemePath);
    _tcscat(szRootPath,_T("*.ini"));

    WIN32_FIND_DATA data;
    HANDLE hFindFile = FindFirstFile(szRootPath,&data);
    if(hFindFile != INVALID_HANDLE_VALUE)
    {
        TCHAR szSubRootName[_MAX_PATH];
        do
        {
            _tcscpy(szSubRootName,szThemePath);
            _tcscat(szSubRootName,data.cFileName);
            ProcessThemeFile(szSubRootName, data.cFileName);
        }while(FindNextFile(hFindFile,&data));

        FindClose(hFindFile);
    }
}


void CTabBarPage::OnBnClickedChkShowClose()
{
    m_bShowCloseBtn = !m_bShowCloseBtn;

    COptionSheet *pParentSheet = (COptionSheet *)GetParent();
    ASSERT(pParentSheet);
    CWnd *pTabbarWnd = pParentSheet->m_pNotifyWnd;
    ASSERT(pTabbarWnd);

    pTabbarWnd->SendMessage(WM_OPSHEET2TABBAR_SHOW_CLOSE,m_bShowCloseBtn,0);

    cfg_bShowCloseBtn = m_bShowCloseBtn;
    SetBoolRegPorpValue(lpszKeyRoot,lpszcfCloseBtn,cfg_bShowCloseBtn);
    SetModified();
}
