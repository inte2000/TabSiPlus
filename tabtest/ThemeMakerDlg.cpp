// ThemeMakerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tabtest.h"
#include "ThemeMakerDlg.h"
#include "SettingDlg.h"
#include "WzIniFile.h"
#include "Gradient.h"

// CThemeMakerDlg dialog

IMPLEMENT_DYNAMIC(CThemeMakerDlg, CDialog)

CThemeMakerDlg::CThemeMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CThemeMakerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pTabTheme = NULL;
    m_TabImages = NULL;
}

CThemeMakerDlg::~CThemeMakerDlg()
{
}

void CThemeMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

void CThemeMakerDlg::InitWzTabCtrl()
{
    m_pTabTheme = new CWzTabTestTheme();
    if(m_pTabTheme == NULL)
    {
        AfxMessageBox(_T("内存错误，无法创建Color Theme对象！"));
        return;
    }
	m_TabImages = ImageList_Create(16,16,ILC_COLOR32|ILC_MASK,0,2);
	HBITMAP hBmp = (HBITMAP)LoadImage(AfxGetResourceHandle(),(LPCTSTR)IDB_FILETYPE,IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
	ImageList_AddMasked(m_TabImages,hBmp,RGB(212,208,200));
	::DeleteObject(hBmp);

    m_pTabTheme->InitDefaultConfig();
    m_pTabTheme->SetImageList(CImageList::FromHandle(m_TabImages));
    m_TabCtrl.SetTabTheme(m_pTabTheme, FALSE);

	m_TabCtrl.SetNotifyWnd(this);
	m_TabCtrl.CreateTabCtrl(this);
	CRect rcClient;
	GetClientRect(&rcClient);
	m_TabCtrl.MoveWindow(rcClient.left + 2,rcClient.top + 32,rcClient.Width() - 4,24);

	m_TabCtrl.InsertItem(0,_T("gpon_profile_public.h"),5, NULL);
	m_TabCtrl.InsertItem(1,_T("gpon_profile_public.c"),1, NULL);
	m_TabCtrl.InsertItem(2,_T("gpon_profile_public.txt"),8, NULL);

	m_TabCtrl.InsertItem(3,_T("gpon_profile_kknd.h"),5, NULL);
	m_TabCtrl.InsertItem(4,_T("gpon_profile_kknd.cpp"),2, NULL);

	m_TabCtrl.InsertItem(3,_T("wz_kknd_otdr.h"),5, NULL);
	m_TabCtrl.InsertItem(4,_T("wz_kknd_otdr.cpp"),2, NULL);

	m_TabCtrl.InsertItem(3,_T("zte_msan_epon.h"),5, NULL);
	m_TabCtrl.InsertItem(4,_T("zte_msan_epon.cpp"),2, NULL);

    m_TabCtrl.SetCurSel(1);
}

void CThemeMakerDlg::OnWaTabRowCountChange(WZTAB_EVENT_PARA *pPara)
{
	int nCx = LOWORD(pPara->para1);
	int nCy = HIWORD(pPara->para1);
	int nRowCount = (int)pPara->para2;

	CRect rcClient;
	GetClientRect(&rcClient);
	if(m_TabCtrl.GetSafeHwnd())
	{
		m_TabCtrl.MoveWindow(rcClient.left + 2,rcClient.top + 32,nCx,nCy);
	}
}

void CThemeMakerDlg::SetButtonText()
{
    ASSERT(m_pTabTheme != NULL);

    GetDlgItem(IDC_BTN_BORMAL_T)->SetWindowText(StringFromColor(m_pTabTheme->m_crTabBkNormalT));
    GetDlgItem(IDC_BTN_BORMAL_C)->SetWindowText(StringFromColor(m_pTabTheme->m_crTabBkNormalC));
    GetDlgItem(IDC_BTN_BORMAL_B)->SetWindowText(StringFromColor(m_pTabTheme->m_crTabBkNormalB));

    SetDlgItemText(IDC_BTN_HOVER_T, StringFromColor(m_pTabTheme->m_crTabBkHoverT));
    SetDlgItemText(IDC_BTN_HOVER_C, StringFromColor(m_pTabTheme->m_crTabBkHoverC));
    SetDlgItemText(IDC_BTN_HOVER_B, StringFromColor(m_pTabTheme->m_crTabBkHoverB));

    SetDlgItemText(IDC_BTN_ACTIVE_T, StringFromColor(m_pTabTheme->m_crTabBkSelectedT));
    SetDlgItemText(IDC_BTN_ACTIVE_C, StringFromColor(m_pTabTheme->m_crTabBkSelectedC));
    SetDlgItemText(IDC_BTN_ACTIVE_B, StringFromColor(m_pTabTheme->m_crTabBkSelectedB));

    SetDlgItemText(IDC_BTN_LAST_ACTIVE_T, StringFromColor(m_pTabTheme->m_crTabBkLastSelectedT));
    SetDlgItemText(IDC_BTN_LAST_ACTIVE_C, StringFromColor(m_pTabTheme->m_crTabBkLastSelectedC));
    SetDlgItemText(IDC_BTN_LAST_ACTIVE_B, StringFromColor(m_pTabTheme->m_crTabBkLastSelectedB));

    SetDlgItemText(IDC_BTN_TEXT_NORMAL, StringFromColor(m_pTabTheme->m_crTabTextNormal));
    SetDlgItemText(IDC_BTN_TEXT_HOVER, StringFromColor(m_pTabTheme->m_crTabTextHover));
    SetDlgItemText(IDC_BTN_TEXT_ACTIVE, StringFromColor(m_pTabTheme->m_crTabTextSelected));
    SetDlgItemText(IDC_BTN_TEXT_LAST_ACTIVE, StringFromColor(m_pTabTheme->m_crTabTextLastSelected));

    SetDlgItemText(IDC_BTN_BORDER_NORMAL, StringFromColor(m_pTabTheme->m_crBorderNormal));
    SetDlgItemText(IDC_BTN_BORDER_HOVER, StringFromColor(m_pTabTheme->m_crBorderHover));
    SetDlgItemText(IDC_BTN_BORDER_ACTIVE, StringFromColor(m_pTabTheme->m_crBorderSelected));
    SetDlgItemText(IDC_BTN_BORDER_LAST_ACTIVE, StringFromColor(m_pTabTheme->m_crBorderLastSelected));
}

CString CThemeMakerDlg::StringFromColor(COLORREF color)
{
    CString colorString;

    colorString.Format(_T("RGB(%d,%d,%d)"), GetRValue(color), GetGValue(color), GetBValue(color));

    return colorString;
}

BEGIN_MESSAGE_MAP(CThemeMakerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_BTN_BORMAL_T, &CThemeMakerDlg::OnBnClickedBtnBormalT)
    ON_BN_CLICKED(IDC_BTN_BORMAL_C, &CThemeMakerDlg::OnBnClickedBtnBormalC)
    ON_BN_CLICKED(IDC_BTN_BORMAL_B, &CThemeMakerDlg::OnBnClickedBtnBormalB)
    ON_BN_CLICKED(IDC_BTN_HOVER_T, &CThemeMakerDlg::OnBnClickedBtnHoverT)
    ON_BN_CLICKED(IDC_BTN_HOVER_C, &CThemeMakerDlg::OnBnClickedBtnHoverC)
    ON_BN_CLICKED(IDC_BTN_HOVER_B, &CThemeMakerDlg::OnBnClickedBtnHoverB)
    ON_BN_CLICKED(IDC_BTN_ACTIVE_T, &CThemeMakerDlg::OnBnClickedBtnActiveT)
    ON_BN_CLICKED(IDC_BTN_ACTIVE_C, &CThemeMakerDlg::OnBnClickedBtnActiveC)
    ON_BN_CLICKED(IDC_BTN_ACTIVE_B, &CThemeMakerDlg::OnBnClickedBtnActiveB)
    ON_BN_CLICKED(IDC_BTN_LAST_ACTIVE_T, &CThemeMakerDlg::OnBnClickedBtnLastActiveT)
    ON_BN_CLICKED(IDC_BTN_LAST_ACTIVE_C, &CThemeMakerDlg::OnBnClickedBtnLastActiveC)
    ON_BN_CLICKED(IDC_BTN_LAST_ACTIVE_B, &CThemeMakerDlg::OnBnClickedBtnLastActiveB)
    ON_BN_CLICKED(IDC_BTN_TEXT_NORMAL, &CThemeMakerDlg::OnBnClickedBtnTextNormal)
    ON_BN_CLICKED(IDC_BTN_TEXT_HOVER, &CThemeMakerDlg::OnBnClickedBtnTextHover)
    ON_BN_CLICKED(IDC_BTN_TEXT_ACTIVE, &CThemeMakerDlg::OnBnClickedBtnTextActive)
    ON_BN_CLICKED(IDC_BTN_TEXT_LAST_ACTIVE, &CThemeMakerDlg::OnBnClickedBtnTextLastActive)
    ON_BN_CLICKED(IDC_BTN_OPEN, &CThemeMakerDlg::OnBnClickedBtnOpen)
    ON_BN_CLICKED(IDC_BTN_FONT, &CThemeMakerDlg::OnBnClickedBtnFont)
    ON_BN_CLICKED(IDC_BTN_OPTION, &CThemeMakerDlg::OnBnClickedBtnOption)
    ON_BN_CLICKED(IDC_BTN_BORDER_NORMAL, &CThemeMakerDlg::OnBnClickedBtnBorderNormal)
    ON_BN_CLICKED(IDC_BTN_BORDER_HOVER, &CThemeMakerDlg::OnBnClickedBtnBorderHover)
    ON_BN_CLICKED(IDC_BTN_BORDER_ACTIVE, &CThemeMakerDlg::OnBnClickedBtnBorderActive)
    ON_BN_CLICKED(IDC_BTN_BORDER_LAST_ACTIVE, &CThemeMakerDlg::OnBnClickedBtnBorderLastActive)
    ON_BN_CLICKED(IDOK, &CThemeMakerDlg::OnBnClickedOk)
    ON_BN_CLICKED(IDCANCEL, &CThemeMakerDlg::OnBnClickedCancel)
	ON_MESSAGE(WZTAB_EVENT_NOTIFY, OnWzTabEventNotify)
END_MESSAGE_MAP()


// CThemeMakerDlg message handlers

BOOL CThemeMakerDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

    InitWzTabCtrl();
    SetButtonText();

    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CThemeMakerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CThemeMakerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CThemeMakerDlg::OnDestroy()
{
    CDialog::OnDestroy();

    // TODO: Add your message handler code here
    if(m_pTabTheme != NULL)
    {
        m_pTabTheme->ReleaseConfig();
        delete m_pTabTheme;
    }
    if(m_TabImages != NULL)
    {
    	ImageList_Destroy(m_TabImages);
        m_TabImages = NULL;
    }
}

LRESULT CThemeMakerDlg::OnWzTabEventNotify(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case EVENT_GROUP_MODE:
			break;
		case EVENT_ITEM_DELETE:
			break;
		case EVENT_ITEM_L_CLICK:
			break;
		case EVENT_ITEM_L_DBCLICK:
			break;
		case EVENT_ITEM_R_CLICK:
			break;
		case EVENT_TIP_NEED:
			break;
		case EVENT_ROW_COUNT_CHANGE:
			OnWaTabRowCountChange((WZTAB_EVENT_PARA *)lParam);
			break;
		case EVENT_ITEM_STATE_CHANGE:
			break;
		default:
			break;
	}

	return 0;
}

static TCHAR szThemeFilter[] = _T("Theme Files (*.ini)|*.ini|All Files (*.*)|*.*||");

void CThemeMakerDlg::OnBnClickedOk()
{
    ASSERT(m_pTabTheme != NULL);
    CFileDialog dlg(FALSE, _T("ini"), NULL, OFN_OVERWRITEPROMPT, szThemeFilter);
    if(dlg.DoModal() == IDOK)
    {
        CString strThemeFileName = dlg.GetPathName();

	    m_pTabTheme->SaveConfig(strThemeFileName);
    }

}

void CThemeMakerDlg::OnBnClickedBtnBormalT()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabBkNormalT, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabBkNormalT = dlg.GetColor();
        GetDlgItem(IDC_BTN_BORMAL_T)->SetWindowText(StringFromColor(m_pTabTheme->m_crTabBkNormalT));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnBormalC()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabBkNormalC, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabBkNormalC = dlg.GetColor();
        GetDlgItem(IDC_BTN_BORMAL_C)->SetWindowText(StringFromColor(m_pTabTheme->m_crTabBkNormalC));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnBormalB()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabBkNormalB, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabBkNormalB = dlg.GetColor();
        GetDlgItem(IDC_BTN_BORMAL_B)->SetWindowText(StringFromColor(m_pTabTheme->m_crTabBkNormalB));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnHoverT()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabBkHoverT, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabBkHoverT = dlg.GetColor();
        SetDlgItemText(IDC_BTN_HOVER_T, StringFromColor(m_pTabTheme->m_crTabBkHoverT));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnHoverC()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabBkHoverC, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabBkHoverC = dlg.GetColor();
        SetDlgItemText(IDC_BTN_HOVER_C, StringFromColor(m_pTabTheme->m_crTabBkHoverC));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnHoverB()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabBkHoverB, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabBkHoverB = dlg.GetColor();
        SetDlgItemText(IDC_BTN_HOVER_B, StringFromColor(m_pTabTheme->m_crTabBkHoverB));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnActiveT()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabBkSelectedT, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabBkSelectedT = dlg.GetColor();
        SetDlgItemText(IDC_BTN_ACTIVE_T, StringFromColor(m_pTabTheme->m_crTabBkSelectedT));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnActiveC()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabBkSelectedC, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabBkSelectedC = dlg.GetColor();
        SetDlgItemText(IDC_BTN_ACTIVE_C, StringFromColor(m_pTabTheme->m_crTabBkSelectedC));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnActiveB()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabBkSelectedB, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabBkSelectedB = dlg.GetColor();
        SetDlgItemText(IDC_BTN_ACTIVE_B, StringFromColor(m_pTabTheme->m_crTabBkSelectedB));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnLastActiveT()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabBkLastSelectedT, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabBkLastSelectedT = dlg.GetColor();
        SetDlgItemText(IDC_BTN_LAST_ACTIVE_T, StringFromColor(m_pTabTheme->m_crTabBkLastSelectedT));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnLastActiveC()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabBkLastSelectedC, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabBkLastSelectedC = dlg.GetColor();
        SetDlgItemText(IDC_BTN_LAST_ACTIVE_C, StringFromColor(m_pTabTheme->m_crTabBkLastSelectedC));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnLastActiveB()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabBkLastSelectedB, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabBkLastSelectedB = dlg.GetColor();
        SetDlgItemText(IDC_BTN_LAST_ACTIVE_B, StringFromColor(m_pTabTheme->m_crTabBkLastSelectedB));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnTextNormal()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabTextNormal, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabTextNormal = dlg.GetColor();
        SetDlgItemText(IDC_BTN_TEXT_NORMAL, StringFromColor(m_pTabTheme->m_crTabTextNormal));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnTextHover()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabTextHover, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabTextHover = dlg.GetColor();
        SetDlgItemText(IDC_BTN_TEXT_HOVER, StringFromColor(m_pTabTheme->m_crTabTextHover));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnTextActive()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabTextSelected, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabTextSelected = dlg.GetColor();
        SetDlgItemText(IDC_BTN_TEXT_ACTIVE, StringFromColor(m_pTabTheme->m_crTabTextSelected));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnTextLastActive()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crTabTextLastSelected, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crTabTextLastSelected = dlg.GetColor();
        SetDlgItemText(IDC_BTN_TEXT_LAST_ACTIVE, StringFromColor(m_pTabTheme->m_crTabTextLastSelected));
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnOpen()
{
    CFileDialog dlg(TRUE, _T("ini"), NULL, OFN_FILEMUSTEXIST, szThemeFilter);
    if(dlg.DoModal() == IDOK)
    {
        CString strThemeFileName = dlg.GetPathName();
        CWzTabTestTheme *pTabTheme = new CWzTabTestTheme();
        if(pTabTheme == NULL)
        {
            AfxMessageBox(_T("内存错误，无法创建Color Theme对象！"));
            return;
        }
        pTabTheme->SetImageList(CImageList::FromHandle(m_TabImages));

        if(!pTabTheme->InitConfig(strThemeFileName))
        {
            AfxMessageBox(_T("配置Color Theme对象错误，不是合法的Color Theme文件！"));
            delete pTabTheme;
            return;
        }
        CWzTabTestTheme *pOldTheme = (CWzTabTestTheme *)m_TabCtrl.SetTabTheme(pTabTheme, TRUE);
        if(pOldTheme != NULL)
        {
            pOldTheme->ReleaseConfig();
            delete pOldTheme;
        }
        m_pTabTheme = pTabTheme;
        SetButtonText();
	    CRect rcClient;
	    GetClientRect(&rcClient);
		CSize st = m_TabCtrl.ReCalcAllTabsPosAndWndSize(rcClient.Width() - 12);
        m_TabCtrl.MoveWindow(rcClient.left + 2,rcClient.top + 32,st.cx,st.cy);
    }
}

void CThemeMakerDlg::OnBnClickedBtnFont()
{
    ASSERT(m_pTabTheme !=  NULL);

    LOGFONT lf;
    m_pTabTheme->m_TextFont.GetLogFont(&lf);
    CFontDialog dlg(&lf);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_fontName = dlg.GetFaceName();
        m_pTabTheme->m_fontSize = dlg.GetSize();
        m_pTabTheme->m_fontBold = dlg.GetWeight();
        m_pTabTheme->ResetTextfont();

	    CRect rcClient;
	    GetClientRect(&rcClient);
		CSize st = m_TabCtrl.ReCalcAllTabsPosAndWndSize(rcClient.Width() - 12);
        m_TabCtrl.MoveWindow(rcClient.left + 2,rcClient.top + 32,st.cx,st.cy);

        //Invalidate();
	    //CRect rcClient;
	    //GetClientRect(&rcClient);
	    //m_TabCtrl.MoveWindow(rcClient.left + 2,rcClient.top + 32,rcClient.Width() - 4,24);
    }
}

/*
// Show the font dialog with 12 point "Times New Roman" as the 
// selected font.
LOGFONT lf;
memset(&lf, 0, sizeof(LOGFONT));

CClientDC dc(this);
lf.lfHeight = -MulDiv(12, dc.GetDeviceCaps(LOGPIXELSY), 72);
_tcscpy_s(lf.lfFaceName, LF_FACESIZE, _T("Times New Roman"));

CFontDialog fdlg(&lf);
fdlg.DoModal();

*/

void CThemeMakerDlg::OnBnClickedBtnOption()
{
    CSettingDlg dlg;
    dlg.m_bGroupSameName = m_TabCtrl.GetGroupSameName();
    dlg.m_bShowCloseBtn = m_TabCtrl.GetShowCloseBtn();
    if(dlg.DoModal() == IDOK)
    {
         m_TabCtrl.SetGroupSameName(dlg.m_bGroupSameName);
         m_TabCtrl.SetShowCloseBtn(dlg.m_bShowCloseBtn);
    }
}

void CThemeMakerDlg::OnBnClickedCancel()
{
    // TODO: Add your control notification handler code here
    OnCancel();
}

void CThemeMakerDlg::OnBnClickedBtnBorderNormal()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crBorderNormal, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crBorderNormal = dlg.GetColor();
        SetDlgItemText(IDC_BTN_BORDER_NORMAL, StringFromColor(m_pTabTheme->m_crBorderNormal));
        m_pTabTheme->ResetNormalBorderPen();
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnBorderHover()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crBorderHover, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crBorderHover = dlg.GetColor();
        SetDlgItemText(IDC_BTN_BORDER_HOVER, StringFromColor(m_pTabTheme->m_crBorderHover));
        m_pTabTheme->ResetHoverBorderPen();
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnBorderActive()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crBorderSelected, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crBorderSelected = dlg.GetColor();
        SetDlgItemText(IDC_BTN_BORDER_ACTIVE, StringFromColor(m_pTabTheme->m_crBorderSelected));
        m_pTabTheme->ResetSelectedBorderPen();
        Invalidate();
    }
}

void CThemeMakerDlg::OnBnClickedBtnBorderLastActive()
{
    ASSERT(m_pTabTheme != NULL);
    CColorDialog dlg(m_pTabTheme->m_crBorderLastSelected, CC_FULLOPEN);
    if(dlg.DoModal() == IDOK)
    {
        m_pTabTheme->m_crBorderLastSelected = dlg.GetColor();
        SetDlgItemText(IDC_BTN_BORDER_LAST_ACTIVE, StringFromColor(m_pTabTheme->m_crBorderLastSelected));
        m_pTabTheme->ResetLastSelectedBorderPen();
        Invalidate();
    }
}
