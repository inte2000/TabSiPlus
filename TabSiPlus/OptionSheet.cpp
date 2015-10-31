// OptionSheet.cpp : implementation file
/*********************************************************************
*    Author         : orbit
*    Date           : October 6, 2000
* Last-modified     : February 12, 2005 
*    E_mail         : inte2000@163.com
**********************************************************************/
#include "stdafx.h"
#include "tabsiplus.h"
#include "OptionSheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COptionSheet

IMPLEMENT_DYNAMIC(COptionSheet, CPropertySheet)

//##ModelId=431C34CA01C5
COptionSheet::COptionSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
    :CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
    m_pNotifyWnd = pParentWnd;
    AddPages();
}

//##ModelId=431C34CA01D7
COptionSheet::COptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
    :CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
    m_pNotifyWnd = pParentWnd;
    AddPages();
}

//##ModelId=431C34CA0243
COptionSheet::~COptionSheet()
{
    //All pages was set WS_CHILD Property,Parent windows would call DestoryWindow method automatically
    if(m_pTabBarPage)
        delete m_pTabBarPage;
    if(m_pFlipPage)
        delete m_pFlipPage;
    if(m_pGeneralPage)
        delete m_pGeneralPage;
}


BEGIN_MESSAGE_MAP(COptionSheet, CPropertySheet)
    //{{AFX_MSG_MAP(COptionSheet)
        // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COptionSheet message handlers

//##ModelId=431C34CA0242
void COptionSheet::AddPages()
{
    m_pTabBarPage = new CTabBarPage;
    m_pTabBarPage->m_psp.dwFlags &= ~PSP_HASHELP;
    AddPage(m_pTabBarPage);//if m_pTabBarPage = NULL,AddPage method will throw out a assert

    m_pFlipPage = new CFlipPage;
    m_pFlipPage->m_psp.dwFlags &= ~PSP_HASHELP;
    AddPage(m_pFlipPage);

    m_pGeneralPage = new CGeneralPage;
    m_pGeneralPage->m_psp.dwFlags &= ~PSP_HASHELP;
    AddPage(m_pGeneralPage);
}

