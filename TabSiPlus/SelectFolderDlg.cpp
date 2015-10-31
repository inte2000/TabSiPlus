// SelectFolderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tabsiplus.h"
#include "SelectFolderDlg.h"
#include "DirDialog.h"
#include "Globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectFolderDlg dialog


CSelectFolderDlg::CSelectFolderDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CSelectFolderDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CSelectFolderDlg)
    m_strFolderName = _T("");
    m_bIncSubFolder = TRUE;
    //}}AFX_DATA_INIT
}


void CSelectFolderDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CSelectFolderDlg)
    DDX_Text(pDX, IDC_EDIT_FOLDER, m_strFolderName);
    DDV_MaxChars(pDX, m_strFolderName, 260);
    DDX_Check(pDX, IDC_CHK_INCLUDE_SUBFOLDER, m_bIncSubFolder);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectFolderDlg, CDialog)
    //{{AFX_MSG_MAP(CSelectFolderDlg)
    ON_BN_CLICKED(IDC_BTN_BOWRSE, OnBtnBowrse)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectFolderDlg message handlers

BOOL CSelectFolderDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();
    
    // TODO: Add extra initialization here
    
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectFolderDlg::OnBtnBowrse() 
{
    CString strTitle,strWinTitle;

    strTitle.LoadString(IDS_ADD_FLIP_PATH);
    strWinTitle.LoadString(IDS_ADD_FLIP_PATH);

    CDirDialog dlg;
    
    CString strTmp = m_strFolderName;
//    int idx = strTmp.ReverseFind(_T('\\'));
//    strTmp = strTmp.Left(idx);
    
    if(dlg.DoBrowse(this,strWinTitle,lpszNull,strTmp) == 1)
    {
        m_strFolderName = dlg.m_strPath;
        SetDlgItemText(IDC_EDIT_FOLDER,m_strFolderName);
    }
}

void CSelectFolderDlg::OnOK() 
{
    UpdateData(TRUE);
    if(GetFileAttributes(m_strFolderName) == 0xFFFFFFFF)
    {
        CString strMsg;
        strMsg.Format(IDS_FOLDER_ACCESS_ERROR,(LPCTSTR)m_strFolderName);
        AfxMessageBox(strMsg);
        return;
    }
    
    CDialog::OnOK();
}

