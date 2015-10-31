/*********************************************************************
*    Author         : orbit
*    Date           : October 6, 2000
* Last-modified     : February 12, 2005 
*    E_mail         : inte2000@163.com
**********************************************************************/

#if !defined(AFX_OPTIONSHEET_H__C54F6982_479C_4D21_96B2_403259F3E4F4__INCLUDED_)
#define AFX_OPTIONSHEET_H__C54F6982_479C_4D21_96B2_403259F3E4F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OptionSheet.h : header file
//
#include "TabBarPage.h"
#include "FlipPage.h"
#include "GeneralPage.h"

/////////////////////////////////////////////////////////////////////////////
// COptionSheet

//##ModelId=431C34CA01B5
class COptionSheet : public CPropertySheet
{
    DECLARE_DYNAMIC(COptionSheet)

// Construction
public:
    //##ModelId=431C34CA01C5
    COptionSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    //##ModelId=431C34CA01D7
    COptionSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:
    //##ModelId=431C34CA01F5
    CTabBarPage        *m_pTabBarPage;
    //##ModelId=431C34CA01FA
    CFlipPage            *m_pFlipPage;
    CGeneralPage  *m_pGeneralPage;

    CWnd *m_pNotifyWnd;
// Operations
public:

// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(COptionSheet)
    //}}AFX_VIRTUAL

// Implementation
public:
    //##ModelId=431C34CA0242
    void AddPages();
    //##ModelId=431C34CA0243
    virtual ~COptionSheet();

    // Generated message map functions
protected:
    //{{AFX_MSG(COptionSheet)
        // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPTIONSHEET_H__C54F6982_479C_4D21_96B2_403259F3E4F4__INCLUDED_)
