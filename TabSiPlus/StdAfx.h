// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__52E4E865_72C5_4B82_B02F_48DC6087B80E__INCLUDED_)
#define AFX_STDAFX_H__52E4E865_72C5_4B82_B02F_48DC6087B80E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#define VC_EXTRALEAN        // Exclude rarely-used stuff from Windows headers

#define WINVER 0x0500 // for multimonitor support

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>            // MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>            // MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>        // MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>            // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxtempl.h>

#include "SyncObject.h"
#include "TabSiPlusDllExport.h"
#include "Localization.h"
#include "Options.h"

#include "Globals.h"
#include "Debug.h"

#include <string>
#include <vector>
#include <map>
#include <stack>
#include <afxdlgs.h>
using namespace std;

typedef basic_string< TCHAR > TString;
//typedef vector< TString > TStringArray;
 
#ifndef SIZEOF_ARRAY
#define SIZEOF_ARRAY(arr) (sizeof(arr)/sizeof(arr[0]))
#endif

#define INT_TO_STR(x) #x
#define INT_TO_STR2(x) INT_TO_STR((x))
#define WARNING __FILE__  INT_TO_STR2(__LINE__) " : warning: "

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__52E4E865_72C5_4B82_B02F_48DC6087B80E__INCLUDED_)
