// TabSiPlus.h : main header file for the TABSIPLUS DLL
//

#if !defined(AFX_TABSIPLUS_H__9F140E1C_1337_4D57_9470_89CA25FCA217__INCLUDED_)
#define AFX_TABSIPLUS_H__9F140E1C_1337_4D57_9470_89CA25FCA217__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
    #error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"        // main symbols

extern BOOL bExitPadding;
/////////////////////////////////////////////////////////////////////////////
// CTabSiPlusApp
// See TabSiPlus.cpp for the implementation of this class
//

class CTabSiPlusApp : public CWinApp
{
public:
    CTabSiPlusApp();
    ~CTabSiPlusApp();


public:
    BOOL Initialize();
    BOOL Deinitialize();

    BOOL IsAnotherTabSiPlusDll();

protected:
// Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CTabSiPlusApp)
    public:
    virtual BOOL InitInstance();
    virtual int ExitInstance();
    //}}AFX_VIRTUAL

    //{{AFX_MSG(CTabSiPlusApp)
        // NOTE - the ClassWizard will add and remove member functions here.
        //    DO NOT EDIT what you see in these blocks of generated code !
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABSIPLUS_H__9F140E1C_1337_4D57_9470_89CA25FCA217__INCLUDED_)
