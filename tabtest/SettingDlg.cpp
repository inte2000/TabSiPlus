// SettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tabtest.h"
#include "SettingDlg.h"


// CSettingDlg dialog

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)

CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
    , m_bGroupSameName(FALSE)
    , m_bShowCloseBtn(FALSE)
{

}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHK_GROUP, m_bGroupSameName);
    DDX_Check(pDX, IDC_CHK_SHOWCLOSE, m_bShowCloseBtn);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
END_MESSAGE_MAP()


// CSettingDlg message handlers
