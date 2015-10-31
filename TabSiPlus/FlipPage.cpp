// FlipPage.cpp : implementation file
/*********************************************************************
*    Author         : orbit
*    Date           : November 15, 2000
*    Last-modified  : June 4, 2002 
*    Contact us     : inte2000@163.com
**********************************************************************/

#include "stdafx.h"
#include "tabsiplus.h"
#include "FlipPage.h"
#include "Globals.h"
#include "SelectFolderDlg.h"
//#include "DirDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFlipPage property page

IMPLEMENT_DYNCREATE(CFlipPage, CPropertyPage)

//##ModelId=431C34CB010A
CFlipPage::CFlipPage() : CPropertyPage(CFlipPage::IDD)
{
    //{{AFX_DATA_INIT(CFlipPage)
    //}}AFX_DATA_INIT
}

//##ModelId=431C34CB010B
CFlipPage::~CFlipPage()
{
}

//##ModelId=431C34CB012F
void CFlipPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CFlipPage)
    DDX_Control(pDX, IDC_BTN_ADDFLPPATH, m_btnAdd);
    DDX_Control(pDX, IDC_BTN_DELFLPPATH, m_btnDel);
    DDX_Control(pDX, IDC_BTN_UPFLPPATH, m_btnUp);
    DDX_Control(pDX, IDC_BTN_DOWNFLPPATH, m_btnDown);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFlipPage, CPropertyPage)
    //{{AFX_MSG_MAP(CFlipPage)
    ON_BN_CLICKED(IDC_BTN_ADDFLPPATH, OnBtnAddflppath)
    ON_BN_CLICKED(IDC_BTN_DELFLPPATH, OnBtnDelflppath)
    ON_BN_CLICKED(IDC_BTN_UPFLPPATH, OnBtnUpflppath)
    ON_BN_CLICKED(IDC_BTN_DOWNFLPPATH, OnBtnDownflppath)
    ON_LBN_SELCHANGE(IDC_LIST_PATH, OnSelchangeListPath)
    ON_EN_CHANGE(IDC_EDIT_HEADFILE, OnChangeEditHeadfile)
    ON_EN_CHANGE(IDC_EDIT_CPPFILE, OnChangeEditCppfile)
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFlipPage message handlers

//##ModelId=431C34CB012D
BOOL CFlipPage::OnApply() 
{
    CString strTmp;
    
    g_strAFlipPath.RemoveAll();
    for(int i = 0; i < m_strAPath.GetSize(); i++)
    {
        if(m_szSelect[i] == _T('1'))
            g_strAFlipPath.Add(m_strAPath[i]);
    }
    m_szSelect[m_strAPath.GetSize()] = _T('\0');
//    m_strAPath.RemoveAt(0);
    CStringArray strATmp;
    strATmp.Copy(m_strAPath);
    strATmp.RemoveAt(0);
    CombineString(strATmp,strTmp);

    SetRegistryRootKey(HKEY_CURRENT_USER);
    SetStringRegPorpValue(lpszKeyRoot,lpszFlipPath,strTmp);
    SetStringRegPorpValue(lpszKeyRoot,lpszSelectPortion,&(m_szSelect[1]));

    GetDlgItemText(IDC_EDIT_HEADFILE,g_szHeadType,128);
    _tcslwr(g_szHeadType);
    SetStringRegPorpValue(lpszKeyRoot,lpszHeadType,g_szHeadType);

    GetDlgItemText(IDC_EDIT_CPPFILE,g_szCPPType,128);
    _tcslwr(g_szCPPType);
    SetStringRegPorpValue(lpszKeyRoot,lpszCppType,g_szCPPType);

    return CPropertyPage::OnApply();
}

//##ModelId=431C34CB0157
BOOL CFlipPage::OnInitDialog() 
{
    CPropertyPage::OnInitDialog();

    m_lstPath.SubclassDlgItem(IDC_LIST_PATH, this);
    LPINT lpTabStop = new int[1];
    lpTabStop[0] = 22;
    m_lstPath.SetTabStops(1, lpTabStop);
    delete[] lpTabStop;
    
    SetRegistryRootKey(HKEY_CURRENT_USER);

    CString strMultiLine;
    BOOL bSuccess = GetStringRegPorpValue(lpszKeyRoot,lpszFlipPath,strMultiLine.GetBuffer(8192),8192);
    strMultiLine.ReleaseBuffer();

    m_strAPath.RemoveAll();
    SplitString(strMultiLine,m_strAPath);

    GetStringRegPorpValue(lpszKeyRoot,lpszSelectPortion,&(m_szSelect[1]),60);//m_szSelect+1);

    TCHAR szDefaultPath[MAX_PATH];
    DWORD dwBufSize = SIZEOF_ARRAY(szDefaultPath);
    LocLoadString(IDS_DEFAULT_FLIP_PATH, szDefaultPath, &dwBufSize );
    m_strAPath.InsertAt(0,szDefaultPath,1);
    m_szSelect[0] = _T('1');


    TCHAR szBuf[128];

    bSuccess = GetStringRegPorpValue(lpszKeyRoot,lpszHeadType,szBuf,128);
    if(!bSuccess)
        _tcscpy(szBuf,g_szHeadType);
    SetDlgItemText(IDC_EDIT_HEADFILE,szBuf);
    bSuccess = GetStringRegPorpValue(lpszKeyRoot,lpszCppType,szBuf,128);
    if(!bSuccess)
        _tcscpy(szBuf,g_szCPPType);
    SetDlgItemText(IDC_EDIT_CPPFILE,szBuf);

    CString strTmp;
    int size = m_strAPath.GetSize();
    for(int i = 0; i < size; i++)
    {
        strTmp.Format(_T("%s\t%s"), lpszNull,m_strAPath[i]);
        m_lstPath.AddString(strTmp);
        m_lstPath.SetCheck(i,(m_szSelect[i] == _T('1')) ? 1 : 0);
    }

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


//##ModelId=431C34CB01D4
void CFlipPage::OnBtnAddflppath() 
{
    CSelectFolderDlg dlg;

    dlg.m_strFolderName = g_szCurDircetory;
    if(dlg.DoModal() == IDOK)
    {
        CString strTmp;
        int size = m_strAPath.GetSize();
        if(size >= 63)
        {
            AfxMessageBox(IDS_TOO_MUCH_FLIP_PATH);
            return;
        }

        for(int i = 0; i < m_strAPath.GetSize(); i++)
        {
            TRACE(_T("%s\r\n"),m_strAPath[i]);
            if(m_strAPath[i].CompareNoCase(dlg.m_strFolderName + _T("\\")) == 0)
                return;
        }

        strTmp.Format(_T("%s\t%s"), lpszNull,dlg.m_strFolderName + _T("\\"));
        m_lstPath.AddString(strTmp);
        m_strAPath.Add(dlg.m_strFolderName + _T("\\"));
        m_szSelect[size] = _T('1');
        m_lstPath.SetCheck(size,(m_szSelect[size] == _T('1')) ? 1 : 0);
        SetModified();
    }
}

//##ModelId=431C34CB01D6
void CFlipPage::OnBtnDelflppath() 
{
    int idx = m_lstPath.GetCurSel();
    int count = m_strAPath.GetSize();
    if((idx <= 0) || (idx >= count))
        return;
    m_lstPath.DeleteString(idx);
    m_strAPath.RemoveAt(idx);
    for(int i = idx; i < count - 1; i++)
        m_szSelect[idx] = m_szSelect[idx + 1];
    SetModified();
}

//##ModelId=431C34CB01E4
void CFlipPage::OnBtnUpflppath() 
{
    int idx = m_lstPath.GetCurSel();
    int count = m_strAPath.GetSize();
    if((idx < 2) || (idx >= count))
        return;
//ListBox control exchange
    CString strTmp;
    m_lstPath.GetText(idx - 1,strTmp);
    m_lstPath.DeleteString(idx - 1);
    m_lstPath.InsertString(idx,strTmp);
//check mark array exchange
    TCHAR cTmp = m_szSelect[idx - 1];
    m_szSelect [idx - 1] = m_szSelect[idx];
    m_szSelect[idx] = cTmp;
    m_lstPath.SetCheck(idx,(m_szSelect[idx] == _T('1')) ? 1 : 0);
//m_strAPath exchange
    strTmp = m_strAPath[idx - 1];
    m_strAPath.SetAt(idx - 1,m_strAPath[idx]);
    m_strAPath.SetAt(idx,strTmp);
    SetModified();
}

//##ModelId=431C34CB01F5
void CFlipPage::OnBtnDownflppath() 
{
    int idx = m_lstPath.GetCurSel();
    int count = m_strAPath.GetSize();
    if((idx <= 0) || (idx >= count - 1))
        return;
//ListBox control exchange
    CString strTmp;
    m_lstPath.GetText(idx + 1,strTmp);
    m_lstPath.DeleteString(idx + 1);
    m_lstPath.InsertString(idx,strTmp);
//check mark array exchange
    TCHAR cTmp = m_szSelect[idx + 1];
    m_szSelect [idx + 1] = m_szSelect[idx];
    m_szSelect[idx] = cTmp;
    m_lstPath.SetCheck(idx,(m_szSelect[idx] == _T('1')) ? 1 : 0);
//m_strAPath exchange
    strTmp = m_strAPath[idx + 1];
    m_strAPath.SetAt(idx + 1,m_strAPath[idx]);
    m_strAPath.SetAt(idx,strTmp);    
    SetModified();
}

//##ModelId=431C34CB0203
void CFlipPage::OnSelchangeListPath() 
{
//do nothing when select the first item
    int idx = m_lstPath.GetCurSel();
    if(idx == -1)
        return;
    SetDlgItemText(IDC_EDIT_FULLSRHPATH,m_strAPath.GetAt(idx));

    int oldStatus = (m_szSelect[idx] == _T('1') ) ? 1 : 0;
    int bCheck = m_lstPath.GetCheck(idx);
    if(bCheck != oldStatus)
    {
        if(idx > 0)
            m_szSelect[idx] = (bCheck == 1) ? _T('1') : _T('0');
        SetModified();
    }
}

//##ModelId=431C34CB0205
void CFlipPage::OnChangeEditHeadfile() 
{
    SetModified();
}

//##ModelId=431C34CB0213
void CFlipPage::OnChangeEditCppfile() 
{
    SetModified();
}
