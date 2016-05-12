// tabtest.h : main header file for the TABTEST application
//

#if !defined(AFX_TABTEST_H__893847D8_A4F1_43D2_9ED7_2CEC68E3EEA6__INCLUDED_)
#define AFX_TABTEST_H__893847D8_A4F1_43D2_9ED7_2CEC68E3EEA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CTabtestApp:
// See tabtest.cpp for the implementation of this class
//

class CTabtestApp : public CWinApp
{
public:
	CTabtestApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTabtestApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CTabtestApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TABTEST_H__893847D8_A4F1_43D2_9ED7_2CEC68E3EEA6__INCLUDED_)
