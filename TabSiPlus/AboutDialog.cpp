// AboutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "tabsiplus.h"
#include "AboutDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog dialog


CAboutDialog::CAboutDialog(CWnd* pParent /*=NULL*/)
    : CDialog(CAboutDialog::IDD, pParent)
{
    //{{AFX_DATA_INIT(CAboutDialog)
        // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}


void CAboutDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CAboutDialog)
        // NOTE: the ClassWizard will add DDX and DDV calls here
    DDX_Control(pDX, IDC_STC_HOMEPAGE, m_stcHome);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDialog, CDialog)
    //{{AFX_MSG_MAP(CAboutDialog)
    ON_WM_LBUTTONDOWN()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDialog message handlers

int CAboutDialog::DoModal() 
{
    // TODO: Add your specialized code here and/or call the base class
    
    return CDialog::DoModal();
}

BOOL CAboutDialog::DestroyWindow() 
{
    // TODO: Add your specialized code here and/or call the base class
    
    return CDialog::DestroyWindow();
}

BOOL CAboutDialog::OnInitDialog() 
{
    CDialog::OnInitDialog();

    m_stcHome.SetLinkCursor(AfxGetResourceHandle(),IDC_CURHAND);
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
    CDialog::OnLButtonDown(nFlags, point);
}
