// tabtestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tabtest.h"
#include "tabtestDlg.h"
#include "ThemeMakerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabtestDlg dialog

CTabtestDlg::CTabtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTabtestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTabtestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_TabImages = NULL;
}

void CTabtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTabtestDlg)
	// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_MESSAGE, m_lstMessge);
}

BEGIN_MESSAGE_MAP(CTabtestDlg, CDialog)
	//{{AFX_MSG_MAP(CTabtestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BTN_ADDITEMS, OnBtnAdditems)
	ON_BN_CLICKED(IDC_BTN_GROUP, OnBtnGroup)
	ON_BN_CLICKED(IDC_BTN_SHOWCLOSE, OnBtnShowclose)
	ON_BN_CLICKED(IDC_BTN_DELETEALL, &CTabtestDlg::OnBnClickedBtnDeleteall)
	ON_BN_CLICKED(IDC_BTN_DELETEITEM, &CTabtestDlg::OnBnClickedBtnDeleteitem)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WZTAB_EVENT_NOTIFY, OnWzTabEventNotify)
    ON_BN_CLICKED(IDC_BTN_THEME, &CTabtestDlg::OnBnClickedBtnTheme)
    ON_WM_DESTROY()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTabtestDlg message handlers
void CTabtestDlg::OnWaTabItemGroupMode(WZTAB_EVENT_PARA *pPara)
{
	CString strMsg;
	strMsg.Format(_T("Group Mode Change : mode = %d, count = %d"),pPara->para1,pPara->para2);
	m_lstMessge.AddString(strMsg);
	m_lstMessge.SetTopIndex(m_lstMessge.GetTopIndex() + 1);
}

void CTabtestDlg::OnWaTabItemDelete(WZTAB_EVENT_PARA *pPara)
{
    m_TabCtrl.DeleteItem(pPara->para1);
	CString strMsg;
	strMsg.Format(_T("ItemDelete : item %d was deleted"),pPara->para1);
	m_lstMessge.AddString(strMsg);
	m_lstMessge.SetTopIndex(m_lstMessge.GetTopIndex() + 1);
}

void CTabtestDlg::OnWaTabItemLClick(WZTAB_EVENT_PARA *pPara)
{
}

void CTabtestDlg::OnWaTabItemLDbClick(WZTAB_EVENT_PARA *pPara)
{
	POINT pt;

	pt.x = LOWORD(pPara->para1);
	pt.y = HIWORD(pPara->para1);
	int nItemId = pPara->para2;

	CString strMsg;
	strMsg.Format(_T("Double Click : item %d on (%d,%d)"),nItemId,pt.x,pt.y);
	m_lstMessge.AddString(strMsg);
	m_lstMessge.SetTopIndex(m_lstMessge.GetTopIndex() + 1);
}

void CTabtestDlg::OnWaTabItemRClick(WZTAB_EVENT_PARA *pPara)
{
	POINT pt;

	pt.x = LOWORD(pPara->para1);
	pt.y = HIWORD(pPara->para1);
	int nItemId = pPara->para2;

	CString strMsg;
	strMsg.Format(_T("Right Click : item %d on (%d,%d)"),nItemId,pt.x,pt.y);
	m_lstMessge.AddString(strMsg);
	m_lstMessge.SetTopIndex(m_lstMessge.GetTopIndex() + 1);

    m_TabCtrl.DeleteItem(nItemId);
}

void CTabtestDlg::OnWaTabItemTipTextNeed(WZTAB_EVENT_PARA *pPara)
{
}

void CTabtestDlg::OnWaTabRowCountChange(WZTAB_EVENT_PARA *pPara)
{
	int nCx = LOWORD(pPara->para1);
	int nCy = HIWORD(pPara->para1);
	int nRowCount = (int)pPara->para2;

	CRect rcClient;
	GetClientRect(&rcClient);
	if(m_TabCtrl.GetSafeHwnd())
	{
		m_TabCtrl.MoveWindow(rcClient.left + 2,rcClient.top + 120,nCx,nCy);
        //m_TabCtrl.UpdateWindow();
	}
	
	CString strMsg;
	strMsg.Format(_T("Item Count Change : nCx = %d, nCy = %d, nRowCount = %d"),nCx,nCy,nRowCount);
	m_lstMessge.AddString(strMsg);
	m_lstMessge.SetTopIndex(m_lstMessge.GetTopIndex() + 1);
}

void CTabtestDlg::OnWaTabItemStateChange(WZTAB_EVENT_PARA *pPara)
{
	CString strMsg;
	strMsg.Format(_T("State Change : item %d was %s"),pPara->para1, (pPara->para2 != 0) ? _T("actived") : _T("deactived"));
	m_lstMessge.AddString(strMsg);
	m_lstMessge.SetTopIndex(m_lstMessge.GetTopIndex() + 1);
}

LRESULT CTabtestDlg::OnWzTabEventNotify(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case EVENT_GROUP_MODE:
			OnWaTabItemGroupMode((WZTAB_EVENT_PARA *)lParam);
			break;
		case EVENT_ITEM_DELETE:
			OnWaTabItemDelete((WZTAB_EVENT_PARA *)lParam);
			break;
		case EVENT_ITEM_L_CLICK:
			OnWaTabItemLClick((WZTAB_EVENT_PARA *)lParam);
			break;
		case EVENT_ITEM_L_DBCLICK:
			OnWaTabItemLDbClick((WZTAB_EVENT_PARA *)lParam);
			break;
		case EVENT_ITEM_R_CLICK:
			OnWaTabItemRClick((WZTAB_EVENT_PARA *)lParam);
			break;
		case EVENT_TIP_NEED:
			OnWaTabItemTipTextNeed((WZTAB_EVENT_PARA *)lParam);
			break;
		case EVENT_ROW_COUNT_CHANGE:
			OnWaTabRowCountChange((WZTAB_EVENT_PARA *)lParam);
			break;
		case EVENT_ITEM_STATE_CHANGE:
			OnWaTabItemStateChange((WZTAB_EVENT_PARA *)lParam);
			break;
		default:
			break;
	}

	return 0;
}

BOOL CTabtestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_TabImages = ImageList_Create(16,16,ILC_COLOR32|ILC_MASK,0,2);
	HBITMAP hBmp = (HBITMAP)LoadImage(AfxGetResourceHandle(),(LPCTSTR)IDB_FILETYPE,IMAGE_BITMAP,0,0,LR_DEFAULTSIZE|LR_CREATEDIBSECTION);
	ImageList_AddMasked(m_TabImages,hBmp,RGB(212,208,200));
	::DeleteObject(hBmp);

    m_TabTheme.SetImageList(CImageList::FromHandle(m_TabImages));
    m_TabTheme.InitConfig(_T("E:\\Code\\tabtest_20071125\\Debug\\blue_graident.ini"));
    m_TabCtrl.SetTabTheme(&m_TabTheme, FALSE);

	m_TabCtrl.SetNotifyWnd(this);
	m_TabCtrl.CreateTabCtrl(this);
	CRect rcClient;
	GetClientRect(&rcClient);
	m_TabCtrl.MoveWindow(rcClient.left + 2,rcClient.top + 120,rcClient.Width() - 4,24);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTabtestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTabtestDlg::OnPaint() 
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
HCURSOR CTabtestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTabtestDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	CRect rcClient;
	GetClientRect(&rcClient);
	if(m_TabCtrl.GetSafeHwnd())
	{
		CSize st = m_TabCtrl.ReCalcAllTabsPosAndWndSize(rcClient.Width() - 12);

        m_TabCtrl.MoveWindow(rcClient.left + 2,rcClient.top + 120,st.cx,st.cy);
	}
}

void CTabtestDlg::OnBtnAdditems() 
{
	m_TabCtrl.InsertItem(0,_T("gpon_profile_public.h"),5, NULL);
	m_TabCtrl.InsertItem(1,_T("gpon_profile_public.c"),1, NULL);
	m_TabCtrl.InsertItem(2,_T("gpon_profile_public.txt"),8, NULL);

	m_TabCtrl.InsertItem(3,_T("gpon_profile_kknd.h"),5, NULL);
	m_TabCtrl.InsertItem(4,_T("gpon_profile_kknd.cpp"),2, NULL);
	m_TabCtrl.InsertItem(5,_T("gpon_profile_kknd.html"),4, NULL);

	m_TabCtrl.InsertItem(3,_T("wz_kknd_otdr.h"),5, NULL);
	m_TabCtrl.InsertItem(4,_T("wz_kknd_otdr.cpp"),2, NULL);
	m_TabCtrl.InsertItem(5,_T("wz_kknd_otdr.html"),4, NULL);

	m_TabCtrl.InsertItem(3,_T("zte_msan_epon.h"),5, NULL);
	m_TabCtrl.InsertItem(4,_T("zte_msan_epon.cpp"),2, NULL);
	m_TabCtrl.InsertItem(5,_T("zte_msan_epon.html"),4, NULL);

    m_TabCtrl.SetCurSel(1);
}

void CTabtestDlg::OnBtnGroup() 
{
	static BOOL bGroup = TRUE;
	m_TabCtrl.SetGroupSameName(bGroup);
	
	bGroup = !bGroup;
}

void CTabtestDlg::OnBtnShowclose() 
{
	static BOOL bShowClose = TRUE;
	m_TabCtrl.SetShowCloseBtn(bShowClose);
	
	bShowClose = !bShowClose;
}

void CTabtestDlg::OnBnClickedBtnDeleteall()
{
	m_TabCtrl.DeleteAllItems();
}

void CTabtestDlg::OnBnClickedBtnDeleteitem()
{
	m_TabCtrl.DeleteItem(m_TabCtrl.GetCurSel());
}

void CTabtestDlg::OnBnClickedBtnTheme()
{
    CThemeMakerDlg dlg;

    dlg.DoModal();
}

void CTabtestDlg::OnDestroy()
{
    CDialog::OnDestroy();

    if(m_TabImages != NULL)
    {
    	ImageList_Destroy(m_TabImages);
        m_TabImages = NULL;
    }
}
